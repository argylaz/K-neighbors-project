#include <bits/stdc++.h>
#include <cmath>
#include "graph.hpp"
using namespace std;


// Function for the calculation of the Euclidean distance
template<typename Type>             // Type is either integer or float
float Euclidean_Distance(vector<Type> a, vector<Type> b) {
    
    float sum = 0;

    // Iterators of the vectors
    auto i = a.begin();
    auto j = b.begin();
    
    while( i != a.end() || j != b.end() ){
        sum += pow(*j-*i,2);           // sum = (i1 -j1)^2 + ... + (i_n-j_n)^2  
        i++;
        j++;
    }
    float x = sqrt(sum);                        // [(i1 -j1)^2 + ... + (i_n-j_n)^2] ^ 1/2
    return x;

}



/* Function that given a set S and a point xquery, finds the point p in S with the min Euclidean distance with xquery*/
template <typename Type>
vector<Type> find_min_Euclidean(set<vector<Type>> S, vector<Type> xquery){
    
    auto i = S.begin();
    float min_distance = Euclidean_Distance<Type>(xquery, *i);
    //cout << min_distance << endl;
    vector<Type> min_point = *i;

    while( ++i != S.end()){
        if( Euclidean_Distance<Type>(xquery, *i) < min_distance ){
            min_distance = Euclidean_Distance<Type>(xquery, *i);
            min_point = *i;
        }
    }

    return min_point;
}


/* Given a set of vectors and a target vector (xquery), only keep the L vectors closest to xquery */
template <typename Type>
void retain_closest_points(set<vector<Type>> &output_set, vector<Type> xquery, int L){
    
    // Create vector with the elements of the set
    output_set.erase(xquery);
    vector<vector<Type>> output_vec(output_set.begin(), output_set.end());
    
    // Sort the vector comoaring the Euclidean Distance of each element with the xquery
    sort(output_vec.begin(), output_vec.end(), [&xquery](const vector<Type>& a, const vector<Type>& b) {
        return Euclidean_Distance<Type>(a, xquery) < Euclidean_Distance<Type>(b, xquery);
    });

    if( output_vec.size() >= (long unsigned int) L )
        output_vec.resize(L);
    else   
        cout << "L is greater than the size of the vector/set\n";
    
    output_set.clear();
    output_set.insert(output_vec.begin(), output_vec.end());
        

    // int count = 0;
    // for( vector<Type> v : output_set ){
    //     if( Euclidean_Distance<Type>(v, xquery) > L){
    //         output_set.erase(v);
    //         count++;
    //     }
    // }
}



/* Method which finds the medoid of a given graph */
template <typename Type>
vector<Type> medoid(Graph<vector<Type>>& G){
    
    float min = numeric_limits<float>::max();
    vector<Type> medoid_vertice; 

    // Check if the graph is empty
    if ( G.get_vertices_count() == 0  ) { 
        cerr << "\nThe Graph is empty. The Medoid can't be calculated\n" << endl;
        // Returns an empty vector
        return {};
    }


    for( vector<Type> vertice : G.get_vertices() ){

        // Calculate the sum of the Euclidean Distances of this vertice with all the others 
        float sum = 0;
        for( vector<Type> x : G.get_vertices() ){
            sum += Euclidean_Distance<Type>(vertice, x);
        }

        // Check if this sum is the minimum
        if ( sum < min ){
            min = sum;
            medoid_vertice = vertice;
        }

    }

    cout << "Medoid point ";  
    print_vector(medoid_vertice);
    cout << "\nFound at index " << G.get_index_from_vertex(medoid_vertice) << endl;

    return medoid_vertice;

}

/* Method which adds randomly exactly R outgoing neighbors to each vertex of the graph */
template <typename T>
void rDirectional(Graph<T>& G, int R) {
    
    if ( R > G.get_vertices_count() ) {
        cerr << "R-Directional Graph initialization failed...\n" << endl;
        return ;
    }

    set<T> vertices = G.get_vertices();

    // If the graph already has edges, we remove them and construct the graph from the beginning 
    if (G.get_edge_count() > 0) {
        cerr << "\nGraph already has edges\n" << endl;
        
        for (T vertex: vertices) {
            set<T> neighbors = G.get_neighbors(vertex);
            for (T neighbor: neighbors) {
                G.remove_edge(vertex, neighbor);
            }
        }
        
    }

    // For each vertex of the graph
    for ( T v : vertices ) {
        
        // Create a vector with all the elements of the graph
        vector<T> shuffled_vertices(vertices.begin(), vertices.end());

        // To obtain a time-based seed 
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        
        // Shuffle the vector 
        // We may need a random generator as an argument here (for better randomness)
        shuffle(shuffled_vertices.begin(), shuffled_vertices.end(), default_random_engine(seed));


        // Add R random outgoing neighbors
        int i = 0;
        int count = 0;
        while ( count < R ){
            if ( shuffled_vertices[i] != v ){
                G.add_edge(v, shuffled_vertices[i]);  
                count++;  
            }
            i++;
        }

    }

}

/*-------------------------------------Utility functions and classes for the tests-------------------------------------*/

/* Creating a testable graph that inherits from Graph to access protected attributes */
template<typename T>
class TestGraph: public Graph<T> {  // Graph<int> for testing
public:
    TestGraph(bool isDirected=true) : Graph<T>(isDirected) {}

    vector<vector<T>> get_private_adjacency_list() const {
        return this->get_adjacency_list();
    }

    set<T> get_private_vertex_set() const{
        return this->get_vertices();
    }
};


/* Given an empty graph, reads an fvec or an ivec file and fills the graph with all the vectors read as its vertices and no edges */
/* This functions assumes that all the vectors given in the file will be of the same dimension                                    */
/* Otherwise, the entries inside the graph will be uneven (vectors of different dimensions)                                       */
template <typename type>
void vec_to_graph(const string& filename, Graph<vector<type>>& G) {
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when opening file " << filename << endl;
        return;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while (file.peek() != EOF) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if (!file) {
            correctFormat = false; 
            break;
        }

        // Create vector to hold the values and resize to the correct dimension
        vector<type> v; v.resize(d);

        // Read the data from file
        file.read(reinterpret_cast<char*>(v.data()), d * sizeof(type));
        if (!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if (v.size() != (size_t) d) {
            correctFormat = false;
            break;
        }

        G.add_vertex(v);
    }

    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors." << endl;
    file.close();
}

template <typename Type>
vector<vector<Type>> read_vecs(string& filename) {
    
    vector<vector<Type>> result;
    
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when opening file " << filename << endl;
        return NULL;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while (file.peek() != EOF) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if (!file) {
            correctFormat = false; 
            break;
        }

        // Create vector to hold the values and resize to the correct dimension
        vector<Type> v; v.resize(d);

        // Read the data from file
        file.read(reinterpret_cast<char*>(v.data()), d * sizeof(Type));
        if (!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if (v.size() != (size_t) d) {
            correctFormat = false;
            break;
        }

        result.push_back(v);
    }

    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors." << endl;
    file.close();
}

/*--------------------------------------------Utility functions for the tests--------------------------------------------*/

/* Creates an fvec or ivec file with the given vectors for testing */
template <typename type>
void make_vec(const string& filename, const vector<vector<type>>& vectors) {
    // Create file and check it was created successfully
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when creating file " << filename << endl;
        return;
    }

    // Read vectors and wite them in the files
    for (const auto& v: vectors) {
        int d = v.size();         // Get the dimension of the vector

        // First write the dimension as an int
        file.write(reinterpret_cast<const char*>(&d), sizeof(int));

        // Then write the data as floats
        file.write(reinterpret_cast<const char*>(v.data()), d * sizeof(type));
    }

    // Close the files
    file.close();
}