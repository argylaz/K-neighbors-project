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
void vec_to_graph(const std::string& filename, Graph<vector<type>>& G) {
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when opening file " << filename << endl;
        return;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while(file.peek() != EOF) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if(!file) {
            correctFormat = false; 
            break;
        }

        // Create vector to hold the values and resize to the correct dimension
        vector<type> v; v.resize(d);

        // Read the data from file
        file.read(reinterpret_cast<char*>(v.data()), d * sizeof(type));
        if(!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if(v.size() != (size_t) d) {
            correctFormat = false;
            break;
        }

        G.add_vertex(v);
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