# Prime Number Finder

## Overview
This project is a multi-threaded prime number finder written in C++. It determines prime numbers within a given range using multiple threads for improved performance.

## Features
1. **Printing variations**
- Print when a prime is found
- Wait until all threads are done.
2. **Task Division Schemes**
- Straight division of search range
- The search is linear but the threads are for divisibility testing of individual numbers.

### Changing Parameters
On lines 98-99, you can change the boolean of the parameters for different printing and task division variations.

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/PrimeFinder.git
   cd PrimeFinder
   ```

2. Ensure you have a valid `config.txt` file in the project directory:
   ```
   x 4    # Number of threads
   y 1000 # Max number to check for primes
   ```

## Compilation
Compile using `g++` (for Linux/macOS) or `cl.exe` (for Windows):

### **Linux/macOS** (G++)
```sh
 g++ -std=c++11 -pthread Main.cpp -o PrimeFinder
```

### **Windows** (MSVC)
```sh
 cl /EHsc /std:c++11 Main.cpp
```

## Usage
Run the compiled executable:
```sh
 ./PrimeFinder
```
### **Expected Output:**
```
Start: 2025-02-07 12:00:00
Thread 0 found prime: 2 at 2025-02-07 12:00:01
Thread 1 found prime: 3 at 2025-02-07 12:00:01
...
Primes found:
2, 3, 5, 7, 11, ...
End: 2025-02-07 12:00:05
```
