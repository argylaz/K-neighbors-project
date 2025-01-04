/* This file contains all the algorithms relevant to the KNN project such as vamana indexing algorithm */
/* Αll the algorithms below are implementations of given pseudocode */

#include <limits.h>
#include <numeric>
#include "./filterGraph.hpp"
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
pair<set<gIndex>, set<gIndex>> GreedySearch(Graph<T>& G, const T& start, const T& xquery, int k, int L);




// Implementation of Filtered Greedy Search
/*  G:      Graph that represents the dataset        */
/*  xquery: The query vector                         */
/*  k:      The number of neaerst neighbors returned */
/*  L:      The size of the search list              */
/*  Fq:     The query filters                        */
// template <typename T>
template <typename T, typename F> 
pair<set<gIndex>, set<gIndex>> FilteredGreedySearch(FilterGraph<T, F>& G, set<T>& S, const T& xquery, const int k, const int L, set<F>& Fq);





/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T>
void RobustPrune(Graph<T>& G, const T& point, set<gIndex>& V, float a, int R);




/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T, typename F>
void FilteredRobustPrune(FilterGraph<T, F>& G, const T& point, set<gIndex>& V, float a, int R);




/* Implementation of the Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T>
T Vamana(Graph<T>& G, int L, int R, float a = 1.2);


/* Implementation of the Filtered Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T, typename F>
void FilteredVamana(FilterGraph<T, F>& G, int L, int R, map<F, gIndex> MedoidMap, float a = 1.2);




/*
    Function returns a map M, mapping filters to the equivalent medoid node 
    Argument F is the set of all filters
*/
template <typename T, typename F>
map<F , gIndex> FindMedoid(FilterGraph<T, F>& G,  int threshold);







/* Implementation of the Stiched Vamana Indexing Algorithm */
// Lsmall is the search list size for the regular Vamana algorithm
// Rsmall is the degree bound for the regular Vamana algorithm
// Rstiched is is the degree bound for the
template <typename T, typename F>
void StichedVamana(FilterGraph<T, F>& G, int Lsmall, int Rsmall, int Rstiched, float a = 1.2);



/*----------------------------------------------------Function Definitions-----------------------------------------------------------*/



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

        // Find the vertex with the minimum euclidean distance from the xquery
        T min = find_min_Euclidean(G, diff_set, xquery);
        
        // Updating L and V
        vector<gIndex> neighbors = G.get_neighbors(min);
        set_union(L_output.begin(), L_output.end(), neighbors.begin(), neighbors.end(), inserter(L_output, L_output.begin()));

        V.insert(G.get_index_from_vertex(min));

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

    // cout << "V size " << V.size() << endl;
    // cout << "L_output size " << L_output.size() << endl;


    return {L_output, V};

}





// Implementation of Filtered Greedy Search
/*  G:      Graph that represents the dataset        */
/*  xquery: The query vector                         */
/*  k:      The number of neaerst neighbors returned */
/*  L:      The size of the search list              */
/*  Fq:     The query filters                        */
// template <typename T>
template <typename T, typename F> 
pair<set<gIndex>, set<gIndex>> FilteredGreedySearch(FilterGraph<T, F>& G, set<T>& S, const T& xquery, const int k, const int L, set<F>& Fq) {


    // First we check that the input values are correct
    if (L < k) { // L >= k
        cerr << "\nSearch list size L must be greater than or equal to k!\n" << endl;
        // Returns a pair with two empty sets
        return {{},{}};
    }


    // Get the set of vertices from the graph
    // set<T> S = G.get_vertices();

    // Initialize set L_output = {} and V = {}
    set<gIndex> L_output;
    set<gIndex> V;


    // Filtering 
    for( T s : S ){
            
        /* GET FILTERS FROM GRAPH */
        set<F> Fs  = G.get_filters(G.get_index_from_vertex(s)); 

        // Checking if the intersection of Fs and Fq is empty
        set<F> intersection;
        set_intersection(Fs.begin(), Fs.end(), Fq.begin(), Fq.end(), inserter(intersection, intersection.begin()));      
        if (!intersection.empty()) {
            L_output.insert(G.get_index_from_vertex(s));
        }        

    }

    // cout << "\nL_output size is " << L_output.size() << endl;

    // Subtraction of sets L_output \ V
    set<gIndex> diff_set;  // !!! vector for performance
    set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));

    while ( !diff_set.empty() ) {

        // Find the vertex with the minimum euclidean distance from the xquery // !!!
        T min = find_min_Euclidean(G, diff_set, xquery);
        // Check whether min has been already inserted to V
        // Check whether the p' exists within V
        V.insert(G.get_index_from_vertex(min));

        // Line 4 && 5 of pseudocode
        vector<gIndex> neighbors = G.get_neighbors(min);

        // For each one of min's (p*) neighbors p' (p_ in the code
        set<gIndex> Nout_;
        for ( gIndex p_ : neighbors) {
 
            /* GET FILTERS FROM GRAPH */
            set<F> Fp_ = G.get_filters(p_); // !!!
            
            // Check if the intersection of Fp_ and Fq is not empty and whether the p' exists within V
            // Inserting straight to L_output instead of making a new set N'out(p*)
            set<F> intersection;
            set_intersection(Fp_.begin(), Fp_.end(), Fq.begin(), Fq.end(), inserter(intersection, intersection.begin()));
            if ( !intersection.empty() && V.find(p_) == V.end() ) {
                Nout_.insert(p_); 
            }

        }

        set_union(L_output.begin(), L_output.end(), Nout_.begin(), Nout_.end(), inserter(L_output, L_output.begin()));

        // Upper bound check
        if ( L_output.size() > (long unsigned int) L ) {
            retain_closest_points(G, L_output, xquery, L);          
        }

        // Recalculate the difference of L and V
        diff_set.clear();
        set_difference(L_output.begin(), L_output.end(), V.begin(), V.end(), inserter(diff_set, diff_set.begin()));
        
    }

    if( L_output.size() > (size_t)k )
        retain_closest_points(G, L_output, xquery, k);

    // cout << "V size " << V.size() << endl;
    // cout << "L_output size " << L_output.size() << endl;

    return {L_output,V};
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




/* Prunes the graph to make it more fit for the GreedySearch algorithm, modified to have at most R out-neighbors for p */
// p is the index of the given point p, which is included in the Graph
// V is the candidate set
// a is the distance threshold
// R is the degree bound
template <typename T, typename F>
void FilteredRobustPrune(FilterGraph<T, F>& G, const T& point, set<gIndex>& V, float a, int R) {

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

            set<F> F_vertex = G.get_filters(vertex_index);                       // p'
            set<F> F_point = G.get_filters(G.get_index_from_vertex(point));      // p
            set<F> F_p_min = G.get_filters(G.get_index_from_vertex(p_min));      // p*

            set<F> F_intersection; // the intersection of F_vertex and F_point
            set_intersection(F_vertex.begin(), F_vertex.end(), F_point.begin(), F_point.end(), inserter(F_intersection, F_intersection.begin()));

            // F_intercestion <not subset of> F_p_min   IFF    F_intersection <intersection> F_p_min != F_intersection
            set<F> temp_intersection;
            set_intersection(F_intersection.begin(), F_intersection.end(), F_p_min.begin(), F_p_min.end(), inserter(temp_intersection, temp_intersection.begin()));

            if (temp_intersection != F_intersection) {
                continue;
            }


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

    if (R >= n) R = n - 1;
    if (L >= n) L = n - 1;

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




/* Implementation of the Filtered Vamana Indexing Algorithm */
// L is the search list size
// R is the degree bound
template <typename T, typename F>
void FilteredVamana(FilterGraph<T, F>& G, int L, int R, map<F, gIndex> MedoidMap, float a) {

    int n = G.get_vertices_count();
    // set<gIndex> V;

    // At the beginning the graph is empty

    
    // Getting the vertex indices in a random order. Vector sigma will be the random permutation.
    vector<gIndex> sigma(n);
    iota(sigma.begin(), sigma.end(), 0);
    shuffle(sigma.begin(), sigma.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));

    // For all vertices
    for (int i = 0; i < n; i++) { 

        // Getting the random vertex
        T vertex = G.get_vertex_from_index(sigma[i]);
        
        // Getting its filters
        set<F> Fx = G.get_filters(sigma[i]);


        // Construct S, which is the set of starting nodes belonging to the same filters  as vertex sigma(i)
        set<T> Sf_x;
        for (F filter: Fx) {
            Sf_x.insert(G.get_vertex_from_index(MedoidMap[filter]));
        }

        // Calling GreadySearch() from the medoid to the vertex to get the appropriate sets [L_output,V]
        pair<set<gIndex>, set<gIndex>> result = FilteredGreedySearch<T, F>(G, Sf_x, vertex, 0, L, Fx);

        set<gIndex> V_vertex = result.second;
        
        // set_union(V.begin(), V.end(). V_vertex.begin(), V_vertex.end(), inserter(V, V.begin()));
        

        // First calling RobustPrune for the vertex
        // float a = 1.2; // !!!        
        FilteredRobustPrune<T, F>(G, vertex, V_vertex, a, R);

        // For each neighbor j of the vertex
        vector<gIndex> neighbors = G.get_neighbors(vertex);
        // ------------------------------------------------
        for (gIndex j: neighbors) {
            
            T neighbor = G.get_vertex_from_index(j);

            // We add an edge from the neighbor j to the vertex itself
            G.add_edge(neighbor, vertex);
            
            // Getting the neighbors of the neighbor j (including the vertex itself)
            vector<gIndex> j_neighbors = G.get_neighbors(neighbor);

            if ( j_neighbors.size() > (long unsigned int) R) {
                set<gIndex> set_j_neighbors(j_neighbors.begin(), j_neighbors.end());
                FilteredRobustPrune<T, F>(G, neighbor, set_j_neighbors, a, R);
            }
   
        }
    }

}


/*
    Function returns a map M, mapping filters to the equivalent medoid node 
    Argument F is the set of all filters
*/
template <typename T, typename F>
map<F, gIndex> FindMedoid(FilterGraph<T, F>& G,  int threshold) {
    map<F, gIndex> M;

    set<F> Filters = G.get_filters_set(); 
    
    // Initialization of zero map T
    map<gIndex, int> T_;               // Zero map T is intended as a counter

    set<T> vertices = G.get_vertices();

    // Initialising random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
 
    // For each filter in the set
    for ( F filter : Filters ) {

        // contains the gIndices of all points matching filter in question
        vector<gIndex> Pf;

        // Find all the gIndices of vertices matching the filter
        for ( T vertex : vertices ) {

            set<F> Fx = G.get_filters(G.get_index_from_vertex(vertex));

            if (Fx.find(filter) != Fx.end()){
                Pf.push_back(G.get_index_from_vertex(vertex));
            }   
        }

        // If Pf is empty, skip this filter
        if (Pf.empty()) {
            continue;
        }
        
        // Maximum threshold value should be he size of Pf
        threshold = min(threshold, static_cast<int>(Pf.size()));

        // Randomly sample `threshold` unique elements from Pf
        unordered_set<int> selected_indices;
        vector<gIndex> Rf;
        uniform_int_distribution<int> dist(0, Pf.size() - 1);

        while (Rf.size() < static_cast<size_t>(threshold)) {
            int random_index = dist(rng);
            if (selected_indices.insert(random_index).second) {
                Rf.push_back(Pf[random_index]);
            }
        }

        // Finding p_min point, where p_min is min{T[p], for each p in Rf};
        gIndex p_min_index = Rf[0];
        for ( size_t i = 1 ; i < Rf.size() ; i++ ) {
            if ( T_[Rf[i]] < T_[p_min_index] ) {
                p_min_index = Rf[i];
            }
        }
        
        M[filter] = p_min_index;
        T_[p_min_index]++;

    }

    return M;

}



template <typename T, typename F>
set<T> get_nodes_from_gIndex_map(FilterGraph<T, F>& G, map<F, gIndex> M){

    set<T> nodes;

    for( auto& pair : M){
        gIndex& node_index = pair.second;
        nodes.insert(G.get_vertex_from_index(node_index));
    }

    return nodes;
}



/* Implementation of the Stiched Vamana Indexing Algorithm */
// Lsmall is the search list size for the regular Vamana algorithm
// Rsmall is the degree bound for the regular Vamana algorithm
// Rstiched is is the degree bound for the
template <typename T, typename F>
void StichedVamana(FilterGraph<T, F>& G, int Lsmall, int Rsmall, int Rstiched, float a) {

    set<F> filters = G.get_filters_set();
    set<T> vertices = G.get_vertices();

    map<F, Graph<T>*> Gf;
    mutex graph_mutex;

    // Lambda for asynchronous subgraphs construction
    auto process_filter = [&](const F& filter) {
        // Getting the vertices that have the filter
        vector<gIndex> Pf;
        for (const T& vertex : vertices) {
            set<F> Fx = G.get_filters(G.get_index_from_vertex(vertex));
            if (Fx.find(filter) != Fx.end()) {
                Pf.push_back(G.get_index_from_vertex(vertex));
            }
        }

        // Creating the subgraph
        Graph<T>* graph_f = new Graph<T>;
        for (gIndex i : Pf) {
            graph_f->add_vertex(G.get_vertex_from_index(i));
        }

        // Running Vamana Indexing Algorithm on the subgraph
        Vamana(*graph_f, Lsmall, Rsmall, a);

        // Critical area for adding the subgraph to the shared map
        {
            lock_guard<mutex> lock(graph_mutex);
            Gf[filter] = graph_f;
        }
    };

    // Parallelize subgraph construction
    vector<thread> threads;
    for (const F& filter : filters) {
        threads.emplace_back(process_filter, filter);
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }


    // Merging the Gf subgraphs into G
    for (const F& filter : filters) {
        set<T> gf_vertices = Gf[filter]->get_vertices();
        for (const T& gf_vertex : gf_vertices) {
            vector<gIndex> gf_neighbor_indices = Gf[filter]->get_neighbors(gf_vertex);
            for (gIndex gf_neighbor_index : gf_neighbor_indices) {
                T neighbor = Gf[filter]->get_vertex_from_index(gf_neighbor_index);
                // Technically, this is a critical area, but it is not necessary to lock it
                {
                    std::lock_guard<std::mutex> lock(graph_mutex);
                    G.add_edge(gf_vertex, neighbor);
                }
            }
        }
        delete Gf[filter];
    }


    // Parallelized Filtered Robust Prune
    auto robust_prune = [&](int start, int end) {
        auto it = vertices.begin();
        std::advance(it, start);
        for (int i = start; i < end; ++i, ++it) {
            T vertex = *it;
            vector<gIndex> neighbor_indices = G.get_neighbors(vertex);
            set<gIndex> Nout(neighbor_indices.begin(), neighbor_indices.end());
            FilteredRobustPrune<T, F>(G, vertex, Nout, a, Rstiched);
        }
    };

    // Divide vertices into chunks for pruning
    int num_threads = std::thread::hardware_concurrency();
    int chunk_size = (vertices.size() + num_threads - 1) / num_threads;

    threads.clear();
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = std::min(start + chunk_size, (int)vertices.size());
        threads.emplace_back(robust_prune, start, end);
    }

    // Join all pruning threads
    for (auto& thread : threads) {
        thread.join();
    }
}
