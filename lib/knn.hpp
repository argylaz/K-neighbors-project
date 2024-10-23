/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
#include "utils.hpp"
using namespace std;

/* Traverses the graph and finds k-approx NNs */
// s is the index of the start node s 
// xq is the index of the query 
// k is the number of approximate NNs calculated 
// L is the search list size 
// Returns a pair of sets, the first contains the K-approx NNs and the second contains all the visited nodes
// template <typename T>
// pair<set<T>, set<T>>& GreedySearch(Graph<T>& G, T start, T xquery, int k, int L_input) {
//     // First we check that the input values are correct
//     if (L_input < k) { // L>=k
//         cerr << "Search list size L must be greater than or equal to k!" << endl;
//         return None;
//     }

//     // Initialize set L_output = {s} and V = { }
//     set<T> L_output, V;
//     L_output.insert(start);
    
//     // L_output\V
//     set<T> diff_set;
//     set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));
//     p = diff_set.begin();

//     // while L_output\V != 0
//     while( !diff_set.empty() ){

//         // Find the 
//         T min = find_min_Euclidean(diff_set, xquery)
//         // gIndex p_min_index = G.get_index_from_vertex(min); 
//         set<T> neighbors = G.get_neighbors(min);
//         set_union(L_output.begin(), L_output.end(), neighbors.begin(), neighbors.end(), inserter(L_output, L_output.end()));
//         V.insert(min);       
//         p++;

//         if( )

//     }


// }

/* Function that given a set S and a point xquery, finds the point p in S with the min Euclidean distance with xquery*/
template <typename Type>
vector<Type> find_min_Euclidean(set<vector<Type>> S, vector<Type> xquery){
    
    auto i = S.begin();
    float min_distance = Euclidean_Distance<Type>(xquery, *i);
    //cout << min_distance << endl;
    vector<Type> min_point = *i;

    while( ++i != S.end()){
        if( Euclidean_Distance<Type>(xquery, *i) < min_distance  ){
            min_distance = Euclidean_Distance<Type>(xquery, *i);
            min_point = *i;
        }
    }

    // cout << min_distance << " " << min_point[0] << " " << min_point[1] << " " << xquery[0] << " " << xquery[1]<< endl;
    
    return min_point;
}

/* Prunes the graph to make it more fit for the GreedySearch algorithm */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degreee bound
template <typename T>
void RobustPrune(Graph<T>& G, gIndex p, set<T>& V, float a, int R) {
    
}

/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
void vamana(Graph<T>& G, int L, int R) {
    
}