/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
#include "utils.hpp"
#include <limits.h>
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
    if (L < k) { // L >= k
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

        // Find the vertex with the minimum euclidean distance from the xquery
        T min = find_min_Euclidean(diff_set, xquery);
        
        
        // Updating sets L and V
        set<T> neighbors = G.get_neighbors(min);
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





/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T>
void RobustPrune(Graph<T>& G, T point, set<T>& V, float a, int R) {

    // First we check that the input values are correct
    if (a < 1) { // a >= 1
        cerr << "\nDistance threshold a must be greater than or equal to 1!\n" << endl;
        // Return
        return;
    }

    // Calculating the union of V and the neighbors of the given point, without the point itself
    set<T> neighbors = G.get_neighbors(point);
    set_union(V.begin(), V.end(), neighbors.begin(), neighbors.end(), inserter(V, V.end()));
    V.erase(point);
    
    // Removing all edges leaving the given vertex and selectively adding up to R edges
    for (T neighbor: neighbors) {
        G.remove_edge(point, neighbor);
    }
    neighbors.clear();

    vector<T> toBeRemoved;
    while (V.size() != 0) {

        // Find the vertex with the minimum euclidean distance from the given point and adding an edge to it
        T p_min = find_min_Euclidean(V, point);

        neighbors.insert(p_min);
        G.add_edge(point, p_min);

        // Stopping if R edges have been added
        if (neighbors.size() == (long unsigned int) R) break;

        // Removing from V the vertices that do not satisfy the distance threshold
        toBeRemoved.clear();
        for (T vertex: V) {
            if (a * Euclidean_Distance(p_min, vertex) <= Euclidean_Distance(point, vertex)) {
                toBeRemoved.push_back(vertex);
            }
        }
        for (T vertex: toBeRemoved) {
            V.erase(vertex);
        }
        
    }
}



/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
void vamana(Graph<T>& G, int L, int R) {
    rDirectional(G, R);
    T s = medoid<T>(G);
    
    set<T> shuffled_vertices = G.get_vertices();
    // shuffle();

}