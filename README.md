# Bank Queue Simulator (Poisson Distribution)

A C-based simulation program that models customer arrivals and wait times at a bank using the Poisson distribution.

## Project Description

This simulator helps bank managers make data-driven staffing decisions by analyzing customer wait times over an 8-hour business day (480 minutes). It uses probability theory to generate realistic customer arrival patterns and provides comprehensive statistical analysis of wait times.

## Concepts Implemented

### C Programming Concepts:
- **Structures (structs)**: `Customer`, `QueueNode`, `Queue` for organizing data
- **Pointers**: Extensively used for linked list implementation and dynamic memory
- **Dynamic Memory Management**: `malloc()`, `realloc()`, `calloc()`, `free()`
- **Linked Lists**: Queue implementation for efficient customer management
- **Functions**: Modular design with 15+ functions for separation of concerns
- **Loops**: `for` loops for 480-minute simulation
- **Conditional Logic**: `if/else` for customer processing and teller assignment

### Mathematics Concepts:
- **Poisson Distribution**: Models random customer arrivals (λ parameter)
- **Mean (Average)**: Central tendency measure of wait times
- **Median**: Middle value of sorted wait times
- **Mode**: Most frequent wait time
- **Standard Deviation**: Measure of wait time variability
- **Variance**: Square of standard deviation

## How to Compile
```bash
gcc bank_simulator.c -o bank_simulator -lm
```

**Note**: The `-lm` flag links the math library (required for `exp()`, `sqrt()`, `pow()` functions).

## How to Run
```bash
./bank_simulator
```

The program will prompt you to enter the average number of customers per minute (lambda). Recommended values:
- **0.5** - Slow day
- **1.0** - Normal day
- **1.5** - Busy day
- **2.0** - Very busy day

## Sample Output
```
Enter average customers per minute (lambda): 1.5

Total customers arrived: 723
Mean Wait Time: 8.45 minutes
Maximum Wait Time: 35 minutes
Standard Deviation: 6.23 minutes
```

## Key Features

- ✅ Dynamic queue management using linked lists
- ✅ Realistic Poisson-distributed arrivals
- ✅ Multiple statistical measures (mean, median, mode, std dev)
- ✅ Memory-efficient with proper cleanup
- ✅ Professional recommendations based on analysis

## Author

Created By Piyush Goyal

## License

Educational use only
