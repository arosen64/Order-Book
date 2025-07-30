// tests/test_orderbook.cpp
#include "../include/OrderBook.h"             // brings in Order, OrderBook, etc.
#include "../include/ConcurrentOrderBook.h"   // if you want to test the concurrent wrapper
#include <cassert>
#include <iostream>


void testAddLimitBidOrder();
void testAddLimitAskOrder();
void testMarketEmptyBookBid();
void testMarketEmptyBookAsk();
void testCancelOrderBid();
void testCancelOrderAsk();
// void testBasicMarketBid();
// void testBasicMarketAsk();
// void testPartialBidFill();
// void testPartialAskFill();
// void testBidFillsAtDifferentPrices();
// void testAskFillsAtDifferentPrices();
// void testAskFillsAtDifferentPricesForDifferentParties();
// void testBidFillsAtDifferentPricesForDifferentParties();
// void partiallyFilledBidThenRejectedBid();
// void partiallyFilledAskThenRejectedAsk();


int main() {
    testAddLimitBidOrder();
    std::cout << "testAddLimitBidOrder passed!" << std::endl;
    testAddLimitAskOrder();
    std::cout << "testAddLimitAskOrder passed!" << std::endl;
    testMarketEmptyBookBid();
    std::cout << "testMarketEmptyBookBid passed!" << std::endl;
    testMarketEmptyBookAsk();
    std::cout << "testMarketEmptyBookAsk passed!" << std::endl;
    testCancelOrderBid();
    std::cout << "testCancelOrderBid passed!" << std::endl;
    testCancelOrderAsk();
    std::cout << "testCancelOrderAsk passed!" << std::endl;
    // testBasicMarketBid();
    // std::cout << "testBasicMarketBid passed!" << std::endl;
    // testBasicMarketAsk();
    // std::cout << "testBasicMarketAsk passed!" << std::endl;
    // testPartialBidFill();
    // std::cout << "testPartialBidFill passed!" << std::endl;
    // testPartialAskFill();
    // std::cout << "testPartialAskFill passed!" << std::endl;
    // testBidFillsAtDifferentPrices();
    // std::cout << "testBidFillsAtDifferentPrices passed!" << std::endl;
    // testAskFillsAtDifferentPrices();
    // std::cout << "testAskFillsAtDifferentPrices passed!" << std::endl;
    // testAskFillsAtDifferentPricesForDifferentParties();
    // std::cout << "testAskFillsAtDifferentPricesForDifferentParties passed!" << std::endl;
    // testBidFillsAtDifferentPricesForDifferentParties();
    // std::cout << "testBidFillsAtDifferentPricesForDifferentParties passed!" << std::endl;
    // partiallyFilledBidThenRejectedBid();
    // std::cout << "partiallyFilledBidThenRejectedBid passed!" << std::endl;
    // partiallyFilledAskThenRejectedAsk();
    // std::cout << "partiallyFilledAskThenRejectedAsk passed!" << std::endl;

    std::cout << "All tests passed!" << std::endl;
    // Your main function implementation here
    return 0;
}

// OrderBook test cases
void testAddLimitBidOrder() {
    OrderBook orderBook;
    Order order(OrderType::LIMIT, Side::BUY, 100.0, 10, 1);
    FillReport report = orderBook.addOrder(order);
    
    // Check if the order was added correctly
    assert(report.status == OrderResult::RESTED);
}

void testAddLimitAskOrder() {
    OrderBook orderBook;
    Order order(OrderType::LIMIT, Side::SELL, 100.0, 10, 1);
    FillReport report = orderBook.addOrder(order);
    
    // Check if the order was added correctly
    assert(report.status == OrderResult::RESTED);
}

void testMarketEmptyBookBid() {
    OrderBook orderBook;
    Order order(OrderType::MARKET, Side::BUY, 0.0, 10, 1);
    FillReport report = orderBook.addOrder(order);
    
    // Check if the order was rejected due to empty book
    assert(report.status == OrderResult::REJECTED);
}

void testMarketEmptyBookAsk() {
    OrderBook orderBook;
    Order order(OrderType::MARKET, Side::SELL, 0.0, 10, 1);
    FillReport report = orderBook.addOrder(order);
    
    // Check if the order was rejected due to empty book
    assert(report.status == OrderResult::REJECTED);
}

void testCancelOrderBid() {
    OrderBook orderBook;
    Order order(OrderType::LIMIT, Side::BUY, 100.0, 10, 1);
    orderBook.addOrder(order);
    
    // Cancel the order
    bool result = orderBook.cancelOrder(1);
    
    // Check if the order was cancelled successfully
    assert(result == true);

    // double check by attempting to perform a market order
    Order order1(OrderType::MARKET, Side::BUY, 0.0, 10, 1);
    FillReport report = orderBook.addOrder(order1);
    
    // Check if the order was rejected due to empty book
    assert(report.status == OrderResult::REJECTED);
}

void testCancelOrderAsk() {
    OrderBook orderBook;
    Order order(OrderType::LIMIT, Side::SELL, 100.0, 10, 1);
    orderBook.addOrder(order);
    
    // Cancel the order
    bool result = orderBook.cancelOrder(1);
    
    // Check if the order was cancelled successfully
    assert(result == true);

    // double check by attempting to perform a market order
    Order order1(OrderType::MARKET, Side::BUY, 0.0, 10, 1);
    FillReport report = orderBook.addOrder(order1);
    
    // Check if the order was rejected due to empty book
    assert(report.status == OrderResult::REJECTED);
}





