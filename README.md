<H1> COP3530 - Project 2 </h1>
Analyzing the effectivness of MergeSort and QuickSort on very large datasets

<h2> Overview </h2>
This project uses a dataset of USA Real Estate Dataset which contains listings (2.2M+) in the US broken down by State and zip code
More detailed on the dataset can be found at https://www.kaggle.com/datasets/ahmedshahriarsakib/usa-real-estate-dataset

## Features
* **Custom Data Parsing:** Efficiently parses a massive CSV dataset into dynamically allocated memory, handling corrupted entries with error-checking.
* **From-Scratch Algorithms:** Implements both an $O(N \log N)$ recursive **Merge Sort** and an iterative **Quick Sort**  entirely in raw C++ without standard library sorting functions.
* **Stack Overflow Prevention:** Utilizes a custom `std::stack` implementation for Quick Sort to prevent call stack crashes during extreme recursion depths.
* **Performance Benchmarking:** Uses C++ `<chrono>` to precisely measure CPU execution time in milliseconds.
* **Interactive CLI:** Allows the user to toggle between randomized data, worst-case pre-sorted data, and different sorting by attributes (Price, Beds, Baths, House Size).

## Requirements
* C++17
* CMake >= 3.20
