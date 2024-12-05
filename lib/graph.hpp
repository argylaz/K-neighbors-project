/* Graph data structure representation using STL vector and set*/
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

/*------------------------------------- CLASS DECLARATION ----------------------------------------------------*/

typedef int gIndex;

template <typename T>              // Template T to make the graph generic
class Graph {
public:
    /* Constructor */
    Graph(bool isDirected = true); // The graph is directed by default because of the nature of the project

   
    /* Adds a vertex (of type T) to the graph, return false if the vertex is already in the graph*/
    virtual bool add_vertex(const T& v);

    /* Adds a key in a sorted vector, in a sorted way based on the vertex index key */
    void insert_sorted(vector<gIndex>& v, T key);
    /* Adds a key in an adjacency list, in a sorted way based on the vertex index key */
    void insert_sorted_in_adj_list(vector<T>& v, T key);

    /* Adds edge between vertex(T) start and vertex(T) end */
    bool add_edge(const T& start, const T& end);

    /* Removes edge between node start and node end from the graph */
    bool remove_edge(const T& start, const T& end);

    /* Method to print the graph (mainly for debugging purposes)*/
    void print_graph();

    /* Method to get vertex from index */
    inline const T get_vertex_from_index(gIndex i);

    /* Method to get index from vertex */
    inline gIndex get_index_from_vertex(const T& v);

    /* Method which returns the vertices count*/
    inline const int get_vertices_count();

    /* Method that returns the edge count */
    inline const int get_edge_count();

    /* Returns true if the graph is directed */
    inline const bool is_directed();

    /* Returns a set containing all the current neighbours of the given vertex */
    const vector<gIndex> get_neighbors(T vertex);

    /* Method that returns if exists an edge from vertex_a to vertex_b*/
    inline bool exist_edge(T vertex_a, T vertex_b);

    /* Getter method for the set of vertices*/
    inline set<T> get_vertices() const;

    /* Virtual destructor doing nothing*/
    virtual ~Graph();

protected:
    vector<vector<T>> get_adjacency_list() const;

    int countEdges;
    int countVertices;
    bool isDirected;                        // True if the graph is directed

    
    set<T> vertices;                        // A set of the graphs vertices
    map<T, gIndex> v_index;                 // Mapping from T to indices

    /* Adjacency list (vector of vectors)                      */
    /* First element of each vector will be the vertex corresponding to that index
       in order to avoid a second mapping from index to vertex */
    vector<vector<T>> adjacencyList;
};


/*------------------------------ FUNCTION DEFINITIONS (in .hpp because of the templates) -----------------------------*/


/* Default constructor */
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

    /* Increase counter and then insert the vertex with its index to the map 
       because indexing starts from 0 */
    v_index.insert({v,countVertices});
    countVertices++;                   

    /* When vertex is added, resize the Adjacency List
       The first element of each row of the adjacency list is the vertex in question */
    adjacencyList.resize(countVertices);
    adjacencyList[v_index[v]].push_back(v);

    return true;
}



/* Adds a key in a sorted vector, in a sorted way based on the vertex index key */
template <typename T>
void Graph<T>::insert_sorted(vector<gIndex>& v, T key) {
    // First appending the element in the vector, so that it can grow in size if needed
    v.push_back(v_index[key]);
    
    // Looking for the right place in the vector
    int insertPos = 0;
    for (auto i = v.begin(); i < v.end() - 1; i++) {
        if (v_index[key] >= *i) {
            insertPos++;
        } else {
            break;
        }
    }
    copy_backward(v.begin() + insertPos, v.end() - 1, v.end());

    v[insertPos] = v_index[key];
}

/* Adds a key in an adjacency list, in a sorted way based on the vertex index key. Similar to the above but ignoring the first element of the vector */
template <typename T>
void Graph<T>::insert_sorted_in_adj_list(vector<T>& adj, T key) {
    adj.push_back(key);
    
    // Looking after the first element
    int insertPos = 1;
    for (auto i = adj.begin() + 1; i < adj.end() - 1; i++) {
        if (v_index[key] >= v_index[*i]) {
            insertPos++;
        } else {
            break;
        }
    }
    if (adj.begin() + insertPos < adj.end() - 1) {
        copy_backward(adj.begin() + insertPos, adj.end() - 1, adj.end());
        adj[insertPos] = key;
    }
}

/* Add edge between vertex(T) start and vertex(T) end, returns true if the edge added successfully*/
template <typename T>
bool Graph<T>::add_edge(const T& start, const T& end) {

    // Check if the given vertices are vertices of the graph, if they aren't return false
    if ( v_index.find(start) == v_index.end() || v_index.find(end) == v_index.end() ) {
        return false;
    }

    // If edge already exists, we just return true
    if (exist_edge(start, end))
        return true;

    // Get the index of vertex start and add entry (vertex end) to adjacency list
    int pos_start = v_index[start];
    insert_sorted_in_adj_list(adjacencyList[pos_start], end);

    countEdges++;

    /* If the graph is undirected, add also the end->start edge*/
    if ( isDirected == false ) {
        auto pos_end = v_index[end];
        
        insert_sorted_in_adj_list(adjacencyList[pos_end], start);
        countEdges++;
    }
    
    return true;
}



/* Removes edge between node start and node end from the graph, returns true if the edge removed successfully */
template <typename T>
bool Graph<T>::remove_edge(const T& start, const T& end) {

    // Check if the given vertices are vertices of the graph, if they aren't return false
    if( v_index.find(start) == v_index.end() || v_index.find(end) == v_index.end() ){
        return false;
    }

    int v_pos = v_index[start];

    // Find and remove the entry (vertex end) from adjacency list
    int i = 0;
    for( auto j = adjacencyList[v_pos].begin() ; j < adjacencyList[v_pos].end() ; j++){
        if( end == adjacencyList[v_pos][i] )
            adjacencyList[v_pos].erase(j);
        i++;
    }

    // Reducecounter
    countEdges--;

    /* If the graph is Undirected, also remove the edge end->start */
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

    return true;
}

/* Method to for printing vectors (mainly used for debugging) */
template <typename type>
void print_vector(vector<type> vec) {
    int size = vec.size();
    cout << "{";
    for( int i = 0 ; i < size ; i++){
        cout << vec[i];
        if( i != size - 1 &&  size != 1 )
            cout << ",";
    }
    cout << "}";
}


/* Method to print the graph */
template <typename T>
void Graph<T>::print_graph(void) {

    for( auto ver = vertices.begin() ; ver!=vertices.end() ; ver++ ){
        cout << "Vertex ";
        print_vector(*ver);
        cout << ": ";
        int v_pos = v_index[*ver];
        /* The first element of each row is the is the vertex itself */
        for( size_t j = 1 ; j < adjacencyList[v_pos].size() ; j++){
            // cout << " -> " << adjacencyList[v_pos][j];
            if( j != 1)
                cout << " -> ";
            print_vector(adjacencyList[v_pos][j]);
        } 

        cout << endl;
    }

    cout << endl;
}


template <typename T>
inline const T Graph<T>::get_vertex_from_index(int i) {
    // Returns the first element of the i-th row of the adjacency list, which is the vetrex corresponding to the row
    return this->adjacencyList[i][0];          
}


template <typename T>
inline gIndex Graph<T>::get_index_from_vertex(const T& v) {
    return this->v_index[v];  // Just returning the mapping of v
}


template <typename T>
inline const int Graph<T>::get_vertices_count(void) {
    // Just returns the private attribute
    return countVertices;
}


template <typename T>
inline const int Graph<T>::get_edge_count(){
    // Just returns the private attribute
    return countEdges;
}

/* Returns true if the graph is directed */
template <typename T>
inline const bool Graph<T>::is_directed(){
    return isDirected;
}

/* Getting the neigbors of a given vertex */
template <typename T>
const vector<gIndex> Graph<T>::get_neighbors(T vertex) {
 
    // Getting the elements in the corresponding row of the adjacency list without the vertex itself
    vector<gIndex> neighbors;
    neighbors.reserve(adjacencyList[v_index[vertex]].size() - 1);

    // Inserting neighbors into vector in the order they exist within the adjacency list.
    for (auto ver = adjacencyList[v_index[vertex]].begin() + 1; ver < adjacencyList[v_index[vertex]].end(); ver++) {
        neighbors.push_back(v_index[*ver]);
    }

    return neighbors;
}


/* Get adjacency list method for testing purposes */
template <typename T>
vector<vector<T>> Graph<T>::get_adjacency_list(void) const {
    // Just returns the private attribute
    return adjacencyList;
}


/* Method that returns true if an edge from vertex_a to vertex_b exists*/
template <typename T>
inline bool Graph<T>::exist_edge(T vertex_a, T vertex_b) {
    // First we check whether the nodes themselves exist in the graph
    if ( vertices.find(vertex_a) == vertices.end() || vertices.find(vertex_b) == vertices.end() )
        return false;
        
    // Then we check whether the entry for the edge exists in the adjacency list
    if ( find(adjacencyList[v_index[vertex_a]].begin(), adjacencyList[v_index[vertex_a]].end(), vertex_b) == adjacencyList[v_index[vertex_a]].end() )
        return false;

    return true;
}

template <typename T>
inline set<T> Graph<T>::get_vertices() const {
    // Just returns the private attribute
    return vertices;
}

template <typename T>
Graph<T>::~Graph() {}

