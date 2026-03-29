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

# Build Instructions
## Windows
```bash
# 1. Create a build folder and move into it
mkdir -p build && cd build

# 2. Tell CMake to read CMakeLists.txt blueprint
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. Compile the code 
cmake --build . --target Project2 -j

# 4. Move back to the main folder and run the program
cd ..
./build/Project2
```
# Frontend Setup
* Node.js
* npm
# From the my-app folder, 
```bash
npm install
npm install recharts
npm run dev
Local server will open at http://localhost:5173
After running CLI commands and exiting the program, move benchmark.json from cmake-build-debug to my-app/public and restart server if needed
```
