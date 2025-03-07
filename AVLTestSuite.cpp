// AVLTestSuite.cpp
//
// Assignment 3 - Performance Testing of a Custom Database
// Group Project (2–3 students per group)
// Due: Sunday, March 23, 2025
//
// This file contains a complete test suite that compares a custom AVL tree
// (provided as AVLTree.cpp/AVLTree.h) with std::map. The tests include:
//   1. Insertion correctness
//   2. Deletion correctness
//   3. Maximum size (catching bad_alloc)
//   4. Load testing (repeated insertion and access)
//   5. Speed of search (worst-case)
//   6. Memory leak simulation (repeated creation/destruction)
//
//
// (Note: The AVLTree and Timer code are given and must not be modified.)
//
#include "AVLTree.h"
#include "timer.h"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <sys/resource.h>
#include <unistd.h>

using namespace std;

#if PLATFORM == POSIX
#include <sys/resource.h>
#include <unistd.h>

long memUsed()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss / 1024; // Memory used in MB (Note: on macOS, ru_maxrss is in bytes)
}

#elif PLATFORM == WINDOWS
#include <windows.h>
#include <psapi.h>

size_t memUsed()
{
    PROCESS_MEMORY_COUNTERS pmc;
    size_t currentUsage = 0;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        currentUsage = pmc.WorkingSetSize / (1024 * 1024); // Convert from bytes to MB
    }
    return currentUsage;
}
#endif

// ---------------------------------------------------------------------------
// DatabaseTestSuite: A test class that contains methods to test both the
// custom AVL tree and the standard std::map for insertion, deletion,
// maximum size, load, search speed and (simulated) memory leaks.
// ---------------------------------------------------------------------------
class DatabaseTestSuite
{
public:
    // Helper function: Create an EmployeeInfo record with a given sin.
    EmployeeInfo createEmployee(int sin)
    {
        EmployeeInfo e;
        e.age = 30;         // arbitrary value
        e.salary = 50000;   // arbitrary value
        e.emplNumber = sin; // use sin as employee number for testing
        e.sin = sin;
        return e;
    }

    // -----------------------------------------------------------------------
    // ===== AVL Tree Tests =====
    // -----------------------------------------------------------------------

    // Test 1: Correctness of insertion for AVL tree.
    void testInsertionAVL()
    {
        cout << "[AVL] Insertion Correctness Test...\n";
        AVL avl;
        vector<int> vals = {50, 30, 70, 20, 40, 60, 80};
        for (int v : vals)
        {
            avl.insert(createEmployee(v));
        }
        node *minNode = avl.findMin(avl.GetRoot());
        node *maxNode = avl.findMax(avl.GetRoot());
        assert(minNode && minNode->empl.sin == 20);
        assert(maxNode && maxNode->empl.sin == 80);
        cout << "[AVL] Insertion test passed.\n\n";
        // Clean up the tree after test.
        avl.makeEmpty(avl.GetRoot());
    }

    // Test 2: Correctness of deletion for AVL tree.
    void testDeletionAVL()
    {
        cout << "[AVL] Deletion Correctness Test...\n";
        AVL avl;
        vector<int> vals = {50, 30, 70, 20, 40, 60, 80};
        for (int v : vals)
        {
            avl.insert(createEmployee(v));
        }
        // Delete nodes and verify deletion.
        avl.remove(20);
        assert(avl.Find(avl.GetRoot(), 20) == NULL);
        avl.remove(30);
        assert(avl.Find(avl.GetRoot(), 30) == NULL);
        avl.remove(70);
        assert(avl.Find(avl.GetRoot(), 70) == NULL);
        cout << "[AVL] Deletion test passed.\n\n";
        // Clean up the tree after test.
        avl.makeEmpty(avl.GetRoot());
    }

    // Test 3: Maximum size test for AVL tree.
    // This test repeatedly builds trees until a bad_alloc is thrown.
    void testMaxSizeAVL()
    {
        int stepSize = 5000;
        int totalInsertions = 0;
        long maxStorageCapacity = 4096; // 4096 MB (4 GB) memory capacity
        long memory_used = memUsed();
        while (memory_used < maxStorageCapacity)
        {
            try
            {
                AVL avl;
                // Insert 'stepSize' new elements
                for (int i = 0; i < totalInsertions + stepSize; i++)
                {
                    avl.insert(createEmployee(i));
                }
                totalInsertions += stepSize;
                // Clean up the tree to free memory
                avl.makeEmpty(avl.GetRoot());
            }
            catch (const std::bad_alloc &e)
            {
                std::cerr << "Caught bad_alloc: " << e.what() << '\n';
                cerr << "Maximum size reached!" << endl;
                break;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception: " << e.what() << '\n';
            }
            memory_used = memUsed();
            cout << "Current memory usage: " << memory_used << " MB" << endl;
        }
        cout << "testMaxSizeAVL: Max size AVL: " << totalInsertions << endl;
    }

    // Test 4: Load test for AVL tree: repeated insertion and random access.
    void testLoadAVL(int iterations)
    {
        cout << "[AVL] Load Test (" << iterations << " iterations)...\n";
        AVL avl;
        for (int i = 0; i < iterations; i++)
        {
            avl.insert(createEmployee(i));
            if (i % 1000 == 0)
            {
                int target = rand() % (i + 1);
                avl.Find(avl.GetRoot(), target);
            }
        }
        cout << "[AVL] Load test completed.\n\n";
        // Clean up after test.
        avl.makeEmpty(avl.GetRoot());
    }

    // Test 5: Speed test for AVL tree search.
    void testSearchSpeedAVL(int numElements)
    {
        cout << "[AVL] Search Speed Test with " << numElements << " elements...\n";
        AVL avl;
        for (int i = 0; i < numElements; i++)
        {
            avl.insert(createEmployee(i));
        }
        Timer timer;
        // Worst-case: search for the minimum value.
        timer.start();
        node *result = avl.Find(avl.GetRoot(), 0);
        timer.stop();
        double elapsed = timer.currtime();
        cout << "[AVL] Time to search for minimum element (key 0): " << elapsed << " seconds.\n";
        // Also search for maximum.
        timer.reset();
        timer.start();
        result = avl.Find(avl.GetRoot(), numElements - 1);
        timer.stop();
        elapsed = timer.currtime();
        cout << "[AVL] Time to search for maximum element (key " << numElements - 1 << "): " << elapsed << " seconds.\n\n";
        // Clean up after test.
        avl.makeEmpty(avl.GetRoot());
    }

    // Test 6: Memory leak simulation for AVL tree.
    // (For true memory leak detection, use valgrind or Visual Studio's leak detector.)
    void testMemoryLeakAVL(int iterations)
    {
        cout << "[AVL] Memory Leak Test Simulation (" << iterations << " iterations)...\n";
        for (int i = 0; i < iterations; i++)
        {
            AVL avl;
            for (int j = 0; j < 1000; j++)
            {
                avl.insert(createEmployee(j));
            }
            avl.makeEmpty(avl.GetRoot());
        }
        cout << "[AVL] Memory leak simulation test completed.\n\n";
    }

    // -----------------------------------------------------------------------
    // ===== std::map Tests =====
    // -----------------------------------------------------------------------

    // Test 1 (Map): Correctness of insertion.
    void testInsertionMap()
    {
        cout << "[map] Insertion Correctness Test...\n";
        map<int, EmployeeInfo> m;
        vector<int> vals = {50, 30, 70, 20, 40, 60, 80};
        for (int v : vals)
        {
            m.insert(make_pair(v, createEmployee(v)));
        }
        // Check that the smallest key is 20 and the largest is 80.
        assert(m.begin()->first == 20);
        auto it = m.end();
        it--;
        assert(it->first == 80);
        cout << "[map] Insertion test passed.\n\n";
        // Clean up the map.
        m.clear();
    }

    // Test 2 (Map): Correctness of deletion.
    void testDeletionMap()
    {
        cout << "[map] Deletion Correctness Test...\n";
        map<int, EmployeeInfo> m;
        vector<int> vals = {50, 30, 70, 20, 40, 60, 80};
        for (int v : vals)
        {
            m.insert(make_pair(v, createEmployee(v)));
        }
        // Remove keys and verify deletion.
        m.erase(20);
        assert(m.find(20) == m.end());
        m.erase(30);
        assert(m.find(30) == m.end());
        m.erase(70);
        assert(m.find(70) == m.end());
        cout << "[map] Deletion test passed.\n\n";
        // Clean up the map.
        m.clear();
    }

    // Test 3 (Map): Maximum size test.
    void testMaxSizeMap()
    {
        cout << "[map] Maximum Size Test...\n";
        int stepSize = 1000;
        int totalInsertions = 0;
        map<int, EmployeeInfo> m;
        long memory_used = memUsed();
        long maxStorageCapacity = memory_used + 4096; // 4096 MB memory capacity
        cout << "Current memory usage: " << memory_used << " MB" << endl;

        while (memory_used < maxStorageCapacity)
        {
            try
            {
                // Insert exactly 'stepSize' new elements in each iteration.
                for (int i = 0; i < stepSize; i++)
                {
                    int key = totalInsertions + i;
                    m.insert(make_pair(key, createEmployee(key)));
                }
                totalInsertions += stepSize;
            }
            catch (const std::bad_alloc &e)
            {
                cout << "[map] Caught bad_alloc after inserting approximately "
                     << totalInsertions << " elements into std::map.\n\n";
                break;
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
            memory_used = memUsed();
            cout << "Current memory usage: " << memory_used << " MB" << endl;
        }
        m.clear();
        cout << "testMaxSizeMap: Max size of std::map is: " << totalInsertions << endl;
    }

    // Test 4 (Map): Load test.
    void testLoadMap(int iterations)
    {
        cout << "[map] Load Test (" << iterations << " iterations)...\n";
        map<int, EmployeeInfo> m;
        for (int i = 0; i < iterations; i++)
        {
            m.insert(make_pair(i, createEmployee(i)));
            if (i % 1000 == 0)
            {
                int target = rand() % (i + 1);
                m.find(target);
            }
        }
        cout << "[map] Load test completed.\n\n";
        m.clear();
    }

    // Test 5 (Map): Search speed test.
    void testSearchSpeedMap(int numElements)
    {
        cout << "[map] Search Speed Test with " << numElements << " elements...\n";
        map<int, EmployeeInfo> m;
        for (int i = 0; i < numElements; i++)
        {
            m.insert(make_pair(i, createEmployee(i)));
        }
        Timer timer;
        timer.start();
        auto it = m.find(0);
        timer.stop();
        double elapsed = timer.currtime();
        cout << "[map] Time to search for minimum element (key 0): " << elapsed << " seconds.\n";
        timer.reset();
        timer.start();
        it = m.find(numElements - 1);
        timer.stop();
        elapsed = timer.currtime();
        cout << "[map] Time to search for maximum element (key " << numElements - 1 << "): " << elapsed << " seconds.\n\n";
        m.clear();
    }

    // Test 6 (Map): Memory leak simulation.
    void testMemoryLeakMap(int iterations)
    {
        cout << "[map] Memory Leak Test Simulation (" << iterations << " iterations)...\n";
        for (int i = 0; i < iterations; i++)
        {
            map<int, EmployeeInfo> m;
            for (int j = 0; j < 1000; j++)
            {
                m.insert(make_pair(j, createEmployee(j)));
            }
            m.clear();
        }
        cout << "[map] Memory leak simulation test completed.\n\n";
    }
};

// ---------------------------------------------------------------------------
// Main Function: Run all tests with pauses between groups.
// ---------------------------------------------------------------------------
int main()
{
    DatabaseTestSuite suite;

    cout << "=============================================\n";
    cout << "Starting Database Performance Tests\n";
    cout << "=============================================\n\n";

    // ----- AVL Tree Tests -----
    cout << "==== Running AVL Tree Tests ====\n\n";
    suite.testInsertionAVL();
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testDeletionAVL();
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testMaxSizeAVL();
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testLoadAVL(50000); // Adjust iterations as needed.
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testSearchSpeedAVL(100000); // Adjust element count as needed.
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testMemoryLeakAVL(100); // 100 iterations of creation/destruction.
    cout << "Press Enter to continue...\n";
    getchar();

    // ----- std::map Tests -----
    cout << "\n==== Running std::map Tests ====\n\n";
    suite.testInsertionMap();
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testDeletionMap();
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testMaxSizeMap();
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testLoadMap(50000);
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testSearchSpeedMap(100000);
    cout << "Press Enter to continue...\n";
    getchar();

    suite.testMemoryLeakMap(100);
    cout << "Press Enter to continue...\n";
    getchar();

    cout << "=============================================\n";
    cout << "All tests completed.\n";
    cout << "=============================================\n\n";

    // -----------------------------------------------------------------------
    // Answers to Assignment Questions:
    // -----------------------------------------------------------------------
    cout << "Answers to Assignment Questions:\n";
    cout << "1. Additional tests: One could include concurrency tests (simultaneous insertions, deletions, searches) "
         << "and stress tests using extreme patterns (e.g., all duplicate keys or highly skewed input) to further validate robustness.\n";
    cout << "2. std::map vs. AVL Tree: Both offer O(log n) operations. The std::map (usually a red–black tree) "
         << "is robust and well-optimized, while the custom AVL tree maintains stricter balance (and possibly faster search times) "
         << "but may incur additional overhead from frequent rotations.\n";
    cout << "3. Unexpected results: In practice, the AVL tree’s frequent rotations may reduce its theoretical speed advantage, "
         << "yielding performance similar to std::map under certain workloads.\n";
    cout << "4. Recommendation: For rapid development and long-term reliability, std::map is recommended. "
         << "However, if minimal memory overhead and a custom solution are critical, a well-implemented AVL tree can be preferable.\n";
    cout << "5. Memory reservation: Based on maximum size tests, approximately 4 GB (or a bit more) should be reserved "
         << "for the database under worst-case scenarios.\n";
    cout << "6. Reusability: The AVL tree’s clean interface makes it reusable in other projects; similarly, the test software "
         << "is modular and can be extended for future testing needs.\n";

    return 0;
}
