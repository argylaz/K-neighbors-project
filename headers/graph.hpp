/* Graph data structure representation using STL vector and set*/
#include <iostream>
#include <bits/stdc++.h>
#include <set>
using namespace std;


template <typename T>              // Template T to make the graph generic
class Graph {
public:
    /* Constructor */
    Graph(bool isDirected = true); // The graph is directed by default because of the nature of the project

   
    /* Adds a vertex (of type T) to the graph */
    void add_vertex(const T& v);

    /* Adds edge between vertex(T) start and vertex(T) end */
    void add_edge(const T& start, const T& end);

    /* Removes vertex v from the graph, not needed for the project*/
    // void remove_vertex(T& v);

    /* Removes edge between node start and node end from the graph */
    void remove_edge(const T& start, const T& end);

    /* Method to print the graph */
    void print_graph();

    /* Method which returns the vertices count*/
    int get_vertices_count();

    /* Method that returns the edge count */
    int get_edge_count();

    /* Returns true if the graph is directed */
    bool is_directed();


protected:   
    /* Getter methods for testing purposes */
    auto get_adjacency_list() const;
    set<T> get_vertices() const;
    

private:
    int countEdges;
    int countVertices;
    bool isDirected;                        // True if the graph is directed
    set<T> vertices;                            // A set of the graphs vertices
    vector<vector<T>> adjacencyList;        // Adjacency list (vector of vectors)
};
