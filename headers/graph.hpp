/* Graph data structure representation using STL vector */
#include <iostream>
#include <bits/stdc++.h>
using namespace std;


template <typename T>              // Template T to make the graph generic
class Graph {
public:
    /* Constructor */
    Graph(bool isDirected = true); // The graph is directed by default because of the nature of the project

    /* Add a vertex (of type T) to the graph */
    void add_vertex(const T& v);

    /* Add edge between vertex(T) start and vertex(T) end */
    void add_edge(const T& start, const T& end);

    /* Method to print the graph */
    void print_graph(void);

    int get_vertices_count();
    

private:
    bool isDirected;                                // True if the graph is directed
    int countVertices;                              // Number of vertices
    std::vector<std::vector<int>> adjacencyList;    // Adjacency list (vector of vectors)


};
