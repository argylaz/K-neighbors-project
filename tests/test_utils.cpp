#include "../lib/utils.hpp"			// Unit testing library
#include "../lib/acutest.h"

using namespace std;

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
    // cout << Euclidean_Distance(v3,v4);
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


/* Testing the find_min_Euclidean method */
void test_find_min_Euclidean() {
    // // Test the method for vector<int>
    // vector<vector<int>> v1 = {
    //     {1,2},
    //     {3,4},
    //     {5,6},
    //     {7,8},
    // };
    // set<vector<int>> S1(v1.begin(), v1.end());
    
    // vector<vector<int>> v1c = {
    //     {1,1},
    //     {3,3},
    //     {5,5},
    //     {7,7},
    // };

    // // Test that the returned element is correct
    // vector<int> min1 = find_min_Euclidean<int>(S1, v1c[0]);
    // TEST_ASSERT(min1[0] == 1 && min1[1] == 2);
    
    // vector<int> min2 = find_min_Euclidean<int>(S1, v1c[1]);
    // TEST_ASSERT(min2[0] == 3 && min2[1] == 4);

    // vector<int> min3 = find_min_Euclidean<int>(S1, v1c[2]);
    // TEST_ASSERT(min3[0] == 5 && min3[1] == 6);
       
    // vector<int> min4 = find_min_Euclidean<int>(S1, v1c[3]);
    // TEST_ASSERT(min4[0] == 7 && min4[1] == 8);


    // // Now do the same for vector<float>
    // vector<vector<float>> v2 = {
    //     {1.1f,2.2f},
    //     {3.3f,4.4f},
    //     {5.5f,6.6f},
    //     {7.7f,8.8f},
    // };
    // set<vector<float>> S2(v2.begin(), v2.end());

    // vector<vector<float>> v2c = {
    //     {1.1f,1.1f},
    //     {3.3f,3.3f},
    //     {5.5f,5.5f},
    //     {7.7f,7.7f},
    // };

    // float e = 1.23e-4;

    // // Test that the returned element is correct
    // vector<float> minf1 = find_min_Euclidean<float>(S2, v2c[0]);
    // TEST_ASSERT(minf1[0] - 1.1f <= e && minf1[1] - 2.2 <= e);
    
    // vector<float> minf2 = find_min_Euclidean<float>(S2, v2c[1]);
    // TEST_ASSERT(minf2[0] - 3.3f <= e && minf2[1] - 4.4f <= e);

    // vector<float> minf3 = find_min_Euclidean<float>(S2, v2c[2]);
    // TEST_ASSERT(minf3[0] - 5.5f <= e && minf3[1] - 6.6 <= e);

    // vector<float> minf4 = find_min_Euclidean<float>(S2, v2c[3]);
    // TEST_ASSERT(minf4[0] - 7.7f <= e && minf4[1] - 8.8f <= e); 
}

void test_retain_closest_points() {
    TestGraph<vector<int>> G;
    for (int i = 0; i < 10; i++) {
        G.add_vertex({i});
    }
    
    set<gIndex> indices1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Then check that retain_closest_neighbors only keeps the closest neighbors
    vector<int> xquery = {10};
    retain_closest_points(G, indices1, xquery, 3);

    TEST_ASSERT(indices1.size() == 3);
    TEST_ASSERT(find(indices1.begin(), indices1.end(), 7) != indices1.end());
    TEST_ASSERT(find(indices1.begin(), indices1.end(), 8) != indices1.end());
    TEST_ASSERT(find(indices1.begin(), indices1.end(), 9) != indices1.end());


    // Now run a second test for a vector that exists in the set
    set<gIndex> indices2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    xquery = {9};
    retain_closest_points(G, indices2, xquery, 3);

    TEST_ASSERT(indices2.size() == 3);
    TEST_ASSERT(find(indices2.begin(), indices2.end(), 6) != indices2.end());
    TEST_ASSERT(find(indices2.begin(), indices2.end(), 7) != indices2.end());
    TEST_ASSERT(find(indices2.begin(), indices2.end(), 8) != indices2.end());
}

void test_medoid() {
    
    Graph<vector<int>>* G1 = new Graph<vector<int>>;
    TEST_ASSERT( medoid<int>(*G1).size() == 0);

    G1->add_vertex({1,2});
    G1->add_vertex({2,1});
    G1->add_vertex({4,5});
    G1->add_vertex({3,3});
    G1->add_vertex({5,4});


    vector<int> med1 = medoid<int>(*G1);

    /* The medoid is {3,3}*/
    TEST_ASSERT( med1[0] == 3 && med1[1] == 3 );


    Graph<vector<int>>* G2 = new Graph<vector<int>>;

    G2->add_vertex({2,3});
    G2->add_vertex({3,4});
    G2->add_vertex({5,1});
    G2->add_vertex({4,5});
    G2->add_vertex({1,2});

    /* There are two possible medoids in the graph {2,3} and {3,4} */
    vector<int> med2 = medoid<int>(*G2);
    TEST_ASSERT( (med2[0] == 3 && med2[1] == 4) || (med2[0] == 2 && med2[1] == 3) );
    
    delete G1;
}

void test_rDirectional() {

    int r = 3;

    Graph<vector<int>>* G = new Graph<vector<int>>;
    G->add_vertex({1});
    G->add_vertex({2});
    G->add_vertex({4});
    G->add_vertex({3});
    G->add_vertex({5});


    rDirectional(*G, r);

    // Check the number of edges
    TEST_ASSERT( G->get_edge_count() == r * G->get_vertices_count() );

    // Checking if rDirectional works if given a non-empty Graph
    rDirectional(*G, r);
    TEST_ASSERT( G->get_edge_count() == r * G->get_vertices_count() );

    delete G;
}



/* Testing fvec_to_graph */
void test_fvec_to_graph() {
    // Create an fvec file from the following vectors for testing
    vector<vector<float>> vectors = {
        {1.1f, 2.2f, 3.3f},
        {4.4f, 5.5f, 6.6f},
        {7.7f, 8.8f, 9.9f}
    };

    string filename = "sift/test.fvec"; // Full path to the dataset folder

    make_vec<float>(filename, vectors);

    // cout << "File " << filename << " has been created/filled for testing." << endl;

    // Then we create the graph and test it was created properly
    TestGraph<vector<float>> G;

    vec_to_graph<float>(filename, G);

    TEST_ASSERT(G.get_vertices_count() == 3);   // Graph must have 3 vertices (one for each vector)
    TEST_ASSERT(G.get_edge_count() == 0);       // And 0 edges 
    TEST_ASSERT(G.is_directed() == true);       // All the graphs for our prject are directed
    
    // Test that the deata is in the graph the way it should be
    set<vector<float>> vertices = G.get_vertices();
    auto v = vertices.begin();
    TEST_ASSERT(v->at(0) == 1.1f && v->at(1) == 2.2f && v->at(2) == 3.3f); v++;
    TEST_ASSERT(v->at(0) == 4.4f && v->at(1) == 5.5f && v->at(2) == 6.6f); v++;
    TEST_ASSERT(v->at(0) == 7.7f && v->at(1) == 8.8f && v->at(2) == 9.9f); 
    
    // Test that the adjacency list is as it should be
    vector<vector<vector<float>>> L = G.get_private_adjacency_list();
        
    auto l = L.begin();
    TEST_ASSERT(l->at(0).at(0) == 1.1f && l->at(0).at(1) == 2.2f && l->at(0).at(2) == 3.3f && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 4.4f && l->at(0).at(1) == 5.5f && l->at(0).at(2) == 6.6f && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 7.7f && l->at(0).at(1) == 8.8f && l->at(0).at(2) == 9.9f && l->size() == 1);
}

/* Testing ivec_to_graph */
void test_ivec_to_graph() {
    // Create an fvec file from the following vectors for testing
    vector<vector<int>> vectors = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    string filename = "sift/test.ivec"; // Full path to the dataset folder

    make_vec<int>(filename, vectors);

    // cout << "File " << filename << " has been created for testing.";

    // Then we create the graph and test it was created properlys
    TestGraph<vector<int>> G;

    vec_to_graph<int>(filename, G);

    TEST_ASSERT(G.get_vertices_count() == 3);   // Graph must have 3 vertices (one for each vector)
    TEST_ASSERT(G.get_edge_count() == 0);       // And 0 edges 
    TEST_ASSERT(G.is_directed() == true);       // All the graphs for our prject are directed
    
    // Test that the deata is in the graph the way it should be
    set<vector<int>> vertices = G.get_vertices();
    auto v = vertices.begin();
    TEST_ASSERT(v->at(0) == 1 && v->at(1) == 2 && v->at(2) == 3); v++;
    TEST_ASSERT(v->at(0) == 4 && v->at(1) == 5 && v->at(2) == 6); v++;
    TEST_ASSERT(v->at(0) == 7 && v->at(1) == 8 && v->at(2) == 9);

    // Test that the adjacency list is as it should be
    vector<vector<vector<int>>> L = G.get_private_adjacency_list();
    auto l = L.begin();
    TEST_ASSERT(l->at(0).at(0) == 1 && l->at(0).at(1) == 2 && l->at(0).at(2) == 3 && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 4 && l->at(0).at(1) == 5 && l->at(0).at(2) == 6 && l->size() == 1); l++;
    TEST_ASSERT(l->at(0).at(0) == 7 && l->at(0).at(1) == 8 && l->at(0).at(2) == 9 && l->size() == 1);
}


// List of all tests to be executed
TEST_LIST = {
    { "Euclidean Distance", test_Euclidean_Distance },
    { "Find_min_Euclidean", test_find_min_Euclidean},
    { "Retain_closest_neighbors", test_retain_closest_points},
    { "Medoid", test_medoid},
    { "rDirectional", test_rDirectional},
    { "fvec to graph", test_fvec_to_graph },
    { "ivec to graph", test_ivec_to_graph },
    { NULL, NULL }
};
