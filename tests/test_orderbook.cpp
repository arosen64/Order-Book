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
void testBasicMarketBid();
void testBasicMarketAsk();
void testPartialBidFill();
void testPartialAskFill();
void testBidFillsAtDifferentPrices();
void testAskFillsAtDifferentPrices();
void testAskFillsAtDifferentPricesForDifferentParties();
void testBidFillsAtDifferentPricesForDifferentParties();
void partiallyFilledBidThenRejectedBid();
void partiallyFilledAskThenRejectedAsk();
void bidMaintainsTimeOrder();
void askMaintainsTimeOrder();
void testMultipleOrdersFillBid();
void testMultipleOrdersFillAsk();
void testLimitBidCrossesBook();
void testLimitAskCrossesBook();
void testRejectZeroQuantity();
void testRejectNegativeQuantity();
void testCancelAsk();
void testCancelBid();


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
    testBasicMarketBid();
    std::cout << "testBasicMarketBid passed!" << std::endl;
    testBasicMarketAsk();
    std::cout << "testBasicMarketAsk passed!" << std::endl;
    testPartialBidFill();
    std::cout << "testPartialBidFill passed!" << std::endl;
    testPartialAskFill();
    std::cout << "testPartialAskFill passed!" << std::endl;
    testBidFillsAtDifferentPrices();
    std::cout << "testBidFillsAtDifferentPrices passed!" << std::endl;
    testAskFillsAtDifferentPrices();
    std::cout << "testAskFillsAtDifferentPrices passed!" << std::endl;
    testAskFillsAtDifferentPricesForDifferentParties();
    std::cout << "testAskFillsAtDifferentPricesForDifferentParties passed!" << std::endl;
    testBidFillsAtDifferentPricesForDifferentParties();
    std::cout << "testBidFillsAtDifferentPricesForDifferentParties passed!" << std::endl;
    partiallyFilledBidThenRejectedBid();
    std::cout << "partiallyFilledBidThenRejectedBid passed!" << std::endl;
    partiallyFilledAskThenRejectedAsk();
    std::cout << "partiallyFilledAskThenRejectedAsk passed!" << std::endl;
    bidMaintainsTimeOrder();
    std::cout << "bidMaintainsTimeOrder passed!" << std::endl;
    askMaintainsTimeOrder();
    std::cout << "askMaintainsTimeOrder passed!" << std::endl;
    testMultipleOrdersFillBid();
    std::cout << "testMultipleOrdersFillBid passed!" << std::endl;
    testMultipleOrdersFillAsk();
    std::cout << "testMultipleOrdersFillAsk passed!" << std::endl;
    testLimitBidCrossesBook();
    std::cout << "testLimitBidCrossesBook passed!" << std::endl;
    testLimitAskCrossesBook();
    std::cout << "testLimitAskCrossesBook passed!" << std::endl;
    testRejectZeroQuantity();
    std::cout << "testRejectZeroQuantity passed!" << std::endl;
    testRejectNegativeQuantity();
    std::cout << "testRejectNegativeQuantity passed!" << std::endl;
    testCancelAsk();
    std::cout << "testCancelAsk passed!" << std::endl;
    testCancelBid();
    std::cout << "testCancelBid passed!" << std::endl;

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


void testBasicMarketBid() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 10, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::MARKET, Side::BUY, 0.0, 10, 2);
    FillReport report = orderBook.addOrder(order2);
    
    // Check if the market order was filled
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 10);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 10);
}

void testBasicMarketAsk() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 10, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::MARKET, Side::SELL, 0.0, 10, 2);
    FillReport report = orderBook.addOrder(order2);
    
    // Check if the market order was filled
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 10);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 10);
}

void testPartialBidFill() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::MARKET, Side::BUY, 0.0, 10, 2);
    FillReport report = orderBook.addOrder(order2);
    
    // Check if the market order was partially filled
    assert(report.status == OrderResult::PARTIALLY_FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
}

void testPartialAskFill() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::MARKET, Side::SELL, 0.0, 10, 2);
    FillReport report = orderBook.addOrder(order2);
    
    // Check if the market order was partially filled
    assert(report.status == OrderResult::PARTIALLY_FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
}

void testBidFillsAtDifferentPrices() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::SELL, 101.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::BUY, 0.0, 10, 3);
    FillReport report = orderBook.addOrder(order3);
    
    // Check if the market order was filled at both prices
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 10);
    assert(report.transactionPrices.size() == 2);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
    assert(report.transactionPrices[1].first == 101.0);
    assert(report.transactionPrices[1].second == 5);
}

void testAskFillsAtDifferentPrices() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::BUY, 99.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::SELL, 0.0, 10, 3);
    FillReport report = orderBook.addOrder(order3);
    
    // Check if the market order was filled at both prices
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 10);
    assert(report.transactionPrices.size() == 2);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
    assert(report.transactionPrices[1].first == 99.0);
    assert(report.transactionPrices[1].second == 5);
}

void testAskFillsAtDifferentPricesForDifferentParties() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::BUY, 99.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::SELL, 0.0, 5, 3);
    FillReport report = orderBook.addOrder(order3);
    
    Order order4(OrderType::MARKET, Side::SELL, 0.0, 5, 4);
    FillReport report1 = orderBook.addOrder(order4);

    // Check if the market order was filled at both prices
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
    assert(report1.status == OrderResult::FILLED);
    assert(report1.filledQuantity == 5);
    assert(report1.transactionPrices.size() == 1);
    assert(report1.transactionPrices[0].first == 99.0);
    assert(report1.transactionPrices[0].second == 5);
}

void testBidFillsAtDifferentPricesForDifferentParties() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::SELL, 101.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::BUY, 0.0, 5, 3);
    FillReport report = orderBook.addOrder(order3);
    
    Order order4(OrderType::MARKET, Side::BUY, 0.0, 5, 4);
    FillReport report1 = orderBook.addOrder(order4);

    // Check if the market orders were filled at both prices
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
    assert(report1.status == OrderResult::FILLED);
    assert(report1.filledQuantity == 5);
    assert(report1.transactionPrices.size() == 1);
    assert(report1.transactionPrices[0].first == 101.0);
    assert(report1.transactionPrices[0].second == 5);
}

void partiallyFilledBidThenRejectedBid() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::MARKET, Side::BUY, 0.0, 10, 2);
    FillReport report = orderBook.addOrder(order2);
    
    // Check if the market order was partially filled
    assert(report.status == OrderResult::PARTIALLY_FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);

    // Now try to add another bid that would be rejected
    Order order3(OrderType::MARKET, Side::BUY, 0.0, 10, 3);
    FillReport report1 = orderBook.addOrder(order3);

    // Check if the second market order was rejected
    assert(report1.status == OrderResult::REJECTED);
}

void partiallyFilledAskThenRejectedAsk() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::MARKET, Side::SELL, 0.0, 10, 2);
    FillReport report = orderBook.addOrder(order2);
    
    // Check if the market order was partially filled
    assert(report.status == OrderResult::PARTIALLY_FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);

    // Now try to add another ask that would be rejected
    Order order3(OrderType::MARKET, Side::SELL, 0.0, 10, 3);
    FillReport report1 = orderBook.addOrder(order3);

    // Check if the second market order was rejected
    assert(report1.status == OrderResult::REJECTED);
}

void bidMaintainsTimeOrder() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::BUY, 100.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::SELL, 0.0, 5, 3);
    FillReport report = orderBook.addOrder(order3);
    
    // Check if the market order was filled at the correct price
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0); // Should fill at the best bid price
    assert(report.transactionPrices[0].second == 5); // First order filled
    assert(orderBook.isOrderActive(1) == false); // First order should be filled
    assert(orderBook.isOrderActive(2) == true); // Second order should still be
}

void askMaintainsTimeOrder() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::SELL, 100.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::BUY, 0.0, 5, 3);
    FillReport report = orderBook.addOrder(order3);
    
    // Check if the market order was filled at the correct price
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 5);
    assert(report.transactionPrices.size() == 1);
    assert(report.transactionPrices[0].first == 100.0); // Should fill at the best ask price
    assert(report.transactionPrices[0].second == 5); // First order filled
    assert(orderBook.isOrderActive(1) == false); // First order should be filled
    assert(orderBook.isOrderActive(2) == true); // Second order should still be
}

void testMultipleOrdersFillBid() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::SELL, 101.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::BUY, 0.0, 10, 3);
    FillReport report = orderBook.addOrder(order3);
    
    // Check if the market order was filled at both prices
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 10);
    assert(report.transactionPrices.size() == 2);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
    assert(report.transactionPrices[1].first == 101.0);
    assert(report.transactionPrices[1].second == 5);
    assert(orderBook.isOrderActive(1) == false); // First order should be filled
    assert(orderBook.isOrderActive(2) == false); // Second order should be filled
}

void testMultipleOrdersFillAsk() {
    OrderBook orderBook;
    Order order1(OrderType::LIMIT, Side::BUY, 100.0, 5, 1);
    orderBook.addOrder(order1);
    
    Order order2(OrderType::LIMIT, Side::BUY, 99.0, 5, 2);
    orderBook.addOrder(order2);
    
    Order order3(OrderType::MARKET, Side::SELL, 0.0, 10, 3);
    FillReport report = orderBook.addOrder(order3);
    
    // Check if the market order was filled at both prices
    assert(report.status == OrderResult::FILLED);
    assert(report.filledQuantity == 10);
    assert(report.transactionPrices.size() == 2);
    assert(report.transactionPrices[0].first == 100.0);
    assert(report.transactionPrices[0].second == 5);
    assert(report.transactionPrices[1].first == 99.0);
    assert(report.transactionPrices[1].second == 5);
    assert(orderBook.isOrderActive(1) == false); // First order should be filled
    assert(orderBook.isOrderActive(2) == false); // Second order should be filled
}

void testLimitBidCrossesBook() {
    OrderBook ob;
    Order order1(OrderType::LIMIT, Side::SELL, 100.0, 5, 1);
    ob.addOrder(order1);
    Order order2(OrderType::LIMIT, Side::SELL, 102.0, 5, 2);
    ob.addOrder(order2);

    FillReport rpt = ob.addOrder(Order(OrderType::LIMIT, Side::BUY, 101.0, 8, 3));

    assert(rpt.status == OrderResult::PARTIALLY_FILLED);
    assert(rpt.transactionPrices.size() == 1);
    assert(rpt.transactionPrices[0].first == 100.0);
    assert(rpt.transactionPrices[0].second == 5);
    assert(rpt.filledQuantity == 5); // First 5 shares filled at 101
    // remaining 2 shares rest at 101
    assert(ob.isOrderActive(3));

    FillReport rpt1 = ob.addOrder(Order(OrderType::MARKET, Side::SELL, 0, 3, 4));
    assert(rpt1.status == OrderResult::FILLED);
    assert(rpt1.filledQuantity == 3);
    assert(rpt1.transactionPrices.size() == 1);
    assert(rpt1.transactionPrices[0].first == 101.0);
    assert(rpt1.transactionPrices[0].second == 3);
    assert(ob.isOrderActive(3) == false); // The limit order should be filled
}

void testLimitAskCrossesBook() {
    OrderBook ob;
    Order order1(OrderType::LIMIT, Side::BUY, 98.0, 5, 1);
    ob.addOrder(order1);
    Order order2(OrderType::LIMIT, Side::BUY, 99.0, 5, 2);
    ob.addOrder(order2);

    FillReport rpt = ob.addOrder(Order(OrderType::LIMIT, Side::SELL, 99.0, 8, 3));

    assert(rpt.status == OrderResult::PARTIALLY_FILLED);
    assert(rpt.transactionPrices.size() == 1);
    // remaining 2 shares rest at 99
    assert(ob.isOrderActive(3));

    FillReport rpt1 = ob.addOrder(Order(OrderType::MARKET, Side::BUY, 0, 3, 4));
    assert(rpt1.status == OrderResult::FILLED);
    assert(rpt1.filledQuantity == 3);
    assert(rpt1.transactionPrices.size() == 1);
    assert(rpt1.transactionPrices[0].first == 99.0);
    assert(rpt1.transactionPrices[0].second == 3);
    assert(ob.isOrderActive(3) == false); // The limit order should be filled
}

void testRejectZeroQuantity() {
    OrderBook ob;
    Order order(OrderType::LIMIT, Side::BUY, 100.0, 0, 1);
    FillReport rpt = ob.addOrder(order);
    assert(rpt.status == OrderResult::REJECTED);
}

void testRejectNegativeQuantity() {
    OrderBook ob;
    Order order(OrderType::LIMIT, Side::BUY, 100.0, -5, 1);
    FillReport rpt = ob.addOrder(order);
    assert(rpt.status == OrderResult::REJECTED);
}

void testCancelBid() {
    OrderBook ob;
    Order order(OrderType::LIMIT, Side::BUY, 100.0, 10, 1);
    ob.addOrder(order);
    
    // Cancel the order
    bool result = ob.cancelOrder(1);
    
    // Check if the order was cancelled successfully
    assert(result == true);
    
    // Check if the order is no longer active
    assert(ob.isOrderActive(1) == false);
}

void testCancelAsk() {
    OrderBook ob;
    Order order(OrderType::LIMIT, Side::SELL, 100.0, 10, 1);
    ob.addOrder(order);
    
    // Cancel the order
    bool result = ob.cancelOrder(1);
    
    // Check if the order was cancelled successfully
    assert(result == true);
    
    // Check if the order is no longer active
    assert(ob.isOrderActive(1) == false);
}