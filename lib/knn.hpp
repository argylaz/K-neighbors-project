/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
#include "utils.hpp";
using namespace std;

typedef int index;

/* Traverses the graph and finds k-approx NNs */
// s is the index of the start node s 
// xq is the index of the query 
// k is the number of approximate NNs calculated 
// L is the search list size 
// Returns a pair of sets, the first contains the K-approx NNs and the second contains all the visited nodes
template <typename T>
pair<set<T>, set<T>>& GreedySearch(Graph<T>& G, index start, index xquery, int k, int L_input) {
    // First we check that the input values are correct
    if (L < k) { // L>=k
        cerr << "Search list size L must be greater than or equal to k!" << endl;
        return None;
    }

    // Initialize set L_output = {s} and V = { }
    set<T> L_output, V;
    L_output.insert(G.get_vertex_from_index(s));
    
    // L_output\V
    set<T> diff_set = set_difference(L_output.begin(), L_output.end(), V.begin(), V.end())
    
    // while L_output\V != 0
    while( !diff_set.empty() ){
        p = 
    }


}

/* Function that given a set S and a point xquery, finds the point p in S with the min Euclidean distance with xquery*/
template <typename T>
T find_min_Euclidean(set<T> S, T xquery){
    
    auto i = S.begin();
    float min_distance = Euclidean_Distance(xquery, S[i]);
    T min_point = S[i];
    while( i != S.end() ){
        i++;
        if( min_distance > Euclidean_Distance(xquery, S[i]) ){
            min_distance = Euclidean_Distance(xquery, S[i]);
            min_point = S[i];
        }
    }
}

/* Prunes the graph to make it more fit for the GreedySearch algorithm */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degreee bound
template <typename T>
void RobustPrune(Graph<T>& G, index p, set<T>& V, float a, int R) {
    
}

/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
void vamana(Graph<T>& G, int L, int R) {
    
}