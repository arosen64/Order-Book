#pragma once
#include <map>
#include <list>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <cstdint>
#include <iostream>


using Price   = double;
using OrderID = uint64_t;

enum Side {BUY, SELL};
enum OrderType{LIMIT, MARKET};
enum OrderResult {FILLED, PARTIALLY_FILLED, REJECTED, RESTED};

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

class FillReport {
    public:
        FillReport(OrderResult status, int filledQuantity, std::vector<std::pair<Price, int> > transactionPrices) 
            : status(status), filledQuantity(filledQuantity), transactionPrices(transactionPrices) {}
        
    OrderResult status;
    int filledQuantity;
    std::vector<std::pair<Price, int> > transactionPrices; // List of prices and quantities for orders
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
        bool isOrderActive(OrderID orderId) const {
            return orderMap.find(orderId) != orderMap.end();
        }

        void printOrderBook() const; // Function to print the order book for debugging
        


    
        private:
            // Helper function to handle market and limit orders
            FillReport handleMarketOrder(Order& order);
            FillReport handleLimitOrder(Order& order);
        
            // the inputted iterators should start at the proper price level for the order type
            FillReport handleFillBuyOrder(Order& order, std::map<Price, std::list<Order> >::iterator& it);
            FillReport handleFillSellOrder(Order& order,std::map<Price, std::list<Order>, std::greater<Price> >::iterator& it);

            std::map<Price, std::list<Order>, std::greater<Price> > bids; // key: price, value: linked-list of orders
            std::map<Price, std::list<Order> > asks; // key: price, value: linked-list of orders
            std::unordered_map<OrderID, OrderDetails> orderMap; // key: orderId, value: Order

};