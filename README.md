# Social Network Analysis

A C++ implementation of graph algorithms and dynamic programming techniques for analyzing large-scale social network datasets.

## Features

- Graph construction from edge-list datasets
- Dijkstra's shortest path algorithm
- A* search with heuristic optimization
- Performance comparison between Dijkstra and A*
- Dynamic programming solution for the longest increasing influence chain
- Large dataset processing
- Execution time analysis

## Algorithms

- Dijkstra's Algorithm
- A* Search
- Dynamic Programming

## Technologies

- C++
- Standard Template Library (STL)

## Dataset

The project uses three datasets:

```text
social-network-proj-graph.txt
social-network-proj-Influences.txt
social-network-proj-LABELS.txt
```

## Project Structure

```text
code.cpp                           Main implementation
social-network-proj-graph.txt       Graph dataset
social-network-proj-Influences.txt  Influence scores
social-network-proj-LABELS.txt      User labels
Project_Report.pdf                  Project documentation
```

## Build

```bash
g++ code.cpp -O2 -o social-network-analysis
```

## Run

```bash
./social-network-analysis
```

## Academic Project

Developed as part of the Design and Analysis of Algorithms course to demonstrate graph algorithms, heuristic search, dynamic programming, and algorithm performance evaluation.
