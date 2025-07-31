#include "../include/OrderBook.h"

// assumes that this is a limit order
FillReport OrderBook::handleLimitOrder(Order& order) {
    if (order.quantity <= 0) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    }

    if ((order.side == Side::BUY && asks.empty()) || (order.price < asks.begin()->first)) {
        bids[order.price].push_back(order);
        OrderDetails details = {order.side, order.price, --bids[order.price].end()};
        orderMap[order.orderId] = details;
        return FillReport(OrderResult::RESTED, 0, std::vector<std::pair<Price, int> >());
    }

    if ((order.side == Side::SELL && bids.empty()) || (order.price > bids.rbegin()->first)) {
        asks[order.price].push_back(order);
        OrderDetails details = {order.side, order.price, --asks[order.price].end()};
        orderMap[order.orderId] = details;
        return FillReport(OrderResult::RESTED, 0, std::vector<std::pair<Price, int> >());
    }

    return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
}

// assumes that this is a market order
FillReport OrderBook::handleMarketOrder(Order& order) {
    if (order.side == Side::BUY && asks.empty()) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    } else {
        FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
        auto it = asks.begin();

        // iterating through the prices in the book to fill the market bid
        while (it != asks.end() && order.quantity > 0) {
            // iterating through all the offers at the best price to fill the market bid
            while (!it->second.empty()) {
                int fillQuantity = std::min(order.quantity, it->second.front().quantity);
                order.quantity -= fillQuantity;
                deets.filledQuantity += fillQuantity;
                
                // decreasing the limit order quantity at this price
                it->second.front().quantity -= fillQuantity;
                deets.transactionPrices.push_back(std::pair<Price, int>(it->first, fillQuantity));
                int lastOrderId = it->second.front().orderId;


                if (it->second.front().quantity == 0) {
                    // if the order is fully filled, remove it from the book
                    it->second.pop_front();
                    orderMap.erase(lastOrderId);
                }
            }
        }
        // if the order is fully filled then indicate that is the case
        if (order.quantity == 0) {
            deets.status = OrderResult::FILLED;
        } else if (deets.filledQuantity > 0) {
            deets.status = OrderResult::PARTIALLY_FILLED;
        }
        return deets;
    }

    
    if (order.side == Side::SELL && bids.empty()) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    }
}

FillReport OrderBook::addOrder(const Order& order) {

    // initializing the storage of the juicy deets (start out as bad deets)
    FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());

    // make a copy of the order to store in the book if needed
    Order orderCopy(order);

    
    // Check for valid order quantity
    if (order.quantity < 0) {
        return deets;
    }

    if (order.type == OrderType::LIMIT) {
        return handleLimitOrder(orderCopy);
    }

    // in all other cases treat it as a market order
    return handleMarketOrder(orderCopy);
}

bool OrderBook::cancelOrder(OrderID orderId) {
    // 1) look up the order
    auto mapIt = orderMap.find(orderId);
    if (mapIt == orderMap.end()) 
        return false;

    // 2) pick the right book side
    auto& bookSide = (mapIt->second.side == Side::BUY) ? bids : asks;
    auto  price    = mapIt->second.price;

    // 3) find the list at that price
    auto lvlIt = bookSide.find(price);
    if (lvlIt != bookSide.end()) {
        // erase the node via the stored list‐iterator
        lvlIt->second.erase(mapIt->second.it);

        // if empty, remove that price level
        if (lvlIt->second.empty())
            bookSide.erase(lvlIt);
    }

    // 4) finally remove from the orderMap
    orderMap.erase(mapIt);
    return true;
}