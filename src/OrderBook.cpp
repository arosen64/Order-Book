#include "../include/OrderBook.h"

// assumes that this is a limit order
FillReport OrderBook::handleLimitOrder(Order& order) {

    FillReport deets = FillReport(OrderResult::REJECTED, 0, 
        std::vector<std::pair<Price, int> >());

    if (order.quantity <= 0) {
        return deets;
    }

    // if there is an appealing ask for this bid

    if ((order.side == Side::BUY) && !asks.empty() && (order.price >= asks.begin()->first)) {
        auto it = asks.begin();
        deets = handleFillBuyOrder(order, it);
        if (deets.status == OrderResult::FILLED) {
            return deets;
        }
    }

    // in the case that we are partially filled after the filling the bid then this bid necomes rested
    if ((order.side == Side::BUY && asks.empty()) || (deets.status == OrderResult::PARTIALLY_FILLED)) {
        bids[order.price].push_back(order);
        OrderDetails details = {order.side, order.price, --bids[order.price].end()};
        orderMap[order.orderId] = details;
        deets.status = (deets.status == OrderResult::REJECTED) ? OrderResult::RESTED : deets.status;
        return deets;
    }

    deets = FillReport(OrderResult::REJECTED, 0, 
        std::vector<std::pair<Price, int> >());

    // if there is an appealing bid for this ask
    if ((order.side == Side::SELL) && !bids.empty() && (order.price <= bids.begin()->first)) {
        auto it = bids.begin();
        deets = handleFillSellOrder(order, it);
        if (deets.status == OrderResult::FILLED) {
            return deets;
        }
    }

    if ((order.side == Side::SELL && bids.empty()) || (deets.status == OrderResult::PARTIALLY_FILLED)) {
        asks[order.price].push_back(order);
        OrderDetails details = {order.side, order.price, --asks[order.price].end()};
        orderMap[order.orderId] = details;
        deets.status = (deets.status == OrderResult::REJECTED) ? OrderResult::RESTED : deets.status;
        return deets;
    }

    return deets;
}


// assumes that this is a market order
FillReport OrderBook::handleMarketOrder(Order& order) {
    // handles buy orders
    if (order.side == Side::BUY && asks.empty()) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    } else if (order.side == Side::BUY) {
        FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
        auto it = asks.begin();
        return handleFillBuyOrder(order, it);
    }

    // handles sell orders
    if (order.side == Side::SELL && bids.empty()) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    } else if (order.side == Side::SELL) {
        FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
        auto it = bids.begin();
        return handleFillSellOrder(order, it);
    }

    return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
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

    auto  price    = mapIt->second.price;

    // 2) find the list at that price
    if (mapIt->second.side == Side::BUY) {
        auto lvlIt = bids.find(price);
        if (lvlIt != bids.end()) {
            // erase the node via the stored list‐iterator
            lvlIt->second.erase(mapIt->second.it);

            // if empty, remove that price level
            if (lvlIt->second.empty())
                bids.erase(lvlIt);
        }
    } else {
        auto lvlIt = asks.find(price);
        if (lvlIt != asks.end()) {
            // erase the node via the stored list‐iterator
            lvlIt->second.erase(mapIt->second.it);

            // if empty, remove that price level
            if (lvlIt->second.empty())
                asks.erase(lvlIt);
        }
    }

    // 3) finally remove from the orderMap
    orderMap.erase(mapIt);
    return true;
}

FillReport OrderBook::handleFillBuyOrder(Order& order, std::map<Price, std::list<Order> >::iterator& it) {
    FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    while (it != asks.end() && order.quantity > 0) {
        if (order.type == OrderType::LIMIT && it->first > order.price) {
            break;
        }
        while (!it->second.empty() && order.quantity > 0) {
            int fillQuantity = std::min(order.quantity, it->second.front().quantity);
            order.quantity -= fillQuantity;
            deets.filledQuantity += fillQuantity;
            it->second.front().quantity -= fillQuantity;
            deets.transactionPrices.push_back(std::pair<Price, int>(it->first, fillQuantity));
            int lastOrderId = it->second.front().orderId;

            if (it->second.front().quantity == 0) {
                it->second.pop_front();
                orderMap.erase(lastOrderId);
            }
        }
        if (it->second.empty()) {
            it = asks.erase(it); // erase returns the next valid iterator
        } else {
            ++it;
        }
    }
    if (order.quantity == 0) {
        deets.status = OrderResult::FILLED;
    } else if (deets.filledQuantity > 0) {
        deets.status = OrderResult::PARTIALLY_FILLED;
    }
    return deets;
}

FillReport OrderBook::handleFillSellOrder(Order& order, std::map<Price, std::list<Order> >::iterator& it) {
    FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    while (it != bids.end() && order.quantity > 0) {
        if (order.type == OrderType::LIMIT && it->first < order.price) {
            break;
        }
        while (!it->second.empty() && order.quantity > 0) {
            int fillQuantity = std::min(order.quantity, it->second.front().quantity);
            order.quantity -= fillQuantity;
            deets.filledQuantity += fillQuantity;
            it->second.front().quantity -= fillQuantity;
            deets.transactionPrices.push_back(std::pair<Price, int>(it->first, fillQuantity));
            int lastOrderId = it->second.front().orderId;

            if (it->second.front().quantity == 0) {
                it->second.pop_front();
                orderMap.erase(lastOrderId);
            }
        }
        if (it->second.empty()) {
            it = bids.erase(it); // erase returns the next valid iterator
        } else {
            ++it;
        }
    }
    if (order.quantity == 0) {
        deets.status = OrderResult::FILLED;
    } else if (deets.filledQuantity > 0) {
        deets.status = OrderResult::PARTIALLY_FILLED;
    }
    return deets;
}

void OrderBook::printOrderBook() const {
    std::cout << "\n--- Top 5 Bids ---\n";
    int count = 0;
    for (auto it = bids.begin(); it != bids.end() && count < 5; ++it) {
        for (const auto& order : it->second) {
            std::cout << "Price: " << it->first
                      << " | Qty: " << order.quantity
                      << " | ID: " << order.orderId << '\n';
            if (++count >= 5) break;
        }
        if (count >= 5) break;
    }
    if (count == 0) std::cout << "(none)\n";

    std::cout << "--- Top 5 Asks ---\n";
    count = 0;
    for (auto it = asks.begin(); it != asks.end() && count < 5; ++it) {
        for (const auto& order : it->second) {
            std::cout << "Price: " << it->first
                      << " | Qty: " << order.quantity
                      << " | ID: " << order.orderId << '\n';
            if (++count >= 5) break;
        }
        if (count >= 5) break;
    }
    if (count == 0) std::cout << "(none)\n";
}