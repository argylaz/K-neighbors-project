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


/*  */
template <typename T>
void results_Greedy(Graph<T> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, T medoid);


/* */
template <typename T, typename F>
void results_Filtered_Greedy(FilterGraph<T, F> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, vector<F>& queries_filters, map<F, gIndex>& MedoidMap);



/* Main method of the project that runs our vamana (from knn.hpp) on a graph (from graph.hpp)
   created from the databases given in fvec files */
int main(int argc, char* argv[]) {

    int k, L, R, Rstitched;
    float a = 1.2;    // Default value 
    string data_set, base_name, query_name, groundtruth_name, vamana_type; // get arguments figures out the correct dataset files
    if ( !get_arguments(argc, (const char**)argv, k, L, a, R, Rstitched, data_set, base_name, query_name, groundtruth_name, vamana_type) ) {
        return -1;
    }

    cout << "1" << endl;

    
    string prefix = vamana_type + "_" + data_set + "_" + to_string(k) + "_" + to_string(L) + "_" + to_string(R);
    if (vamana_type == "stitched") prefix += "_" + to_string(Rstitched);

    vector<vector<gIndex>> groundtruth;
    vector<vector<float>> queries;
    cout << k << " "<< L << " " << R << " " << Rstitched << " " << a << " " << data_set << " " << base_name << " " << query_name << " " << groundtruth_name << " " << vamana_type<<endl;
    // Simple Vamana Case
    if ( vamana_type == "simple" ) {
        cout << "2" << endl;

        // Reading Groundtruth
        groundtruth = read_vecs<int>(groundtruth_name);
        cout << groundtruth.size() << endl;
        cout << "2.5" << endl;
        // Reading Queries        
        queries = read_vecs<float>(query_name);

        cout << "3" << endl;
        // Reading Base and creating Graph
        Graph<vector<float>> *G = new Graph<vector<float>>(true);
        vec_to_graph<float>(base_name, *G);

        cout << G->get_vertices_count() << " points loaded\n";
        cout << "4" << endl;
        // If the .bin graph file already exists we read it and recreate the graph, otherwise we have to make the graph and save it in the .bin file
        vector<float> medoid;
        if (!(G->get_graph_from_bin(prefix, medoid))) {

            // Running Vamana indexing algorithm
            medoid = Vamana<vector<float>>(*G, L, R, a);
            G->save_graph_to_bin(prefix);
        }        
        cout << "5" << endl;
        cout << "Graph has " << G->get_edge_count() << " edges\n\n"; 


        results_Greedy<vector<float>>(G, k, L, groundtruth, queries, medoid);

        
        
        delete(G);
        
    } 
    // Stitched or Filtered Vamana Case
    else {  

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
        cout << "Num Queries: " << num_queries << endl;
        pair< vector<vector<float>>, vector<float> > queries_data = read_queries(query_name, num_queries, NUM_DIMENSIONS);
        queries = queries_data.first;
        vector<float> queries_filters = queries_data.second;

        cout << "Queries Size " << queries_data.first.size() << endl;
        

        // Reading Base and creating Graph
        FilterGraph<vector<float>, float> *G = new FilterGraph<vector<float>, float>(base_name, NUM_DIMENSIONS);

        cout << G->get_vertices_count() << " points loaded\n";


        vector<float> dummy;
        map<float,gIndex> MedoidMap;
        
        // If the .bin graph file already exists we read it and recreate the graph, otherwise we have to make the graph and save it in the .bin file
        if (!(G->get_graph_from_bin(prefix, dummy))) {
            // Find Medoid Map
            MedoidMap = FindMedoid(*G, (int)floor(0.1 * G->get_vertices_count()) );
            
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
            G->save_graph_to_bin(prefix);
            save_medoid_map_to_bin(prefix, MedoidMap);
            
            
        } else {
            // The .bin Medoid Map file already exists, so we read it
            get_medoid_map_from_bin(prefix, MedoidMap);
        }

        cout << "Graph has " << G->get_edge_count() << " edges\n\n";
        
        
        results_Filtered_Greedy<vector<float>, float>(G, k, L, groundtruth, queries, queries_filters, MedoidMap);


        delete(G);
        
    }
        

    
    return 0;
}






template <typename T>
void results_Greedy(Graph<T> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, T medoid) {
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

    
        // Run GreedySearch to get the k nearest neighbors
        pair<set<gIndex>, set<gIndex>> res = GreedySearch(*G, medoid, q, k, L);
        set<gIndex> X = res.first;
        
        // Get ground truth
        set<gIndex> T_( (*m).begin(), (*m).begin() + k);
        
        // Calculate intersection for recall
        set<gIndex> V_intersec;
        set_intersection(X.begin(), X.end(), T_.begin(), T_.end(), inserter(V_intersec, V_intersec.begin()));

        cout << "\nIntersection size is:" << V_intersec.size();
        cout << "\nT size is:" << T_.size();
        
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





template <typename T, typename F>
void results_Filtered_Greedy(FilterGraph<T, F> *G, int k, int L, vector<vector<gIndex>>& groundtruth, vector<T>& queries, vector<F>& queries_filters, map<F, gIndex>& MedoidMap) {

    // Initialize iterators
    auto m = groundtruth.begin();
    auto n = queries.begin();
    auto f = queries_filters.begin();


    cout << "Queries Size " << queries.size() << endl;

    // Calculate and print recall for given queries
    float total_recall = 0.0f;
    int query_count = 1;
    int count = 0;
    while ( m != groundtruth.end() && n != queries.end() && f != queries_filters.end()) {
        // For each query
        vector<float> q = *n;
        
        cout << "\nQuery no." << query_count << " : ";
        print_vector<float>(q);
        cout << "\n";

        set<F> Fq = {*f}; // even with one element

        set<T> Sf_q;
        for (F filter: Fq) {
            Sf_q.insert(G->get_vertex_from_index(MedoidMap[filter]));
            cout << "filter Fq " << filter;
        }


        // cout << "\nSf q size " << Sf_q.size();
        cout << endl;
        print_vector(q);
        cout << "\nk " << k << endl;
        cout << "L " << L << endl;
        cout << Fq.size();

        pair<set<gIndex>, set<gIndex>> res = FilteredGreedySearch(*G, Sf_q, q, k, L, Fq);
        set<gIndex> X = res.first;



        // Get ground truth
        set<gIndex> T_( (*m).begin(), (*m).begin() + k);
        
        // Calculate intersection for recall
        set<gIndex> V_intersec;
        set_intersection(X.begin(), X.end(), T_.begin(), T_.end(), inserter(V_intersec, V_intersec.begin()));

        cout << "\nIntersection size is:" << V_intersec.size();
        cout << "\nX size is:" << X.size();
        cout << "\nT size is:" << T_.size();

        float recall = 0;     
        // Calculate recall and add to total
        
        recall = (float)V_intersec.size() / ((float) X.size());
        
        cout << "\nRecall is: " << recall << endl;
        total_recall += recall;
        count++;

        // Calculate speciificity and print
        double specificity = G->get_filter_count(*f) / G->get_vertices_count();
        cout << "Specificity is: " << specificity << endl;

        m++;
        n++;
        f++;
        query_count++;
    }
    
    total_recall = total_recall / count;
    cout << "\nTotal Recall is " << total_recall * 100 <<"%\n" << endl;

}