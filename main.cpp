#include <iostream>
#include "lib/knn.hpp"

#define NUM_DIMENSIONS 100 // The number of dimensions of each vector

using namespace std;

/* Main method of the project that runs our vamana (from knn.hpp) on a graph (from graph.hpp)
   created from the databases given in fvec files */
int main(int argc, char* argv[]) {

    int k, L, R;
    float a = 2;    // Default value 
    string base_name, query_name, groundtruth_name, vamana_type; // get arguments figures out the correct dataset files
    if ( get_arguments(argc, (const char**)argv, k, L, a, R, base_name, query_name, groundtruth_name, vamana_type) == -1){
        return -1;
    }

    
    // Reading Groundtruth
    vector<vector<gIndex>> groundtruth;

    if (!hasBinExtension(groundtruth_name)) { // If the queries are unfitlered read_vecs reads the .fvecs file
        groundtruth = read_vecs<int>(groundtruth_name);
        cout << groundtruth.size() << endl;
    } else {                                  // If they are filtered read_groundtruth reads .bin file
        // Check whether the file has been provided or calculated in advance (exists in sift/ )
        if (fopen(groundtruth_name.c_str(), "r")) {
            groundtruth = read_groundtruth("sift/groundtruth.bin");
        } else {
            cerr << "The groundtruth file for the requested datasets has not been provided!" << endl;
            cerr << "Results cannot be properly evaluated!" << endl;
            cerr << "Calculate with make groundtruth or add groundtruth file to sift/.." << endl;
            return -1;
        }
    }


    // Reading Queries
    if (!hasBinExtension(query_name)) {  // If the file is not .bin then it's a .fvec so we call read_vecs
        vector<vector<float>> queries = read_vecs<float>(query_name);
    } else {                             // If it is a .bin file we call read_queries
        map<vector<float>, float> queries = read_queries(query_name);
    }  // !!! NEEDS TO BE FIXED
    

    // Reading Base and creating Graph
    unique_ptr<Graph<std::vector<float>>> G; // Creating unique pointer to allow polymorphism and avoid potential memory leaks
    
    if (!hasBinExtension(base_name)) {
        G = make_unique<Graph<std::vector<float>>>();
        vec_to_graph<float>(base_name, *G);
    } else {
        G = make_unique<FilterGraph<std::vector<float>, float>>(base_name, 100);
    }

    // Get vertices (vectors) from graph
    set<vector<float>> base = G->get_vertices();

    cout << G->get_vertices_count() << " points loaded\n";

    // Simple Filtered Vamana
    if ( vamana_type == "simple" ) {
        // Attempting to cast G to (simple) Graph
        auto* GS = dynamic_cast<Graph<std::vector<float>>*>(G.get());
        // Running Vamana indexing algorithm
        vector<float> medoid = Vamana<vector<float>>(*GS, L, R, a);
    }
    // Filtered Vamana
    else if ( vamana_type == "filtered" ) {
        // Attemptingto cast G to FilterGraph
        auto* FG = dynamic_cast<FilterGraph<std::vector<float>, float>*>(G.get());
        // Running Filtered Vamana indexing algorithm
        // FilteredVamana<>();
    }
    // Stiched Graph
    else if ( vamana_type == "stitched" ) {
        // Attemptingto cast G to FilterGraph
        auto* FG = dynamic_cast<FilterGraph<std::vector<float>, float>*>(G.get());
        // Running Stiched Vamana indexing algorithm
        StichedVamana<vector<float>, float>(*FG, 2, 2, 2);
    }

    cout << "Graph has " << G->get_edge_count() << " edges\n\n"; 


    // Initialise iterators
    auto m = groundtruth.begin();
    auto n = queries.begin();

    // Calculate and print recall for given queries
    float total_recall = 0.0f;
    int count = 0;
    while ( m != groundtruth.end() && n != queries.end() ) {
        // For each query
        vector<float> q = *n;
        
        cout << "\nQuery: ";
        print_vector<float>(q);
        cout << "\n";


        // Run GreedySearch to get the k nearest neighbors
        pair<set<gIndex> , set<gIndex>> res = GreedySearch(*G, medoid, q, k, L);
        set<gIndex> X = res.first;

        // // Convert to set of indices
        // set<int> X;
        // for(vector<float> l : temp) {
        //     int index = G->get_index_from_vertex(l);
        //     X.insert(index);
        // }

        // Get ground truth
        set<int> T( (*m).begin(), (*m).begin() + k);
        
        // Calculate intersection for recall
        set<int> V_intersec;
        set_intersection(X.begin(), X.end(), T.begin(), T.end(), inserter(V_intersec, V_intersec.begin()));

        cout << "\nIntersection size is:" << V_intersec.size();
        cout << "\nT size is:" << T.size();
        
        // Calculate recall and add to total
        float recall = V_intersec.size() / ((float) X.size());
        cout << "\nRecall is: " << recall << endl;
        total_recall += recall;
        count++;

        m++;
        n++;
    }
    
    total_recall = total_recall / count;
    cout << "\nTotal Recall is " << total_recall*100 <<"%\n" << endl;
    return 0;
}