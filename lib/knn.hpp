/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
#include "utils.hpp"
#include <limits.h>
#include <numeric>
using namespace std;

/* Traverses the graph and finds k-approx NNs */
// s is the index of the start node s 
// xq is the index of the query 
// k is the number of approximate NNs calculated 
// L is the search list size ()
// Returns a pair of sets, the first contains the K-approx NNs and the second contains all the visited nodes
// if L < k the method returns a pair of empty sets
template <typename T>
pair<set<gIndex>, set<gIndex>> GreedySearch(Graph<T>& G, const T& start, const T& xquery, int k, int L) {
    
    // First we check that the input values are correct
    if (L < k) { // L >= k
        cerr << "\nSearch list size L must be greater than or equal to k!\n" << endl;
        // Returns a pair with two empty sets
        return {{},{}};
    }

    // Initialize set L_output = {s} and V = { }
    set<gIndex> L_output;
    L_output.insert(G.get_index_from_vertex(start));
    set<gIndex> V;
    
    // Subtraction of sets L_output \ V
    set<gIndex> diff_set;
    set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));

    // while L_output\V != 0
    while ( !diff_set.empty() ) {
        // cout<<"c"<< endl;

        // Find the vertex with the minimum euclidean distance from the xquery
        T min = find_min_Euclidean(G, diff_set, xquery);
        
        // Updating L and V
        vector<gIndex> neighbors = G.get_neighbors(min);
        set_union(L_output.begin(), L_output.end(), neighbors.begin(), neighbors.end(), inserter(L_output, L_output.begin()));

        V.insert(G.get_index_from_vertex(min));
        // G.insert_sorted(V, min);

        // Upper bound check
        if ( L_output.size() > (long unsigned int) L ) {
            retain_closest_points(G, L_output, xquery, L);          
        }

        diff_set.clear();
        // Recalculate the difference of the sets for the next loop
        set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));
        // diff_set.erase(min);
    }

    // return only the k closests vertices
    retain_closest_points(G, L_output, xquery, k);

    return {L_output, V};

}





/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T>
void RobustPrune(Graph<T>& G, const T& point, set<gIndex>& V, float a, int R) {

    // First we check that the input values are correct
    if (a < 1) { // a >= 1
        cerr << "\nDistance threshold a must be greater than or equal to 1!\n" << endl;
        // Return
        return;
    }

    // Calculating the union of V and the neighbors of the given point, without the point itself
    vector<gIndex> neighbors = G.get_neighbors(point);

    // V <- {V U Nout(p)} \ {p}
    set_union(V.begin(), V.end(), neighbors.begin(), neighbors.end(), inserter(V, V.begin()));
    auto k = find(V.begin(), V.end(), G.get_index_from_vertex(point)); 
    if ( k != V.end() ) {   // Only remove if it was found within V
        V.erase(k);
    }
    // V.erase(G.get_index_from_vertex(point));
    
    // Removing all edges leaving the given vertex and selectively adding up to R edges
    for (gIndex j: neighbors) {
        G.remove_edge(point, G.get_vertex_from_index(j));
    }
    neighbors.clear();

    int insertedCount = 0;

    vector<gIndex> toBeRemoved;
    while (V.size() != 0) {

        // Find the vertex with the minimum euclidean distance from the given point and adding an edge to it
        T p_min = find_min_Euclidean(G, V, point);

        G.add_edge(point, p_min);
        insertedCount++;

        // Stopping if R edges have been added
        if (insertedCount == R) break;

        // Removing from V the vertices that do not satisfy the distance threshold
        toBeRemoved.clear();
        for (gIndex vertex_index: V) {
            T vertex = G.get_vertex_from_index(vertex_index);
            if (a * Euclidean_Distance(p_min, vertex) <= Euclidean_Distance(point, vertex)) {
                toBeRemoved.push_back(vertex_index);
            }
        }
        for (gIndex vertex_index: toBeRemoved) {
            // V.erase(find(V.begin(), V.end(), vertex_index));
            V.erase(vertex_index);
        }
    }
}



/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
T Vamana(Graph<T>& G, int L, int R, float a =1.2) {
    
    int n = G.get_vertices_count();

    // Initializing G to a random graph with out-degree = R
    rDirectional(G, R);


    // Calculating the medoid of the points given
    T s = medoid(G);
    // T s = G.get_vertex_from_index(8736);
    
    // Getting the vertex indices in a random order. Vector sigma will be the random permutation.
    vector<gIndex> sigma(n);
    iota(sigma.begin(), sigma.end(), 0);
    shuffle(sigma.begin(), sigma.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));

    // For all vertices
    for (int i = 0; i < n; i++) { 

        // Getting the random vertex
        T vertex = G.get_vertex_from_index(sigma[i]);

        // Calling GreadySearch() from the medoid to the vertex to get the appropriate sets [L_output,V]
        pair<set<gIndex>, set<gIndex>> result = GreedySearch<T>(G, s, vertex, 1, L);

        set<gIndex> L_output = result.first;
        set<gIndex> V = result.second;
        
        // First calling RobustPrune for the vertex
        // float a = 1.2; // !!!        
        RobustPrune<T>(G, vertex, V, a, R);

        // For each neighbor j of the vertex
        vector<gIndex> neighbors = G.get_neighbors(vertex);
        for (gIndex j: neighbors) {
            
            T neighbor = G.get_vertex_from_index(j);

            // We calculate a candidate set with the neighbor's neighbors and the vertex itself
            vector<gIndex> neighbor_union = G.get_neighbors(neighbor);
            G.insert_sorted(neighbor_union, vertex);
            

            if (neighbor_union.size() > (long unsigned int) R) {
                set<gIndex> neighbor_union_set(neighbor_union.begin(), neighbor_union.end());
                // If the candidate set for the neighbor j is too big (size>R) we call RobustPrune for j
                RobustPrune<T>(G, neighbor, neighbor_union_set, a, R);
            } else {
                // Else we add an edge from the neighbor j to the vertex itself
                G.add_edge(neighbor, vertex);
            }     
        }
    }

    return s;
}