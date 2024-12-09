#include <iostream>
#include "lib/knn.hpp"

#define NUM_DIMENSIONS 100 // The number of dimensions of each vector

using namespace std;

/* Returns the number of queries for the given query dataset ()*/
int get_num_queries(string& query_name) {
    if (query_name == "dummy-data.bin") {
        return 10000;
    } else if (query_name == "contest-data-release-1m.bin") {
        return 1000000;
    } else if (query_name == "contest-data-release-10m.bin") {
        return 10000000;
    }
    else return -1;  // Signifying wrong query name
}

/* */
void results_Greedy(Graph<vector<float>> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<vector<float>>& queries, vector<float> medoid);

/* */
void results_Filtered_Greedy(FilterGraph<vector<float>, float> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<vector<float>>& queries, vector<float>& queries_filters);



/* Main method of the project that runs our vamana (from knn.hpp) on a graph (from graph.hpp)
   created from the databases given in fvec files */
int main(int argc, char* argv[]) {

    int k, L, R;
    float a = 1.2;    // Default value 
    string data_set, base_name, query_name, groundtruth_name, vamana_type; // get arguments figures out the correct dataset files
    if ( get_arguments(argc, (const char**)argv, k, L, a, R, data_set, base_name, query_name, groundtruth_name, vamana_type) == -1) {
        return -1;
    }



    vector<vector<gIndex>> groundtruth;
    vector<vector<float>> queries;

    // Simple Vamana Case
    if ( vamana_type == "simple" ) {

        // Reading Groundtruth
        groundtruth = read_vecs<int>(groundtruth_name);
        cout << groundtruth.size() << endl;
        
        // Reading Queries        
        queries = read_vecs<float>(query_name);

        // Reading Base and creating Graph
        Graph<vector<float>> *G = new Graph<vector<float>>(true);
        vec_to_graph<float>(base_name, *G);

        cout << G->get_vertices_count() << " points loaded\n";

        vector<float> medoid;
        if (!(G->get_graph_from_bin("simple" + data_set, medoid))) {

            // Running Vamana indexing algorithm
            medoid = Vamana<vector<float>>(*G, L, R, a);
            G->save_graph_to_bin("simple" + data_set);
        }        


        cout << "Graph has " << G->get_edge_count() << " edges\n\n"; 


        results_Greedy(G, k, L, groundtruth, queries, medoid);

        
        delete(G);
        
    } 
    // Stitched or Filtered Vamana Case
    else {  

        // Reading Groundtruth
        // Check whether the file has been provided or calculated in advance (exists in sift/ )
        if (fopen(groundtruth_name.c_str(), "r")) {
            groundtruth = read_groundtruth("groundtruth_name");
        } else {
            cerr << "The groundtruth file for the requested datasets has not been provided!" << endl;
            cerr << "Results cannot be properly evaluated!" << endl;
            cerr << "Calculate with make groundtruth or add groundtruth file to sift/.." << endl;
            return -1;
        }

        // Reading Queries
        int num_queries = get_num_queries(query_name);
        pair< vector<vector<float>>, vector<float> > queries_data = read_queries(query_name, num_queries, NUM_DIMENSIONS);
        queries = queries_data.first;
        vector<float> queries_filters = queries_data.second;
        

        // Reading Base and creating Graph
        FilterGraph<vector<float>, float> *G = new FilterGraph<vector<float>, float>(base_name, NUM_DIMENSIONS);

        cout << G->get_vertices_count() << " points loaded\n";


        vector<float> dummy;
        map<float,gIndex> MedoidMap;
        string prefix = vamana_type + data_set;
        
        if (!(G->get_graph_from_bin(prefix, dummy))) {
            // Find Medoid Map
            MedoidMap = FindMedoid(*G, (int)floor(0.1 * G->get_vertices_count()) );
            
            if ( vamana_type == "filtered" ) {
                // Running Filtered Vamana indexing algorithm
                // FilteredVamana<>();
            }
            // Stiched Graph
            else if ( vamana_type == "stitched" ) {
                // Running Stiched Vamana indexing algorithm
                StichedVamana<vector<float>, float>(*G, 2, 2, 2);   

            }
            
            // Create two .bin files with the Graph and the Medoid Map
            G->save_graph_to_bin(prefix);
            save_medoid_map_to_bin(prefix, MedoidMap);
            
            
        } else {
            // The .bin Medoid Map file already exists, so we read it
            get_medoid_map_from_bin(prefix, MedoidMap);
        }

        cout << "Graph has " << G->get_edge_count() << " edges\n\n";
        
        
        
        results_Filtered_Greedy(G, k, L, groundtruth, queries, queries_filters);


        delete(G);
        
    }
        

    
    return 0;
}




void results_Greedy(Graph<vector<float>> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<vector<float>>& queries, vector<float> medoid) {
    // Initialize iterators
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

        set<gIndex> X;
        pair<set<gIndex>, set<gIndex>> res;
    
        // Run GreedySearch to get the k nearest neighbors
        res = GreedySearch(*G, medoid, q, k, L);
        X = res.first;
        
        // Get ground truth
        set<gIndex> T( (*m).begin(), (*m).begin() + k);
        
        // Calculate intersection for recall
        set<gIndex> V_intersec;
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
    cout << "\nTotal Recall is " << total_recall * 100 <<"%\n" << endl;

}


void results_Filtered_Greedy(FilterGraph<vector<float>, float> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<vector<float>>& queries, vector<float>& queries_filters) {

    // Initialize iterators
    auto m = groundtruth.begin();
    auto n = queries.begin();
    auto f = queries_filters.begin();

    // Calculate and print recall for given queries
    float total_recall = 0.0f;
    int count = 0;
    while ( m != groundtruth.end() && n != queries.end() && f != queries_filters.end()) {
        // For each query
        vector<float> q = *n;
        
        cout << "\nQuery: ";
        print_vector<float>(q);
        cout << "\n";

        set<gIndex> X;
        pair<set<gIndex>, set<gIndex>> res;


        // res = FilteredGreedySearch(*G, q, k, L, Fq);
        X = res.first;



        // Get ground truth
        set<gIndex> T( (*m).begin(), (*m).begin() + k);
        
        // Calculate intersection for recall
        set<gIndex> V_intersec;
        set_intersection(X.begin(), X.end(), T.begin(), T.end(), inserter(V_intersec, V_intersec.begin()));

        cout << "\nIntersection size is:" << V_intersec.size();
        cout << "\nT size is:" << T.size();
        
        // Calculate recall and add to total
        float recall = V_intersec.size() / ((float) X.size());
        cout << "\nRecall is: " << recall << endl;
        total_recall += recall;
        count++;

        // Calculate speciificity and print
        double specificity = G->get_filter_count(*f) / G->get_vertices_count();
        cout << "Specificity is: " << specificity << endl;

        m++;
        n++;
        f++;
    }
    
    total_recall = total_recall / count;
    cout << "\nTotal Recall is " << total_recall * 100 <<"%\n" << endl;

}