/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
#include "utils.hpp"
using namespace std;

/* Traverses the graph and finds k-approx NNs */
// s is the index of the start node s 
// xq is the index of the query 
// k is the number of approximate NNs calculated 
// L is the search list size ()
// Returns a pair of sets, the first contains the K-approx NNs and the second contains all the visited nodes
// if L < k the method returns a pair of empty sets
template <typename T>
pair<set<T>, set<T>> GreedySearch(Graph<T> G, T start, T xquery, int k, int L) {
    

    // First we check that the input values are correct
    if (L < k) { // L>=k
        cerr << "\nSearch list size L must be greater than or equal to k!\n" << endl;
        // Returns a pair with two empty sets
        return {{},{}};
    }

    // Initialize set L_output = {s} and V = { }
    set<T> L_output, V;
    L_output.insert(start);
    
    // Subtraction of sets L_output \ V
    set<T> diff_set;
    set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));
    
    // while L_output\V != 0
    while( !diff_set.empty() ){

        // cout << "\nL contains:\n";
        // for(auto j : L_output){
        //     cout << j[0] << endl;
        // }

        // cout << "\nV contains:\n";
        // for(auto k : V){
        //     cout << k[0] << endl;
        // }

        // cout << "\n----------\n";

        // Find the vertex with the minimum euclidean distance from the xquery
        T min = find_min_Euclidean(diff_set, xquery);

        set<T> neighbors = G.get_neighbors(min);

        
        // L 
        set_union(L_output.begin(), L_output.end(), neighbors.begin(), neighbors.end(), inserter(L_output, L_output.end()));
        V.insert(min);


       

        // Upper bound check
        if( L_output.size() > (long unsigned int)L ){
            retain_closest_points(L_output, xquery, L);          
        }


        diff_set.clear();
        // Recalculate the difference of the sets for the next loop
        set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));
        // diff_set.erase(min);
    }
    
    // return only the k closests vertices
    retain_closest_points(L_output, xquery, k);

    return {L_output, V};

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