Simple-Hybesis-Algorithms-Logger
Fork of the Simple-Hybesis-Algorithms repository adding logging capabilities used to describe algorithms steps.

# Simple-Hybesis-Algorithms
You can find a fork repository adding algorithm logging features (used for visualization) and its documentation here:
- https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms-Logger/
- http://michaeljeulin-l.com/Projects/SHAL/Doc/

# CMake - Cross-Building
The building relies on CMake, an Open-Source system that allows to easily build cross-platform applications by generating native makefiles (cf. http://www.cmake.org/cmake/resources/software.html).

# Getting Started
## Build using CMake-GUI:

    Run CMake-GUI
    Select source/target directory
    Click on Configure the project
    Click on Generate the Solution files
    Open the solution file (if MSVC) and build or use make command.

## Build in Command-Line (UNIX):

    mkdir Simple-Hybesis-Algorithms-Build (Create Build directory)
    cd Simple-Hybesis-Algorithms-Build    (Open build director)
    ccmake ../Simple-Hybesis-Algorithms   (Configure the make given Source repository)
    make                                  (Make command, use -j parameter to use multi-core)

# Doxygen Documentation & CMake
Use the CMake **'BUILD_DOC'** (default to false) option to automatically setup Documentation Generation with doxygen including SHA Core documentation.

Use the CMake **'BUILD_DOC_LOG'** (default to true) option to automatically setup the logger Documentation Generation with doxygen.

Please find the last generated documentations here:
- http://michaeljeulin-l.com/Projects/SHAC/Doc/
- http://michaeljeulin-l.com/Projects/SHAL/Doc/

# GTest - Unit Testing & CMake
Use the CMake **'BUILD_TESTING_LOG'** (default to true) option to automatically setup Unit Testing with google test on the Logger modules.

Use the CMake **'BUILD_TESTING'** (default to true) option to automatically setup Unit Testing with google test on the Core modules.

It allows to very quickly get set up with C++ and google test:

- Clone GTest via GIT
- Setup the Unit Testings
- Compile GTest at build time

Use the CMake **'WITH_COVERAGE'** (default to true) option to automatically setup Coverage Generation.
The minimal requiered coverage is **95%**.

Please find the last testing dashboard here:
http://my.cdash.org/index.php?project=Simple-Hybesis-Algorithm

## Running Unit Tests (UTs)
You can whether use CTest or manually run the unit tests.

E.g. to manually run the 'TestBasicBinary' Unit Test (using GTest):

    Simple-Hybesis-Algorithms-Build/Modules/Search/Testing/Debug/TestBinary.exe  (Win)
    ./Simple-Hybesis-Algorithms-Build/Modules/Search/Testing/Debug/TestBinary    (UNIX)

# Algorithms/Data Structures with Logs and Visualizations
#### Search
- **Binary Search:** Iteratively proceed a dichotomous search, within a sorted sequence, on the first occurrence of the key.

#### Sort
- **Bubble Sort:** Sometimes referred to as sinking sort: proceed an in-place bubble-sort on the elements.
- **Partition-Exchange:** Proceed an in-place partitioning on the elements.
- **Quick Sort - Partition-Exchange Sort:** Proceed an in-place quick-sort on the elements.

# Other Algorithms/Data Structures core implementations
#### Combinatory
- **Combinations:** Compute all possible combinations of elements containing within the sequence.
- **Intersection:** Compute the intersection of two sequences keeping duplicate keys distinct.
- **IsInterleaved:** Determine whether or not a sequence is the interleave of the two others.
- **Permutations:** Compute all possible permutations of elements containing within the sequence.

#### Data Structures
- **BinarySearchTree:** Binary Search Tree, Ordered Tree or Sorted Binary Tree divides all its sub-trees into two segments: left sub-tree and right sub-tree.

#### Search
- **Kth Smallest / Biggest element - Order Statitstics:** Find the kth smallest/biggest element.
- **Maximal/Minimal Distance:** Identify the two elements of the sequence that give the maximal/minimal distance.
- **Maximal/Minimal M Elements:** Retrieve the m maximal/minimal values sorted in respectively decreasing increasing order.
- **Maximal/Minimal Sub-Sequence:** Identify the sub-sequence with the maximum/minimum sum. One of the problem resolved by this algorithm is:
"Given an array of gains/losses over time, find the period that represents the best/worst cumulative gain."

#### Sort
- **MergeInplace:** Functor that proceeds a in place merge of two sequences of elements.
- **MergeSort:** John von Neumann in 1945: Proceed merge-sort on the elements whether using an in-place strategy or using a buffer.
- **MergeWithBuffer:** Functor that proceeds a merge of two sequences of elements using a buffer to improve time computation.
- **Raddix Sort - LSD:** Proceed the Least Significant Digit Raddix sort, a non-comparative integer sorting algorithm.
