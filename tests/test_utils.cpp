#include "../lib/utils.hpp"			// Unit testing library
#include "../lib/acutest.h"


/* Testing the Euclidean Distance Function */

void test_Euclidean_Distance() {

    /* Testing Integers*/

    vector<int> v1,v2;
    float error = 1.23e-4;

    // Euclidian distances of the examples calculated on an online calculator (https://www.omnicalculator.com/math/euclidean-distance)
    float results[] = {1, 1.414214, 1.73205, 2};

    for(int i = 1 ; i < 8 ; i += 2) {  // Testing for a few different vectors up to 4D
        v1.push_back(i);
        v2.push_back(i+1);
        TEST_ASSERT(abs(Euclidean_Distance(v1,v2) - results[(i-1)/2]) <= error);
    }

    /* Testing Float*/

    vector<float> v3,v4;

    // Euclidian distances of the examples calculated on an online calculator (https://www.omnicalculator.com/math/euclidean-distance)
    float resultsf[] = {1.33f, 6.23354f, 7.16365f, 7.5147f};

    v3.push_back(1.23f);
    v4.push_back(2.56f);
    cout << Euclidean_Distance(v3,v4);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[0]) <= error);

    
    v3.push_back(0.92f);
    v4.push_back(7.01f);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[1]) <= error);

    
    v3.push_back(5.28f);
    v4.push_back(1.75f);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[2]) <= error);

    
    v3.push_back(4.76f);
    v4.push_back(2.49f);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[3]) <= error);
}

/* Testing fvec_to_graph */
void test_fvec_to_graph() {
    // Create an fvec file from the following vectors for testing
    vector<vector<float>> vectors = {
        {1.1f, 2.2f, 3.3f},
        {4.4f, 5.5f, 6.6f},
        {7.7f, 8.8f, 9.9f}
    };

    string filename = "../sift/test.fvec"; // Full path to the dataset folder
    make_fvec(filename, vectors);

    cout << "File " << filename << " has been created/filled for testing.";

    // Then we create the graph and test it was created properly
    TestGraph<vector<float>> G;
    cout << "Before fvec_to_graph" << endl;
    fvec_to_graph(filename, G);
    cout << "After fvec_to_graph" << endl;

    TEST_ASSERT(G.get_vertices_count() == 3);   // Graph must have 3 vertices (one for each vector)
    TEST_ASSERT(G.get_edge_count() == 0);       // And 0 edges 
    TEST_ASSERT(G.is_directed() == true);       // All the graphs for our prject are directed
    
    // Test that the deata is in the graph the way it should be
    set<vector<float>> vertices = G.get_private_vertex_set();
    auto v = vertices.begin();
    TEST_ASSERT(v->at(0) == 1.1f && v->at(1) == 2.2f && v->at(2) == 3.3f); v++;
    TEST_ASSERT(v->at(0) == 4.4f && v->at(1) == 5.5f && v->at(2) == 6.6f); v++;
    TEST_ASSERT(v->at(0) == 7.7f && v->at(1) == 8.8f && v->at(2) == 9.9f); v++;

    // Test that the adjacency list is as it should be
    vector<vector<vector<float>>> L = G.get_private_adjacency_list();
    auto l = L.begin();
    TEST_ASSERT(l->at(0).at(0) == 1.1f && l->at(0).at(1) == 2.2f && l->at(0).at(2) == 3.3f && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 4.4f && l->at(1).at(1) == 5.5f && l->at(1).at(2) == 6.6f && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 7.7f && l->at(2).at(1) == 8.8f && l->at(2).at(2) == 9.9f && l->size() == 1); l++;
}

/* Testing ivec_to_graph */
void test_ivec_to_graph() {
    // Create an fvec file from the following vectors for testing
    vector<vector<int>> vectors = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    string filename = "../sift/test.ivec"; // Full path to the dataset folder
    make_ivec(filename, vectors);

    cout << "File " << filename << " has been created for testing.";

    // Then we create the graph and test it was created properlys
    TestGraph<vector<int>> G;
    ivec_to_graph(filename, G);

    TEST_ASSERT(G.get_vertices_count() == 3);   // Graph must have 3 vertices (one for each vector)
    TEST_ASSERT(G.get_edge_count() == 0);       // And 0 edges 
    TEST_ASSERT(G.is_directed() == true);       // All the graphs for our prject are directed
    
    // Test that the deata is in the graph the way it should be
    set<vector<int>> vertices = G.get_private_vertex_set();
    auto v = vertices.begin();
    TEST_ASSERT(v->at(0) == 1 && v->at(1) == 2 && v->at(2) == 3); v++;
    TEST_ASSERT(v->at(0) == 4 && v->at(1) == 5 && v->at(2) == 6); v++;
    TEST_ASSERT(v->at(0) == 7 && v->at(1) == 8 && v->at(2) == 9); v++;

    // Test that the adjacency list is as it should be
    vector<vector<vector<int>>> L = G.get_private_adjacency_list();
    auto l = L.begin();
    TEST_ASSERT(l->at(0).at(0) == 1 && l->at(0).at(1) == 2 && l->at(0).at(2) == 3 && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 4 && l->at(1).at(1) == 5 && l->at(1).at(2) == 6 && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 7 && l->at(2).at(1) == 8 && l->at(2).at(2) == 9 && l->size() == 1); l++;
}


// List of all tests to be executed
TEST_LIST = {
    { "Euclidean Distance", test_Euclidean_Distance },
    { "fvec to graph", test_fvec_to_graph },
    { "ivec to graph", test_ivec_to_graph },
    { NULL, NULL }
};
