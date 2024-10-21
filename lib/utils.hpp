#include <iostream>
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


/* Creates an fvec file with the given vectors for testing */
void make_fvec(const string& filename, const vector<vector<float>>& vectors);

/* Creates an ivec file with the given vectors for testing */
void make_ivec(const string& filename, const vector<vector<int>>& vectors);



/* Given an empty graph, reads an fvec file and fills the graph with all the vectors read as its vertices and no edges */
/* This functions assumes that all the vectors given in the file will be of the same dimension                         */
/* Otherwise, the entries inside the graph will be uneven (vectors of different dimensions)                            */
void fvec_to_graph(const std::string& filename, Graph<vector<float>>& G);


/* Given an empty graph, reads an ivec file and returns a graph with all the vectors read as its vertices and no edges */
/* Everything mentioned for the method above holds true for this one too                                               */
void ivec_to_graph(const std::string& filename, Graph<vector<int>>& G);