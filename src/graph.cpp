#include "../headers/graph.hpp"

template <typename T>
Graph<T>::Graph(bool isDirected) {
    
    countVertices = 0;
    
    for( vector<T> v : adjacencyList){
        for( T i : v ){
            cout << i << endl;
        };
    };

}

/* Add a vertex (of type T) to the graph */
template <typename T>
void Graph<T>::add_vertex(const T& v) {

    
}

/* Add edge between vertex(T) start and vertex(T) end */
template <typename T>
void Graph<T>::add_edge(const T& start, const T& end) {

}

/* Method to print the graph */
template <typename T>
void Graph<T>::print_graph(void) {

}

/* Method which returns the vertices count*/
template <typename T>
int Graph<T>::get_vertices_count(void) {
    return 0;
}

/* Get adjacency list method for testing purposes */
template <typename T>
auto Graph<T>::get_adjacency_list(void) const {

}
     




int main() {
    Graph<int> g = Graph<int>();
}
