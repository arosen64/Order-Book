#pragma once
#include <map>
#include <list>
#include <unordered_map>
#include <vector>

using OrderID = uint64_t;
using Price   = double;

enum class Side {BUY, SELL};
enum class OrderType{LIMIT, MARKET};
enum class OrderResult {FILLED, PARTIALLY_FILLED, REJECTED, RESTED};

class Order {
    public:
        OrderType type;
        Side side;
        Order(OrderType type, Side side, Price price, int quantity, OrderID orderId): 
            type(type), side(side), price(price), quantity(quantity), orderId(orderId) {}
        Price price;
        int quantity;
        int UserID; // TODO - implement users
        OrderID orderId; // Unique identifier for the order
};

struct FillReport {
    OrderResult status;
    int filledQuantity;
    std::vector<Price> transactionPrices;
};

struct OrderDetails {
  Side side;
  Price price;
  std::list<Order>::iterator it;
};

class OrderBook {
    public:
        FillReport addOrder(const Order& order);
        bool cancelOrder(OrderID orderId);
        // TODO add - bool modifyOrder(int orderId, const Order& newOrder);
    
        private:
            // Helper function to match orders
            void matchOrders();

            std::map<Price, std::list<Order> > bids; // key: price, value: linked-list of orders
            std::map<Price, std::list<Order> > asks; // key: price, value: linked-list of orders
            std::unordered_map<OrderID, OrderDetails> orderMap; // key: orderId, value: Order

};