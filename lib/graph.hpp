/* Graph data structure representation using STL vector and set*/
#include <iostream>
#include <bits/stdc++.h>
#include <set>
using namespace std;

/*------------------------------------- CLASS DECLARATION ----------------------------------------------------*/

typedef int gIndex;

template <typename T>              // Template T to make the graph generic
class Graph {
public:
    /* Constructor */
    Graph(bool isDirected = true); // The graph is directed by default because of the nature of the project

   
    /* Adds a vertex (of type T) to the graph, return false if the vertex is already in the graph*/
    bool add_vertex(const T& v);

    /* Adds edge between vertex(T) start and vertex(T) end */
    void add_edge(const T& start, const T& end);

    /* Removes vertex v from the graph, not needed for the project*/
    // void remove_vertex(T& v);

    /* Removes edge between node start and node end from the graph */
    void remove_edge(const T& start, const T& end);

    /* Method to print the graph */
    // void print_graph();

    /* Method to get vertex from index */
    const T get_vertex_from_index(gIndex i);

    /* Method to get index from vertex **/
    gIndex get_index_from_vertex(const T& v);

    /* Method which returns the vertices count*/
    const int get_vertices_count();

    /* Method that returns the edge count */
    const int get_edge_count();

    /* Returns true if the graph is directed */
    const bool is_directed();

    /* Returns a set containing all the current neighbours of the given vertex */
    const set<T> get_neighbors(T vertex);


protected:   
    /* Getter methods for testing purposes */
    vector<vector<T>> get_adjacency_list() const;
    set<T> get_vertices() const;
    

private:
    int countEdges;
    int countVertices;
    bool isDirected;                        // True if the graph is directed

    
    set<T> vertices;                        // A set of the graphs vertices
    map<T, gIndex> v_index;                    // Mapping from T to indices

    /* Adjacency list (vector of vectors)                                                                             */
    /* First element of each vector will be the vertex itself in order to avoid a second mapping from index to vertex */
    vector<vector<T>> adjacencyList;
};


/*------------------------------ FUNCTION DEFINITIONS (in .hpp because of the templates) -----------------------------*/

template <typename T>
Graph<T>::Graph(bool isDirected) {
    
    // Initialize counters to zero
    countEdges = 0;
    countVertices = 0;
    this->isDirected = isDirected; 
}

/* Add a vertex (of type T) to the graph */
template <typename T>
bool Graph<T>::add_vertex(const T& v) {

    // Try inserting and if v was a duplicate and was discarded return false
    auto successful_insert = vertices.insert(v);
    if( !successful_insert.second ){
        return false;
    }

    // Print vertices inside the set, for testing purposes 
    // for( auto ver = vertices.begin() ; ver!=vertices.end() ; ver++){
    //     cout << *ver << endl;
    // }

    /* Insert the vertex with its index to the map
       Index starts from 0 */
    v_index.insert({v,countVertices});

    /* Increase the counter and update the map*/
    countVertices++;
    /* When vertex is added, resize the Adjacency List
       The first element of each row of the adjacency list is the vertex in question */
    adjacencyList.resize(countVertices);
    adjacencyList[v_index[v]].push_back(v);

    return true;


    // cout << "Adjacency List Size is " << adjacencyList.size() << endl ;
    // cout << "Vertices Size is " << vertices.size() << endl ;
    // cout << "v-index map Size is " << v_index.size() << endl;
    // cout << "v-index position of the vertex is " << v_index[v] << endl;
    // cout << "I just pushed vertex " << adjacencyList[v_index[v]][0] << " in position " << v_index[v] << " of the adjList" << endl;
    
}

/* Add edge between vertex(T) start and vertex(T) end */
template <typename T>
void Graph<T>::add_edge(const T& start, const T& end) {

    // Printing positions for testing 
    // cout << "Vertex " << start <<  " found at position " << v_index[start] << endl ;
    // cout << "Vertex " << end <<  " found at position " << v_index[end] << endl ;

    int pos_start = v_index[start];

    adjacencyList[pos_start].push_back(end);
    countEdges++;

    /* If the graph is directed, add also the end->start edge*/
    if( isDirected == false ){
        auto pos_end = v_index[end];
        adjacencyList[pos_end].push_back(start);
        countEdges++;
    }
    
    // cout << "Edge added: " << start << " -> " << end << endl;

}


/* Removes edge between node start and node end from the graph */
template <typename T>
void Graph<T>::remove_edge(const T& start, const T& end) {

    int v_pos = v_index[start];

    int i = 0;
    for( auto j = adjacencyList[v_pos].begin() ; j < adjacencyList[v_pos].end() ; j++){
        if( end == adjacencyList[v_pos][i] )
            adjacencyList[v_pos].erase(j);
        i++;
    }

    countEdges--;


    /* If the graph is Undirected, remove also the edge end->start */
    if( isDirected == false ){
        v_pos = v_index[end];
        int k = 0;
        for( auto j = adjacencyList[v_pos].begin() ; j < adjacencyList[v_pos].end() ; j++){
            if( start == adjacencyList[v_pos][k] )
                adjacencyList[v_pos].erase(j);
            k++;
        }
        countEdges--;
    }
}

/* Method to print the graph */
// template <typename T>
// void Graph<T>::print_graph(void) {


//     for( auto ver = vertices.begin() ; ver!=vertices.end() ; ver++ ){
//         // cout << "Vertex " << *ver << ":";
//         int v_pos = v_index[*ver];
//         /* The first element of each row is the is the vertex itself */
//         for( size_t j = 1 ; j < adjacencyList[v_pos].size() ; j++){
//             // cout << " -> " << adjacencyList[v_pos][j];
//         } 
//         cout << endl;
//     }

//     cout << endl;

// }


template <typename T>
const T Graph<T>::get_vertex_from_index(int i) {
    return this->adjacencyList[i][0];                   // Returns the first element of the i-th row of the adjacency list, which is the vetrex corresponding to the row
}


template <typename T>
gIndex Graph<T>::get_index_from_vertex(const T& v) {
    return this->v_index[v];  // Just returning the mapping of v
}

/* Method which returns the vertices count*/
template <typename T>
const int Graph<T>::get_vertices_count(void) {
    return countVertices;
}

/* Method that returns the edge count */
template <typename T>
const int Graph<T>::get_edge_count(){
    return countEdges;
}

/* Returns true if the graph is directed */
template <typename T>
const bool Graph<T>::is_directed(){
    return isDirected;
}


template <typename T>
set<T> Graph<T>::get_vertices() const{
    return vertices;
}

/* Get adjacency list method for testing purposes */
template <typename T>
vector<vector<T>> Graph<T>::get_adjacency_list(void) const {
    return adjacencyList;
}

/* Getting the neigbors of a given vertex */
template <typename T>
const set<T> Graph<T>::get_neighbors(T vertex) {
 
    // getting the elements in the corresponding row of the adjacency list, i.e the vertex itself and its neighbors
    vector<T> adj = adjacencyList[v_index[vertex]];

    set<T> neighbors(adj.begin(), adj.end());

    // since adj includes the vertex itself, we remove it from the set
    neighbors.erase(vertex);

    return neighbors;

}
