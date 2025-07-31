#include "../include/OrderBook.h"

FillReport OrderBook::addOrder(const Order& order) {
    
    // Check for valid order quantity
    if (order.quantity < 0) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    }
    if (order.type == OrderType::LIMIT && order.quantity == 0) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    }

    // Check if the order is already active
    if (isOrderActive(order.orderId)) {
        return FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());
    }

    return FillReport(OrderResult::RESTED, 0, std::vector<std::pair<Price, int> >());

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


// assumes that the order book is not empty
void OrderBook::matchOrders(const Order& order) {
    // TODO: Implement the logic to match orders based on the order type and side
}