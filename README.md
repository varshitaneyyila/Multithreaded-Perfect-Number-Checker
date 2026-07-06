# Multithreaded-Perfect-Number-Checker
1.  PROGRAM DESCRIPTION
* This program checks whether a given number N is a perfect number using a multithreaded approach.
* A perfect number is a number whose sum of all its proper divisors (excluding itself) is equal to the number.
* The program creates P threads, divides the work among them, and uses a mutex lock to safely store factors in a shared array and prevent race conditions.

---

2. REQUIREMENTS
* GCC Compiler
* POSIX Threads Library (pthread)
* Linux / Unix environment or Windows with MinGW/WSL support

---

3. COMPILATION

   Use the following command to compile the program:
   gcc B241352CS_A2.c -o perfect -lpthread -lm

---

4. EXECUTION

   Run the program using:
   ./perfect <N> <P>
   Where:
   N → Number to check
   P → Number of threads

---

5. EXAMPLE RUNS
   
    5.1. Example 1 (Perfect Number):
    ./perfect 28 4

    Output:
    28 is a PERFECT number

    5.2. Example 2 (Not a Perfect Number):
    ./perfect 10 3

    Output:
    10 is NOT a perfect number

---

6. PROGRAM LOGIC
   
1. The program reads two command-line arguments:
     N → the number to be checked
     P → the number of threads
2. The inputs are validated:
     If N ≤ 1 or P ≤ 0, the program prints "Invalid input" and exits.
3. The program computes the limit as:
     limit = √N
     This reduces the search space for finding factors.
4. If the number of threads P is greater than limit,
     it is adjusted to P = limit to avoid unnecessary threads.
5. Memory is allocated for:
     - An array of P threads
     - An array of ThreadData structures (to store start and end range)
6. The range [1, √N] is divided among P threads such that:
     Each thread gets a unique subrange with no overlap.
7. Each thread performs the following steps:
    a) Iterates through its assigned range [start, end]
    b) For each number i:
       - If N % i == 0, then i is a factor
    c) If i is a valid factor and i ≠ N:
      - The thread locks the mutex
      - Adds i to the shared array factors[]
      - Increments count
      - Unlocks the mutex
    d) Computes the paired factor:
       pair = N / i
   e) If pair ≠ i and pair ≠ N:
      - The thread locks the mutex
      - Adds pair to the shared array factors[]
      - Increments count
      - Unlocks the mutex
8. The main thread waits for all threads to complete execution
    using pthread_join().
9. After all threads finish, the main thread computes the sum:
    sum = sum of all elements in factors[]
10. The result is determined:
    - If sum == N → the number is PERFECT
    - Otherwise → the number is NOT a perfect number
11. The result is printed to the output.
12. Finally:
    - The mutex is destroyed
    - Allocated memory is freed
    - The program terminates

---

7. SYNCHRONIZATION DETAILS
A mutex (mutual exclusion lock) is used to protect shared resources.
* The shared array factors[] and variable count are accessed by multiple threads.
* To avoid race conditions, a mutex lock is used before inserting elements.
* Only one thread can access the critical section at a time.
* After updating the shared data, the mutex is released.

---

8. NOTES
*  The program uses square root optimization to reduce computation.
*   Factor pairing (i and N/i) is used to efficiently find divisors.
