#include <iostream>
#include <variant>
#include "lib/knn.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    // Check that all the arguements needed have been given
    if ( argc < 4 ) {
        cerr << "k, L and R arguments are needed\n";
        return 0;
    }

    // Take input arguments a, k, L, maybe a
    cout << "\nParameters";

    int k = atoi(argv[1]);
    cout << "\nk: " << k;
    int L = atoi(argv[2]);
    cout << "\nL: " << L;
    int R = atoi(argv[3]);
    cout << "\nR: " << R;
    float a;
    if ( argc > 4 ) {         // a is optional and set to 1.2 by default
        a = atof(argv[4]);
        cout << "\na: " << a ;
    }
    cout << endl;


    // Reading Groundtruth
    string filename = "sift/siftsmall_groundtruth.ivecs";
    vector<vector<int>> groundtruth = read_vecs<int>(filename);
    cout << groundtruth.size() << endl;


    // Reading Queries 
    filename = "sift/siftsmall_query.fvecs";
    vector<vector<float>> queries = read_vecs<float>(filename);
    
    // Reading Base
    filename = "sift/siftsmall_base.fvecs";
    Graph<vector<float>>* G = new Graph<vector<float>>;
    vec_to_graph<float>(filename, *G);
    set<vector<float>> base = G->get_vertices();


    cout << G->get_vertices_count() << " points loaded\n";
    Vamana<vector<float>>(*G, L, R, a); 
    cout << "Graph has " << G->get_edge_count() << " edges\n\n";

    float recall = 0; 
    int count = 0;   

    // Initialise iterators
    auto m = groundtruth.begin();
    auto n = queries.begin();

    // Calculate and print recall for given queries
    while ( m != groundtruth.end() && n != queries.end() ) {
        // For each query
        vector<float> q = *n;
        
        cout << "Query: ";
        print_vector<float>(q);
        cout << endl;

        // Run GreedySearch to get the k nearest neighbors
        pair<set< vector<float>> , set<vector<float>>> res = GreedySearch(*G, G->get_vertex_from_index(0), q, k, L);
        set<vector<float>> temp = res.first;

        set<int> X;
        for(vector<float> l : temp) {
            int index = G->get_index_from_vertex(l);
            X.insert(index);
        }

        // Get ground truth
        set<int> T( (*m).begin(), (*m).begin() + k);

        
        // Calculate intersection for recall@k
        set<int> V_intersec;
        set_intersection(X.begin(), X.end(), T.begin(), T.end(), inserter(V_intersec, V_intersec.end()));

        // Calculate recall
        float recall = V_intersec.size() / ((float) T.size());
        cout << recall << endl;

        m++;
        n++;
    }
    
    return 0;
}