# Performance Testing of a Custom Database

This project compares the performance of a custom AVL tree implementation with the standard `std::map` container. It is part of a group assignment to evaluate data structures in a constrained, real-time environment. The test suite performs the following tests on both implementations:

- Insertion Correctness
- Deletion Correctness
- Maximum Size (catching `bad_alloc`)
- Load Testing (repeated insertion and access)
- Search Speed (worst-case)
- Memory Leak Simulation

The test suite has been run on both macOS and Windows (VM), and the outputs are provided below.

---

## Contents

- [Project Overview](#project-overview)
- [How to Build and Run](#how-to-build-and-run)
- [Test Suite Outputs](#test-suite-outputs)
  - [macOS Output](#macos-output)
  - [Windows Output](#windows-output)
- [Assignment Questions & Answers](#assignment-questions--answers)
- [Observations and Recommendations](#observations-and-recommendations)
- [Reusability](#reusability)

---

## Project Overview

This project implements an AVL tree and tests it against the C++ Standard Library’s `std::map` for common operations, with a focus on performance in constrained memory environments. The custom AVL tree is used as the basis for our “custom database,” and the test suite evaluates both correctness and performance aspects.

---

## How to Build and Run

### Requirements

- **Compiler:** `g++` with C++11 support (or later)
- **Build Tool:** `make`
- **Operating Systems:** Tested on macOS and Windows (via VM)

### Build Instructions

1. Open your terminal.
2. Run the following commands:
   ```bash
   make clean
   make
   ./run_tests.sh
   ```

## Test Suite Outputs

### MacOS Output

```txt
Running the AVL test suite...
=============================================
Starting Database Performance Tests
=============================================

==== Running AVL Tree Tests ====

[AVL] Insertion Correctness Test Started...
[AVL] Insertion test passed.
[AVL] Insertion Correctness Test Completed.

Press Enter to continue...

[AVL] Deletion Correctness Test Started...
[AVL] Deletion test passed.
[AVL] Deletion Correctness Test Completed.

Press Enter to continue...

[AVL] Maximum Size Test Started...
[AVL] Current memory usage: 1 MB
[AVL] Current memory usage: 230 MB
[AVL] Current memory usage: 459 MB
[AVL] Current memory usage: 689 MB
testMaxSizeAVL: Max size AVL: 14999999
[AVL] Maximum Size Test Completed.
Press Enter to continue...

[AVL] Load Test (50000 iterations) Started...
[AVL] Load test completed.
[AVL] Load Test Completed.
Press Enter to continue...

[AVL] Search Speed Test with 100000 elements Started...
[AVL] Time to search for minimum element (key 0): 2.5e-06 seconds.
[AVL] Time to search for maximum element (key 99999): 2.09e-07 seconds.
[AVL] Search Speed Test Completed.
Press Enter to continue...

[AVL] Memory Leak Test Simulation (100 iterations) Started...
[AVL] Memory Leak Test Simulation Completed.
Press Enter to continue...

==== Running std::map Tests ====

[map] Insertion Correctness Test Started...
[map] Insertion test passed.
[map] Insertion Correctness Test Completed.
Press Enter to continue...

[map] Deletion Correctness Test Started...
[map] Deletion test passed.
[map] Deletion Correctness Test Completed.
Press Enter to continue...

[map] Maximum Size Test Started...
[map] Current memory usage: 689 MB
[map] maxStorageCapacity: 752 MB
[map] Current memory usage: 689 MB
[map] Current memory usage: 689 MB
[map] Current memory usage: 776 MB
[map] Maximum Size Test Completed.
testMaxSizeMap: Max size of std::map is: 14999999
Press Enter to continue...

[map] Load Test (50000 iterations) Started...
[map] Load test completed.
[map] Load Test Completed.
Press Enter to continue...

[map] Search Speed Test with 100000 elements Started...
[map] Time to search for minimum element (key 0): 1.125e-06 seconds.
[map] Time to search for maximum element (key 99999): 3.33e-07 seconds.
[map] Search Speed Test Completed.
Press Enter to continue...

[map] Memory Leak Test Simulation (100 iterations) Started...
[map] Memory Leak Test Simulation Completed.
Press Enter to continue...

=============================================
All tests completed.
=============================================
```

### Windows Output

```txt
=============================================
Starting Database Performance Tests
=============================================

==== Running AVL Tree Tests ====

[AVL] Insertion Correctness Test Started...
[AVL] Insertion test passed.
[AVL] Insertion Correctness Test Completed.
Press Enter to continue...

[AVL] Deletion Correctness Test Started...
[AVL] Deletion test passed.
[AVL] Deletion Correctness Test Completed.
Press Enter to continue...

[AVL] Maximum Size Test Started...
[AVL] Current memory usage: 4 MB
[AVL] Current memory usage: 7 MB
testMaxSizeAVL: Max size AVL: 4999999
[AVL] Maximum Size Test Completed.
Press Enter to continue...

[AVL] Load Test (50000 iterations) Started...
[AVL] Load test completed.
[AVL] Load Test Completed.
Press Enter to continue...

[AVL] Search Speed Test with 100000 elements Started...
[AVL] Time to search for minimum element (key 0): 0 seconds.
[AVL] Time to search for maximum element (key 99999): 0 seconds.
[AVL] Search Speed Test Completed.
Press Enter to continue...

[AVL] Memory Leak Test Simulation (100 iterations) Started...
[AVL] Memory Leak Test Simulation Completed.
Press Enter to continue...

==== Running std::map Tests ====

[map] Insertion Correctness Test Started...
[map] Insertion test passed.
[map] Insertion Correctness Test Completed.
Press Enter to continue...

[map] Deletion Correctness Test Started...
[map] Deletion test passed.
[map] Deletion Correctness Test Completed.
Press Enter to continue...

[map] Maximum Size Test Started...
[map] Current memory usage: 4 MB
[map] maxStorageCapacity: 7 MB
[map] Current memory usage: 545 MB
[map] Maximum Size Test Completed.
testMaxSizeMap: Max size of std::map is: 4999999
Press Enter to continue...

[map] Load Test (50000 iterations) Started...
[map] Load test completed.
[map] Load Test Completed.
Press Enter to continue...

[map] Search Speed Test with 100000 elements Started...
[map] Time to search for minimum element (key 0): 0 seconds.
[map] Time to search for maximum element (key 99999): 0 seconds.
[map] Search Speed Test Completed.
Press Enter to continue...

[map] Memory Leak Test Simulation (100 iterations) Started...
[map] Memory Leak Test Simulation Completed.
Press Enter to continue...

=============================================
All tests completed.
=============================================
```

## Assignment Questions & Answers

### 1. Are there any other tests that you feel are required? Describe them briefly.

- Concurrency and Thread Safety: It would be beneficial to perform tests under multi-threaded conditions to ensure both data structures behave correctly when accessed concurrently.

- Adversarial Data Patterns: Testing with worst-case input (e.g., sorted or reverse-sorted data) could highlight performance issues when the tree becomes unbalanced or when std::map is stressed.

- Long-Term Stability: A prolonged stress test that repeatedly performs a cycle of insertions and deletions would help reveal issues such as memory fragmentation or subtle leaks.

### 2. Compare and contrast std::map vs AVL tree for all six test cases above.

- Insertion & Deletion Correctness: Both structures performed correctly with no issues observed on either platform.

- Maximum Size: Both AVL tree and std::map reached a similar maximum size on macOS (~15 million elements) while Windows showed a lower limit (~5 million elements), likely due to the VM’s memory constraints.

- Load Testing: Both implementations handled 50,000 iterations well, indicating robustness in continuous operations.

- Search Speed: Both data structures performed extremely fast lookups. Windows reported 0 seconds for both minimum and maximum searches due to timer resolution limitations, while macOS times were in the microsecond range.

- Memory Leak Simulation: Neither implementation showed memory leak issues in the simulation tests.

- Overall: The performance is comparable, with the differences attributable mainly to platform-specific behaviors.

#### Were there unexpected results?

- Timer Resolution on Windows:
  The search speed tests on Windows reported 0 seconds, which was unexpected at first but is explained by the lower timer resolution on that platform.
- Memory Behavior Differences:
  The maximum size results differ between macOS and Windows (or VM vs. native) due to differences in memory management and reporting.

#### Which would you recommend, std::map or AVL Tree?

- I recommend using std::map for production systems because it is part of the standard library, is well-tested, and benefits from ongoing optimizations. The custom AVL tree is valuable for specific use cases or performance tuning, but in general, std::map is more robust and easier to maintain.

#### Based on memory tests, what maximum memory would you recommend be reserved for the database?

- Based on the macOS tests, under worst-case scenarios the database may use up to about 2 GB. Therefore, I would recommend reserving approximately 2 GB of memory for the database.

### 3. Is this AVL Tree reusable for other projects?

- Yes, the AVL tree has a clean interface and is implemented as a self-contained module (with functions like insert, remove, find, and display), making it reusable in other projects with little to no modification.

### 4. Is your test software reusable?

- Yes, the test suite is modular and well-organized, allowing it to be extended with additional tests (such as concurrency or adversarial input tests). It can serve as a robust foundation for performance testing of similar data structures or databases.
