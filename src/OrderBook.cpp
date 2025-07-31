#include "../include/OrderBook.h"

// assumes that this is a limit order
FillReport OrderBook::handleLimitOrder(Order& order) {
    if (order.quantity <= 0) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    }

    if (order.side == Side::BUY && asks.empty()) {
        bids[order.price].push_back(order);
        OrderDetails details = {order.side, order.price, --bids[order.price].end()};
        orderMap[order.orderId] = details;
        return FillReport(OrderResult::RESTED, 0, std::vector<std::pair<Price, int> >());
    }

    if (order.side == Side::SELL && bids.empty()) {
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