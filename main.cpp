#include <iostream>
#include <variant>
#include "lib/knn.hpp"

using namespace std;


template <typename Type>
void zoo(Graph<vector<Type>>& G, int k, int L, int R, float a=1.2){
    
    cout << G.get_vertices_count() << " points loaded\n";
    Vamana(G, L, R, a);
    cout << "Graph has " << G.get_edge_count() << " edges\n\n";
    
    // // Reading Groundtruth
    // string filename1 = "sift/siftsmall_groundtruth.ivecs";
    // Graph<vector<int>>* G1 = new Graph<vector<int>>;
    // vec_to_graph<int>(filename1, *G1);
    // set<vector<Type>> groundtruth = G1->get_vertices()  

    // // Reading Queries 
    // string filename1 = "sift/siftsmall_groundtruth.ivecs";
    // Graph<vector<int>>* G1 = new Graph<vector<int>>;
    // vec_to_graph<int>(filename1, *G1);
    // set<vector<Type>> groundtruth = G1->get_vertices()



    



    // set<vector<Type>> V_intersec;
    // set_intersection(G.get_vertices().begin(), G.get_vertices().end(), G.get_vertices().begin(), G1->get_vertices().end(), inserter(V_intersec, V_intersec.end()));

    // float recall = V_intersec.size()/G.get_vertices().size();

    // cout << "Recall@k is " << recall << endl;
    
}




int main(int argc, char* argv[]) {

    // Check that all the arguements needed have been given
    if( argc < 4 ){
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
    if( argc > 4 ){         // a is optional and set to 1.2 by default
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
    Graph<vector<float>>* G2 = new Graph<vector<float>>;
    vector<vector<float>> queries = read_vecs<float>(filename);
    
    // Reading Base
    filename = "sift/siftsmall_base.fvecs";
    Graph<vector<float>>* G = new Graph<vector<float>>;
    vec_to_graph<float>(filename, *G);
    set<vector<float>> base = G->get_vertices();


    cout << G->get_vertices_count() << " points loaded\n";
    Vamana(*G, L, R, a);
    cout << "Graph has " << G->get_edge_count() << " edges\n\n";
    

    // while(){
    //     // vector<float> q = queries.at(i);
    //     // cout << "Query: " << *q.data() << endl;
    
    //     // pair<set< vector<float>> , set<vector<float>>> res = GreedySearch(*G, G->get_vertex_from_index(0), q, k, L);
    //     // set<vector<float>> X = res.first;
    //     // set<vector<float>> T = groundtuth[i][0:100];
    // }
    
    return 0;
}