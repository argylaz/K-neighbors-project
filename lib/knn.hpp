/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
/* Αll the algorithms below are implementations of given pseudocode */

#include <limits.h>
#include <numeric>
#include "../lib/filterGraph.hpp"
using namespace std;


/*----------------------------------------------------Function Declarations---------------------------------------------------------*/


/* Traverses the graph and finds k-approx NNs */
// s is the index of the start node s 
// xq is the index of the query 
// k is the number of approximate NNs calculated 
// L is the search list size ()
// Returns a pair of sets, the first contains the K-approx NNs and the second contains all the visited nodes
// if L < k the method returns a pair of empty sets
template <typename T>
pair<set<gIndex>, set<gIndex>> GreedySearch(Graph<T>& G, T start, T xquery, int k, int L);




// Implementation of Filtered Greedy Search
/*  G:      Graph that represents the dataset        */
/*  xquery: The query vector                         */
/*  k:      The number of neaerst neighbors returned */
/*  L:      The size of the search list              */
/*  Fq:     The query filters                        */
// template <typename Type>
template <typename Type, typename F> 
pair<set<gIndex>, set<gIndex>> FilteredGreedySearch(FilterGraph<vector<Type>, F>& G, set<vector<Type>> S, vector<Type>& xquery, const int k, const int L, vector<F>& Fq);




/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T>
void RobustPrune(Graph<T>& G, T point, set<gIndex>& V, float a, int R);




/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T,typename F>
void FilteredRobustPrune(FilterGraph<T,F>& G, T point, set<gIndex>& V, float a, int R);




/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
T Vamana(Graph<T>& G, int L, int R, float a = 1.2);


/* Implementation of the Filtered Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename Type, typename F>
void FilteredVamana(FilterGraph<vector<Type>,F>& G, int L, int R, float a = 1.2);




/*
    Function returns a map M, mapping filters to the equivalent medoid node 
    Argument F is the set of all filters
*/
template <typename Type, typename F>
map<vector<F> , gIndex> FindMedoid(FilterGraph<vector<Type>, F>& G,  int threshold);



/*----------------------------------------------------Function Definitions-----------------------------------------------------------*/



/* Traverses the graph and finds k-approx NNs */
// s is the index of the start node s 
// xq is the index of the query 
// k is the number of approximate NNs calculated 
// L is the search list size ()
// Returns a pair of sets, the first contains the K-approx NNs and the second contains all the visited nodes
// if L < k the method returns a pair of empty sets
template <typename T>
pair<set<gIndex>, set<gIndex>> GreedySearch(Graph<T>& G, T start, T xquery, int k, int L) {
    
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



// Implementation of Filtered Greedy Search
/*  G:      Graph that represents the dataset        */
/*  xquery: The query vector                         */
/*  k:      The number of neaerst neighbors returned */
/*  L:      The size of the search list              */
/*  Fq:     The query filters                        */
// template <typename Type>
template <typename Type, typename F> 
pair<set<gIndex>, set<gIndex>> FilteredGreedySearch(FilterGraph<vector<Type>, F>& G, set<vector<Type>> S,  vector<Type>& xquery, const int k, const int L, vector<F>& Fq) {
     

    // First we check that the input values are correct
    if (L < k) { // L >= k
        cerr << "\nSearch list size L must be greater than or equal to k!\n" << endl;
        // Returns a pair with two empty sets
        return {{},{}};
    }


    // Get the set of vertices from the graph
    // set<vector<Type>> S = G.get_vertices();

    // Initialize set L_output = {} and V = {}
    set<gIndex> L_output;
    set<gIndex> V;


    // Filtering 
    for( vector<Type> s : S ){
            
        /* GET FILTERS FROM GRAPH */
        vector<F> Fs  = G.get_filters(G.get_index_from_vertex(s)); 

        vector<F> Fx = G.get_filters(G.get_index_from_vertex(xquery));
 
        // Checking if the intersection of Fs and Fq is empty        
        for( size_t j = 0 ; j < Fx.size() ; j++ ){

            /* Check if the filters match */
            if( Fs[j] == Fx[j]){
                L_output.insert(G.get_index_from_vertex(s));
                break;                        // We need at least one to match so that the intersection is non-empty
            }
        }
    }

    // Subtraction of sets L_output \ V
    set<gIndex> diff_set;  // !!! vector for performance
    set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));

    while ( !diff_set.empty() ) {

        // Find the vertex with the minimum euclidean distance from the xquery // !!!
        vector<Type> min = find_min_Euclidean<Type>(G, diff_set, xquery);
        // Check whether min has been already inserted to V
        // Check whether the p' exists within V
        V.insert(G.get_index_from_vertex(min));

        // Line 4 && 5 of pseudocode
        vector<gIndex> neighbors = G.get_neighbors(min);

        // For each one of min's (p*) neighbors
        for ( gIndex n : neighbors) {
 
            /* GET FILTERS FROM GRAPH */
            vector<Type> Fs = G.get_filters(n); // !!!
            
            // Check if the intersection of Fs and Fq is empty
            bool filter_flag = false;
            for( size_t j = 0 ; j < Fq.size() ; j++ ) {

                // We need at least one to match for the intersection to be non-empty
                if( Fq[j] == Fs[j] ){
                    filter_flag = true;
                    break;
                } 
            }
            
            // Check whether the p' exists within V
            // vector<gIndex>::iterator iter = find(V.begin(), V.end(), n);
            set<gIndex>:: iterator iter = find(V.begin(), V.end(), n);


            // Inserting straight to L_output instead of making a new set N'out(p*)
            if( filter_flag && iter == V.end() ) {
                L_output.insert(G.get_index_from_vertex(min)); 
            }

        }

        // Upper bound check
        if ( L_output.size() > (long unsigned int) L ) {
            retain_closest_points(G, L_output, xquery, L);          
        }

        // Recalculate the difference of L and V
        diff_set.clear();
        set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));
        
    }

    retain_closest_points(G, L_output, xquery, k);

    return {L_output,V};
} 

/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T>
void RobustPrune(Graph<T>& G, T point, set<gIndex>& V, float a, int R) {

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
    if( k != V.end() ){   // Only remove if it was found within V
        V.erase(k);
    }
    
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
            V.erase(find(V.begin(), V.end(), vertex_index));
        }
    }
}


/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T,typename F>
void FilteredRobustPrune(FilterGraph<T,F>& G, T point, set<gIndex>& V, float a, int R) {

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
    if( k != V.end() ){   // Only remove if it was found within V
        V.erase(k);
    }
    
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

            // For one filter 
            vector<F> F_point = G.get_filters(G.get_index_from_vertex(point));      // p
            vector<F> F_p_min = G.get_filters(G.get_index_from_vertex(p_min));      // p*
            vector<F> F_vertex = G.get_filters(vertex_index);                       // p'
            if( F_point == F_vertex && F_point != F_p_min ){
                continue;
            }


            /*
            // For many filters
            vector<F> F_point = G.get_filters(G.get_index_from_vertex(point));      // p
            vector<F> F_p_min = G.get_filters(G.get_index_from_vertex(p_min));      // p*
            vector<F> F_vertex = G.get_filters(vertex_index);                       // p'
            // Assume that all filters have the same size
            for( size_t i = 0 ; i < F_point.size() ; i++ ){
                if( F_point[i] == F_vertex[i] && F_point[i] != F_p_min[i]){
                    continue;
                }
            }
            */



            if (a * Euclidean_Distance(p_min, vertex) <= Euclidean_Distance(point, vertex)) {
                toBeRemoved.push_back(vertex_index);
            }
        }
        for (gIndex vertex_index: toBeRemoved) {
            V.erase(find(V.begin(), V.end(), vertex_index));
        }
    }
}


/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
T Vamana(Graph<T>& G, int L, int R, float a) {
    
    int n = G.get_vertices_count();

    // Initializing G to a random graph with out-degree = R
    rDirectional(G, R);


    // Calculating the medoid of the points given
    T s = medoid(G);
    
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




/* Implementation of the Filtered Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename Type, typename F>
void FilteredVamana(FilterGraph<vector<Type>,F>& G, int L, int R, float a){

    int n = G.get_vertices_count();
    set<gIndex> V;

    // At the beginning the graph is empty

    // Calculating the medoid of the points given
    // Threshold is 1                                         ///////////// Maybe this has to be changed
    map<vector<F> , gIndex> MedoidMap = FindMedoid(G, 1);
    // set<vector<Type>> Sf = get_nodes_from_gIndex_map(G, MedoidMap);

    
    
    // Getting the vertex indices in a random order. Vector sigma will be the random permutation.
    vector<gIndex> sigma(n);
    iota(sigma.begin(), sigma.end(), 0);
    shuffle(sigma.begin(), sigma.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));

    // For all vertices
    for (int i = 0; i < n; i++) { 

        // Getting the random vertex
        vector<Type> vertex = G.get_vertex_from_index(sigma[i]);
        
        // Getting its filter
        vector<F> filter = G.get_filters(sigma[i]);


        // Construct S, which is the set of starting nodes belonging to the same filters  as vertex sigma(i)
        set<vector<Type>> Sf;
        Sf.insert(G.get_vertex_from_index(MedoidMap[filter]));

        for( vector<Type> k : Sf){
            cout << "\nThe Medoid of filter is ";
            print_vector(k);
            cout << endl;
        }

        // Calling GreadySearch() from the medoid to the vertex to get the appropriate sets [L_output,V]
        pair<set<gIndex>, set<gIndex>> result = FilteredGreedySearch<Type,F>(G, Sf, vertex, 0, L, filter);

        // set<gIndex> L_output = result.first;
        set<gIndex> V_vertex = result.second;
        for( gIndex i : V_vertex){
            V.insert(i);
        }

        // First calling RobustPrune for the vertex
        // float a = 1.2; // !!!        
        FilteredRobustPrune<vector<Type>,F>(G, vertex, V_vertex, a, R);

        // For each neighbor j of the vertex
        vector<gIndex> neighbors = G.get_neighbors(vertex);
        // ------------------------------------------------
        for (gIndex j: neighbors) {
            
            vector<Type> neighbor = G.get_vertex_from_index(j);
            // //  we add an edge from the neighbor j to the vertex itself
            // G.add_edge(neighbor, vertex);

            // The neighbors of the neighbor j
            vector<gIndex> j_neighbors = G.get_neighbors(neighbor);
            G.insert_sorted(j_neighbors, vertex);
            set<gIndex> set_j_neighbors(j_neighbors.begin(), j_neighbors.end());

            if( set_j_neighbors.size() > (long unsigned int) R){
                FilteredRobustPrune<vector<Type>,F>(G,G.get_vertex_from_index(j), set_j_neighbors, a, R);
            }



            // // We calculate a candidate set with the neighbor's neighbors and the vertex itself
            // vector<gIndex> neighbor_union = G.get_neighbors(neighbor);
            // G.insert_sorted(neighbor_union, vertex);
            

            // if (neighbor_union.size() > (long unsigned int) R) {
            //     set<gIndex> neighbor_union_set(neighbor_union.begin(), neighbor_union.end());
            //     // If the candidate set for the neighbor j is too big (size>R) we call RobustPrune for j
            //     RobustPrune<T>(G, neighbor, neighbor_union_set, a, R);
            // } else {
            //     // Else we add an edge from the neighbor j to the vertex itself
            //     G.add_edge(neighbor, vertex);
            // }     
        }
    }

}


/*
    Function returns a map M, mapping filters to the equivalent medoid node 
    Argument F is the set of all filters
*/
template <typename Type, typename F>
map<vector<F> , gIndex> FindMedoid(FilterGraph<vector<Type>, F>& G,  int threshold){
    map<vector<F> , gIndex> M;

    set<vector<F>> Filters = G.get_filters_set(); 
    
    // Νομίζω ότι εξ αρχής αρχικοποιείται σαν zero map
    // εναλλακτικά θα πρέπει να κάνουμε αρχικοποίηση σε 0 κάθε στοιχείο του map 
    map<gIndex, int> T;               // Zero map T is intended as a counter

    set<vector<Type>> vertices = G.get_vertices();
 
    // For each filter in the set
    for( vector<F> f : Filters ){

        // cout << "\nFilter ";
        // print_vector(f);
        // cout << endl;

        // contains the gIndices of all points matching filter in question
        vector<gIndex> Pf;

        // Find all the gIndices matching the filter f
        for( vector<Type> v : vertices ){

            // int dimension = F[i].first
            // F value = F[i].second

            vector<F> filter = G.get_filters(G.get_index_from_vertex(v));              
            
            if( filter == f ){
                Pf.push_back(G.get_index_from_vertex(v));
                // cout << "Added ";
                // print_vector(v);
            }

            // if( v[dimension] == value ){
            //     Pf.push_back(G.get_index_from_vertex(v));
            // }      
        }

        

        // To do :: Check for optimization
        // Let Rf <- threshold randomly sampled data point ids from Pf        
        // Create a vector with all the elements of Pf
        vector<gIndex> temp_vector;
        for( gIndex vec : Pf ){
            temp_vector.push_back(vec);
        }


        // To obtain a time-based seed 
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        
        // Shuffle the temp vector Pf
        shuffle(temp_vector.begin(), temp_vector.end(), default_random_engine(seed));


        if( (size_t) threshold >= temp_vector.size() ){
            threshold = temp_vector.size();
        }

        // Keep the first threshold items of the shuffled vector
        vector<gIndex> Rf(temp_vector.begin(), temp_vector.begin() + threshold);

        // Finding p_min point, where p_min is min{T[p], for each p in Rf};
        gIndex p_min_index;

        p_min_index = Rf[0];
        for( size_t i = 1 ; i < Rf.size() ; i++ ){
            if( T[Rf[i]] < T[p_min_index] ){
                p_min_index = Rf[i];
            }
        }
        
        M[f] = p_min_index;
        T[p_min_index]++;

    }


    return M;

}


template <typename T, typename F>
set<T> get_nodes_from_gIndex_map(FilterGraph<T,F>& G, map<vector<F>, gIndex> M){

    set<T> nodes;

    for( auto& pair : M){
        gIndex& node_index = pair.second;
        nodes.insert(G.get_vertex_from_index(node_index));
    }

    return nodes;
}