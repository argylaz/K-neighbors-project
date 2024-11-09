#include <iostream>
#include "lib/knn.hpp"

using namespace std;

/* Main method of the project that runs our vamana (from knn.hpp) on a graph (from graph.hpp)
   created from the databases given in fvec files */
int main(int argc, char* argv[]) {

    int k, L, R;
    float a = 2;    // Default value 
    string base_name, query_name, groundtruth_name;
    if( get_arguments(argc, argv, k, L, a, R, base_name, query_name, groundtruth_name) == -1){
        return -1;
    }
    

    // Reading Groundtruth
    vector<vector<int>> groundtruth = read_vecs<int>(groundtruth_name);
    cout << groundtruth.size() << endl;


    // Reading Queries 
    vector<vector<float>> queries = read_vecs<float>(query_name);
    

    // Reading Base and creating Graph
    Graph<vector<float>>* G = new Graph<vector<float>>;
    vec_to_graph<float>(base_name, *G);
    set<vector<float>> base = G->get_vertices();


    cout << G->get_vertices_count() << " points loaded\n";

    // Running Vamana indexing algorithm
    vector<float> medoid = Vamana<vector<float>>(*G, L, R, a); 
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
        
        cout << "Query: ";
        print_vector<float>(q);
        cout << endl;


        // Run GreedySearch to get the k nearest neighbors
        pair<set< vector<float>> , set<vector<float>>> res = GreedySearch(*G, medoid, q, k, L);
        set<vector<float>> temp = res.first;

        // Convert to set of indices
        set<int> X;
        for(vector<float> l : temp) {
            int index = G->get_index_from_vertex(l);
            X.insert(index);
        }

        // Get ground truth
        set<int> T( (*m).begin(), (*m).begin() + k);
        
        // Calculate intersection for recall
        set<int> V_intersec;
        set_intersection(X.begin(), X.end(), T.begin(), T.end(), inserter(V_intersec, V_intersec.begin()));

        cout << "\nIntersection size is:" << V_intersec.size() << endl;
        cout << "T size is:" << T.size() << endl;
        
        // Calculate recall and add to total
        float recall = V_intersec.size() / ((float) X.size());
        cout << "Recall is: " << recall << endl;
        total_recall += recall;
        count++;

        m++;
        n++;
    }
    
    total_recall = total_recall / count;
    cout << "Total Recall is " << total_recall*100 <<"%\n" << endl;
    return 0;
}