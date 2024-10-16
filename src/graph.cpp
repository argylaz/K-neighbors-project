#include "../headers/graph.hpp"

template <typename T>
Graph<T>::Graph(bool isDirected) {
    
    // Initialize counters to zero
    countEdges = 0;
    countVertices = 0;
    this->isDirected = isDirected; 
    cout << this->isDirected << endl;
    
    for( vector<T> v : adjacencyList){
        for( T i : v ){
            cout << i << endl;
        };
    };


}

/* Add a vertex (of type T) to the graph */
template <typename T>
void Graph<T>::add_vertex(const T& v) {
    
    vertices.insert(v);
    
    // Print vertices inside the set, for testing purposes 
    // for( auto ver = vertices.begin() ; ver!=vertices.end() ; ver++){
    //     cout << *ver << endl;
    // }

    /* Insert the vertex with its index to the map
       Index starts from 0 */
    v_index.insert({v,countVertices});

    /* When vertex is added, resize the Adjacency List
       The first element of each row of the adjacency list is the vertex in question */
    adjacencyList.resize(vertices.size());
    adjacencyList[v_index[v]].push_back(v);

    /* Increase the counter and update the map*/
    countVertices++;

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



    /* If the graph is Undirected, remove also the edge end->start */
    if( isDirected == false ){
        v_pos = v_index[end];
        int k = 0;
        for( auto j = adjacencyList[v_pos].begin() ; j < adjacencyList[v_pos].end() ; j++){
            if( start == adjacencyList[v_pos][k] )
                adjacencyList[v_pos].erase(j);
            k++;
        }
    }
}

/* Method to print the graph */
template <typename T>
void Graph<T>::print_graph(void) {


    for( auto ver = vertices.begin() ; ver!=vertices.end() ; ver++ ){
        cout << "Vertex " << *ver << ":";
        int v_pos = v_index[*ver];
        /* The first element of each row is the is the vertex itself */
        for( size_t j = 1 ; j < adjacencyList[v_pos].size() ; j++){
            cout << " -> " << adjacencyList[v_pos][j];
        } 
        cout << endl;
    }

    cout << endl;

}

/* Method which returns the vertices count*/
template <typename T>
const int Graph<T>::get_vertices_count(void) {
    return vertices.size();
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

// /* Get adjacency list method for testing purposes */
template <typename T>
vector<vector<T>> Graph<T>::get_adjacency_list(void) const {
    return adjacencyList;
}
     




/* int main() {
    // cout << "edges " << g.get_edge_count() << endl;
    // cout << "vertices " << g.get_vertices_count() << endl;

    // cout << "edges " << g.get_edge_count() << endl;
    // cout << "vertices " << g.get_vertices_count() << endl;

    // cout << endl;

    // Testing Directed graph
    Graph<int> g1 = Graph<int>();
    g1.add_vertex(4);
    g1.add_vertex(90);
    g1.add_vertex(76);

    g1.add_edge(4,90);
    g1.add_edge(90,76);
    g1.add_edge(76,4);

    g1.print_graph();

    // Testing Undirected graph
    Graph<int> g2 = Graph<int>(false);
    g2.add_vertex(10);
    g2.add_vertex(20);
    g2.add_vertex(30);
    g2.add_vertex(40);

    g2.add_edge(10,20);
    g2.add_edge(10,30);
    g2.add_edge(20,40);
    g2.add_edge(30,40);

    g2.print_graph();

    // Testing Directed graph with characters
    Graph<char> g3 = Graph<char>();
    g3.add_vertex('c');
    g3.add_vertex('s');
    g3.add_vertex('x');
    g3.add_vertex('e');

    g3.add_edge('c','s');
    g3.add_edge('c','x');
    g3.add_edge('c','e');
    g3.add_edge('e','x');
    g3.print_graph();

    g3.remove_edge('c','s');
    g3.remove_edge('c','x');
    g3.remove_edge('c','e');
    g3.remove_edge('e','x');
    g3.print_graph();

    // Testing removals on an Undirected graph
    Graph<char> g4 = Graph<char>(false);
    g4.add_vertex('a');
    g4.add_vertex('b');
    g4.add_vertex('c');
    g4.add_vertex('d');

    g4.add_edge('a','b');
    g4.add_edge('a','c');
    g4.add_edge('a','d');
    g4.add_edge('b','c');
    g4.add_edge('b','d');
    g4.add_edge('c','d');
    g4.print_graph();

    g4.remove_edge('a','b');
    g4.remove_edge('a','c');
    g4.remove_edge('c','d');
    g4.print_graph();

    

} */
