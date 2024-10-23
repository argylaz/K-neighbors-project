#include <iostream>
#include "lib/utils.hpp"

using namespace std;



int main() {

    string filename = "sift/sift_learn.fvecs";
    Graph<vector<float>>* G = new Graph<vector<float>>;
    vec_to_graph<float>(filename, *G);

    // string filename = "sift/siftsmall_groundtruth.ivecs";
    // Graph<vector<int>>* G = new Graph<vector<int>>;
    // vec_to_graph<int>(filename, *G);

    cout << "Vertices Count " << G->get_vertices_count() << endl;

    // G.print_graph();
    
    return 0;
}