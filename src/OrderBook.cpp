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
    auto it = orderMap.find(orderId);
    if (it != orderMap.end()) {
        // Remove the order from the orderMap
        orderMap.erase(it);
        return true;
    }
    return false;
}

// assumes that the order book is not empty
void OrderBook::matchOrders(const Order& order) {
}