#include <iostream>
#include <iomanip>
#include <limits>
#include "../include/OrderBook.h"

OrderID nextOrderId = 1;

int main() {
    OrderBook ob;
    while (true) {
        ob.printOrderBook(); // Print the current state of the order book
        std::cout << "\nMenu:\n"
                  << "1. Place LIMIT BUY\n"
                  << "2. Place LIMIT SELL\n"
                  << "3. Place MARKET BUY\n"
                  << "4. Place MARKET SELL\n"
                  << "5. Exit\n"
                  << "Choose: ";
        int choice;
        std::cin >> choice;
        if (choice == 5) break;

        double price = 0.0;
        int qty = 0;
        if (choice == 1 || choice == 2) {
            std::cout << "Enter price: ";
            std::cin >> price;
        }
        std::cout << "Enter quantity: ";
        std::cin >> qty;

        FillReport report(OrderResult::REJECTED, 0, std::vector<std::pair<double, int> >());
        if (choice == 1) {
            Order order(OrderType::LIMIT, Side::BUY, price, qty, nextOrderId++);
            report = ob.addOrder(order);
        } else if (choice == 2) {
            Order order(OrderType::LIMIT, Side::SELL, price, qty, nextOrderId++);
            report = ob.addOrder(order);
        } else if (choice == 3) { 
            Order order(OrderType::MARKET, Side::BUY, 0.0, qty, nextOrderId++);
            report = ob.addOrder(order);
        } else if (choice == 4) {
            Order order(OrderType::MARKET, Side::SELL, 0.0, qty, nextOrderId++);
            report = ob.addOrder(order);
        } else {
            std::cout << "Invalid choice.\n";
            continue;
        }

        std::cout << "Order result: ";
        switch (report.status) {
            case OrderResult::FILLED: std::cout << "FILLED"; break;
            case OrderResult::PARTIALLY_FILLED: std::cout << "PARTIALLY FILLED"; break;
            case OrderResult::REJECTED: std::cout << "REJECTED"; break;
            case OrderResult::RESTED: std::cout << "RESTED"; break;
        }
        std::cout << "\nFilled Quantity: " << report.filledQuantity << "\n";
        if (!report.transactionPrices.empty()) {
            std::cout << "Fills:\n";
            for (const auto& fill : report.transactionPrices) {
                std::cout << "  Price: " << fill.first << " Qty: " << fill.second << "\n";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Goodbye!\n";
    return 0;
}