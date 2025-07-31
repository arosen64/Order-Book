# Order Book Matching Engine

This project implements a simple **order book matching engine** in C++, providing the core functionality for placing, matching, and canceling limit and market orders. It is intended as a learning and prototyping tool for exchange-style trading systems.

> **Status:** In Development 🚧  
> The current implementation is single-threaded. The next major milestone is to support multiple concurrent users accessing the shared order book safely.

---

## Key Features

- **Limit Orders**: Place buy or sell orders at a specified price and quantity.  
- **Market Orders**: Execute immediately against the best available prices.  
- **Order Matching**: Automatic matching with FIFO time priority within each price level.  
- **Partial Fills**: Support for orders that are partially filled and then either rest or reject the remainder.  
- **Order Cancellation**: Remove an active order by its unique ID.  
- **Top-of-Book Display**: Utility function to print the top 5 bids and asks.  
- **Unit Tests**: Extensive test suite covering core behaviors in `tests/test_orderbook.cpp`.  

---

## Project Structure


- **`include/OrderBook.h`**: Declarations of `Order`, `FillReport`, and `OrderBook` classes.  
- **`src/OrderBook.cpp`**: Core matching logic implementations (limit & market orders, fills, cancellations).  
- **`src/main.cpp`**: Simple CLI for placing orders and visualizing the order book.  
- **`tests/test_orderbook.cpp`**: Unit tests validating behavior across a range of scenarios.  

---

## Data Structures & Logic

### Price Levels & Order Queues

- **`std::map<Price, std::list<Order>, std::greater<Price>> bids;`**  
  - Sorted in **descending** order of price.  
  - Each key is a price level; the corresponding `std::list<Order>` maintains FIFO order for orders at that price.  

- **`std::map<Price, std::list<Order>> asks;`**  
  - Sorted in **ascending** order of price.  
  - Similar structure for sell orders.  

- **`std::unordered_map<OrderID, OrderDetails> orderMap;`**  
  - Maps each active order ID to its side, price level, and list iterator for O(1) cancellation and lookup.  

These structures enable:  
1. **Fast best-price retrieval** (`begin()` of each map).  
2. **FIFO matching** via `std::list` operations.  
3. **Efficient cancellation** by erasing the stored list iterator and cleaning empty levels.  

### Order Matching Flow

1. **Add Order (`addOrder`)**  
   - Rejects invalid quantities (≤ 0).  
   - Dispatches to `handleLimitOrder` or `handleMarketOrder`.  

2. **Limit Orders**  
   - **Buy**: If best ask price ≤ limit price, fill via `handleFillBuyOrder`; otherwise, rest in the bids map.  
   - **Sell**: Symmetric logic against bids.  
   - Partially filled orders are either marked `PARTIALLY_FILLED` (with the remainder resting) or `FILLED` if completely matched.  

3. **Market Orders**  
   - Execute immediately against the best available opposite-side price levels until filled or book is exhausted.  
   - Return `REJECTED` if the book is empty on the opposite side.  

4. **Filling Functions**  
   - **`handleFillBuyOrder` / `handleFillSellOrder`**:  
     - Iterate through price levels and orders until the incoming order is satisfied or no more matching levels.  
     - Track `filledQuantity` and record each `(price, quantity)` trade in `FillReport.transactionPrices`.  
     - Clean up empty list nodes and price levels, updating `orderMap` accordingly.  

5. **Cancel Order (`cancelOrder`)**  
   - Lookup in `orderMap` for O(1) access.  
   - Erase the order from its `std::list` via stored iterator.  
   - Remove the price level if it becomes empty and erase from `orderMap`.  

---

## Building & Running

1. **Prerequisites**: C++17-compatible compiler (e.g., GCC, Clang)

2. **Compile**:
   ```make all```

3. **Use Interactive OrderBook**
   ```./main```

4. **Run tests**
   ```./test_orderbook```
