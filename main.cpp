#include <iostream>
#include "lib/knn.hpp"


#define NUM_DIMENSIONS 100 // The number of dimensions of each vector

using namespace std;



/* Returns the number of queries for the given query dataset ()*/
int get_num_queries(string& query_name) {
    if (query_name == "sift/dummy-queries.bin") {
        return 100;
    } else if (query_name == "sift/contest-queries-release-1m.bin") {
        return 10000;
    } else if (query_name == "sift/contest-queries-release-10m.bin") {
        return 4000000;
    }
    else return -1;  // Signifying wrong query name
}


/* Method that runs the queries with Greedy Search and calculates the recall */
template <typename T>
void results_Greedy(Graph<T> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, T medoid);


/* Method that runs the queries with Filtered Greedy Search and calculates the recall */
template <typename T, typename F>
void results_Filtered_Greedy(FilterGraph<T, F> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, vector<F>& queries_filters, map<F, gIndex>& MedoidMap);




/* Main method of the project that runs our vamana (from knn.hpp) on a graph (from graph.hpp)
   created from the databases given in fvec files */
int main(int argc, char* argv[]) {
    // Start measuring time
    auto start_time = chrono::high_resolution_clock::now();

    int k, L, R, Rstitched;
    float a = 1.2;    // Default value 
    string data_set, base_name, query_name, groundtruth_name, vamana_type, execution_direction; // get arguments figures out the correct dataset files
    if ( !get_arguments(argc, (const char**)argv, k, L, a, R, Rstitched, data_set, base_name, query_name, groundtruth_name, vamana_type, execution_direction) ) {
        return -1;
    }
    
    string prefix = vamana_type + "_" + data_set + "_" + to_string(k) + "_" + to_string(L) + "_" + to_string(R) + "_" + to_string_trimmed(a);
    if (vamana_type == "stitched") prefix += "_" + to_string(Rstitched);

    vector<vector<gIndex>> groundtruth;
    vector<vector<float>> queries;
    cout << k << " "<< L << " " << R << " " << Rstitched << " " << a /*<< " " << data_set << " " << base_name << " " << query_name << " " << groundtruth_name << " " << vamana_type*/;
    // Simple Vamana Case
    if ( vamana_type == "simple" ) {

        // Reading Base and creating Graph
        Graph<vector<float>> *G = new Graph<vector<float>>(true);
        vec_to_graph<float>(base_name, *G);

        // cout << G->get_vertices_count() << " points loaded\n";
        
        // If the .bin graph file already exists we read it and recreate the graph, otherwise we have to make the graph and save it in the .bin file
        vector<float> medoid;
        if (!(G->get_graph_from_bin(prefix, medoid))) {

            // Running Vamana indexing algorithm
            medoid = Vamana<vector<float>>(*G, L, R, a);
            G->save_graph_to_bin(prefix);
        }        
        
        // cout << "Graph has " << G->get_edge_count() << " edges\n\n";

        if (execution_direction == "run") {
            
            // Reading Groundtruth
            groundtruth = read_vecs<int>(groundtruth_name);

            // Reading Queries        
            queries = read_vecs<float>(query_name);
            
            // Running Greedy search for the queries
            results_Greedy<vector<float>>(G, k, L, groundtruth, queries, medoid);
        }
        
        delete(G);
        
    } 
    // Stitched or Filtered Vamana Case
    else {  

        // Reading Base and creating Graph
        FilterGraph<vector<float>, float> *G = new FilterGraph<vector<float>, float>(base_name, NUM_DIMENSIONS);

        // cout << G->get_vertices_count() << " points loaded\n";

        vector<float> dummy;
        map<float,gIndex> MedoidMap;
        
        // If the .bin graph file already exists we read it and recreate the graph, otherwise we have to make the graph and save it in the .bin file
        if (!(G->get_graph_from_bin(prefix, dummy))) {
            // Find Medoid Map
            MedoidMap = FindMedoid(*G, (int)(0.1 * G->get_vertices_count()) );
            
            if ( vamana_type == "filtered" ) {
                // Running Filtered Vamana indexing algorithm
                FilteredVamana<vector<float>, float>(*G, L, R, MedoidMap, a);
            }
            // Stiched Graph
            else if ( vamana_type == "stitched" ) {
                // Running Stiched Vamana indexing algorithm
                StichedVamana<vector<float>, float>(*G, L, R, Rstitched, a);   
            }
            
            // Create two .bin files with the Graph and the Medoid Map
            if( ! G->save_graph_to_bin(prefix) ){
                cerr << "ERROR: Can't save graph to _graph.bin file\n";
            }
            if ( !save_medoid_map_to_bin<float>(prefix, MedoidMap) ){
                cerr << "ERROR: Can't save Medoid Map to _medoid_map.bin file\n";
            }

        } else {
            // The .bin Medoid Map file already exists, so we read it
            if( !get_medoid_map_from_bin<float>(prefix, MedoidMap) ){
                cerr << "ERROR: Can't get Medoid Map to _medoid_map.bin file\n";
            }
            
        }

        // cout << "Medoid Map Size " << MedoidMap.size() << endl;
        // cout << "Graph has " << G->get_edge_count() << " edges\n\n";
        
        if (execution_direction == "run") {

            // Reading Groundtruth
            // Check whether the file has been provided or calculated in advance (exists in sift/ )
            if (fopen(groundtruth_name.c_str(), "r")) {
                groundtruth = read_groundtruth(groundtruth_name);
            } else {
                cerr << "The groundtruth file for the requested datasets has not been provided!" << endl;
                cerr << "Results cannot be properly evaluated!" << endl;
                cerr << "Calculate with make groundtruth or add groundtruth file to sift/.." << endl;
                return -1;
            }

            // Reading Queries
            int num_queries = get_num_queries(query_name);

            // cout << "Num Queries: " << num_queries << endl;
            auto queries_data = read_queries(query_name, num_queries, NUM_DIMENSIONS);
            queries = queries_data.first;
            vector<float> queries_filters = queries_data.second;
            // cout << "Queries Size " << queries_data.first.size() << endl;

            results_Filtered_Greedy<vector<float>, float>(G, k, L, groundtruth, queries, queries_filters, MedoidMap);
        }

        delete(G);
        
    }
        
    // End measuring time
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    
    measure_user_time();
    // Print elapsed time
    cout << " " << elapsed.count() << endl;
    
    return 0;
}






template <typename T>
void results_Greedy(Graph<T> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, T medoid) {
    
    // Calculate and print recall for given queries
    float total_recall = 0.0f;
    int count = 0;

    size_t total_queries = queries.size();

    #pragma omp parallel for reduction(+:total_recall) schedule(dynamic)
    for (size_t i = 0; i < total_queries; i++) {
        // For each query

        const vector<float>& query = queries[i];
        const vector<gIndex>& gt = groundtruth[i];

        
        // cout << "\nQuery: ";
        // print_vector<float>(query);
        // cout << "\n";

        // Run GreedySearch to get the k nearest neighbors
        pair<set<gIndex>, set<gIndex>> res = GreedySearch(*G, medoid, query, k, L);
        set<gIndex> X = res.first;
        
        // Get ground truth
        set<gIndex> T_( (gt).begin(), (gt).begin() + k);
        
        // Calculate intersection for recall
        set<gIndex> V_intersec;
        set_intersection(X.begin(), X.end(), T_.begin(), T_.end(), inserter(V_intersec, V_intersec.begin()));

        // cout << "\nIntersection size is:" << V_intersec.size();
        // cout << "\nT size is:" << T_.size();
        
        // Calculate recall and add to total
        float recall = static_cast<float>(V_intersec.size()) / ((float) X.size());
        // #pragma omp critical
        // {
        //     std::cout << "\nRecall for query " << i << " is: " << recall << std::endl;
        // }
        total_recall += recall;
        count++;

    }

    total_recall = total_recall / count;
    cout << " " <<  total_recall * 100; // Printing total recall
    

}





template <typename T, typename F>
void results_Filtered_Greedy(FilterGraph<T, F> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, vector<F>& queries_filters, map<F, gIndex>& MedoidMap) {

    size_t total_queries = queries.size();

    // cout << "Queries Size " << total_queries << endl;

    // Calculate and print recall for given queries
    float total_recall_filtered = 0.0f;
    float total_recall_unfiltered = 0.0f;
    float total_recall = 0.0f;
    int count_unfiltered = 0;
    int count_filtered = 0;

    // Parallel for
    #pragma omp parallel for reduction(+:total_recall_filtered, total_recall_unfiltered, count_filtered, count_unfiltered) schedule(dynamic)
    for( size_t i = 0 ; i < total_queries ; i++ ) {
        // For each query
        vector<float> q = queries[i];
        
        // cout << "\nQuery no." << count_filtered + count_unfiltered + 1<< endl;
        // print_vector<float>(q);
        // cout << "\n";

        set<F> Fq = {queries_filters[i]}; // even with one element

        if ( queries_filters[i] == -1.0f ) {   // Whenever query filter = -1 (no filter) 
            Fq = G->get_filters_set();
        }

        set<T> Sf_q;
        for (F filter: Fq) {
            
            if (queries_filters[i] == -1.0f ) {
                set<T> starting = {G->get_vertex_from_index(MedoidMap[filter])};
                set<F> temp_filter = {filter};
                auto res = FilteredGreedySearch(*G, starting, q, 1, L, temp_filter);
                Sf_q.insert(G->get_vertex_from_index(*(res.first.begin())));
            } else {
                Sf_q.insert(G->get_vertex_from_index(MedoidMap[filter]));
            }
        }
        
        // cout << "\nk " << k << endl;
        // cout << "L " << L << endl;
        // cout << Fq.size();

        // GreedySearch(*G, medoid, q, k, L);
        pair<set<gIndex>, set<gIndex>> res = FilteredGreedySearch(*G, Sf_q, q, k, L, Fq);
        
        set<gIndex> X = res.first;


        // Get ground truth
        set<gIndex> T_( groundtruth[i].begin(), groundtruth[i].begin() + k);
        
        // Calculate intersection for recall
        set<gIndex> V_intersec;
        set_intersection(X.begin(), X.end(), T_.begin(), T_.end(), inserter(V_intersec, V_intersec.begin()));

        // Calculate recall and add to total
        float recall = 0;     
        if( X.size() > 0 )
            recall = (float)V_intersec.size() / ((float) X.size());
        
        // cout << "\nRecall is: " << recall << endl;
        if(queries_filters[i] == -1.0f){
            total_recall_unfiltered += recall;
            count_unfiltered++;
        }
        else{
            total_recall_filtered += recall;
            count_filtered++;
        }
        // count++;

        // Calculate speciificity and print
        float specificity = (float)G->get_filter_count(queries_filters[i]) / ((float)G->get_vertices_count());
        // cout << "Specificity is: " << specificity << endl;
    }


    
    total_recall_filtered = total_recall_filtered / count_filtered;
    // cout << "\nTotal Recall for filtered is " << total_recall_filtered * 100 <<"%\n" << endl;
    // cout << "From " << count_filtered << " filtered queries" << endl;

    total_recall_unfiltered = total_recall_unfiltered / count_unfiltered;
    // cout << "\nTotal Recall for unfiltered is " << total_recall_unfiltered * 100 <<"%\n" << endl;
    // cout << "From " << count_unfiltered << " unfiltered queries" << endl;

    total_recall = (count_filtered * total_recall_filtered + count_unfiltered*total_recall_unfiltered) / (count_filtered + count_unfiltered);
    cout << " " << total_recall * 100;
    
    // cout << "Filters in the Graph\n";
    set<F> F_ = G->get_filters_set();
    // for( F f : F_){
    //     cout << f << " "<<  G->get_filter_count(f) << endl;
    //     G->get_filter_count(f);
    // }

    // cout << "edges: "<< G->get_edge_count()<< endl;



}