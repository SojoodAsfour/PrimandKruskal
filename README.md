# 🇵🇸 Palestinian Cities Road Network - MST Project (COMP2421 Project #3)

## 📌 Overview

This project constructs a **Minimum Spanning Tree (MST)** to connect a set of Palestinian cities and towns with the **minimal road construction cost**. Two well-known algorithms are implemented and compared:

- **Prim's Algorithm**
- **Kruskal's Algorithm**

The goal is to determine the most cost-effective way to build a road network while measuring each algorithm’s execution time.

---

## 🗂️ Features

- Read city connections and distances from a file (`cities.txt`)
- Dynamically build a graph representing the cities and their connections
- Implement both **Prim’s** and **Kruskal’s** MST algorithms
- Display the total cost and execution time of each algorithm
- Compare the results of both algorithms
- Menu-driven CLI for ease of use

---

## 🧪 Sample Input Format (`cities.txt`)
Jerusalem#Gaza#41
Akka#Haifa#35
Jenin#Qalqilya#23
Gaza#Hebron#32
Bethlehem#Jerusalem#9
...

Each line defines a road between two cities and its distance (in kilometers).

---

## 🖥️ Menu Options

Upon running the program, the user can choose from the following options:

1. **Load cities**  
   → Load the data from `cities.txt`, construct the graph, and display graph size and order.

2. **Apply Prim’s Algorithm**  
   → Prompt for a source city and display the MST, its total cost, and execution time.

3. **Apply Kruskal’s Algorithm**  
   → Compute and display the MST using Kruskal’s algorithm.

4. **Compare the Two Algorithms**  
   → Show a side-by-side comparison of total costs and times for both algorithms.

5. **Exit**

---

## ⏱️ Execution Time

The program uses the `clock()` function from `<time.h>` to calculate the CPU time taken by each algorithm.

Reference: [How to measure time in C](https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/)

---
