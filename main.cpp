#include <iostream>
#include "lib/utils.hpp"

using namespace std;



int main() {

    string filename = "sift/sift_learn.fvecs";
    Graph<vector<float>>* G = new Graph<vector<float>>;
    fvec_to_graph(filename, *G);

    cout << "Vertices Count " << G->get_vertices_count() << endl;

    // G.print_graph();
    
    return 0;
}