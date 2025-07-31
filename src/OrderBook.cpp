#include "../include/OrderBook.h"

FillReport OrderBook::addOrder(const Order& order) {

    // initializing the storage of the juicy deets (start out as bad deets)
    FillReport deets = FillReport(OrderResult::REJECTED, 0, std::vector<std::pair<Price, int> >());

    
    // Check for valid order quantity
    if (order.quantity < 0) {
        return deets;
    }
    if (order.type == OrderType::LIMIT && order.quantity == 0) {
        return deets;
    }

    // Check if the order is already active
    if (isOrderActive(order.orderId)) {
        return deets;
    }

    // Getting the right book side
    std::map<Price, std::list<Order> > book = (order.side == Side::BUY) ? asks : bids;

    // make a copy of the order to store in the book if needed
    Order orderCopy(order);


    // attempt to match the order immediately
    if (!book.empty()) {
        // TODO: Implement the logic to match orders based on the order type and side
    // if this side of the book is empty, we reject the market order
    } else if (order.type == OrderType::MARKET) {
        return deets;
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


// assumes that the order book is not empty and 
void OrderBook::matchOrders(const Order& order) {
    // TODO: Implement the logic to match orders based on the order type and side

}