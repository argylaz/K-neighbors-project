#include "../lib/acutest.h"			// Unit testing library
#include "../lib/utils.hpp"
#include <algorithm>

using namespace std;

#define NUM 1000

/* Testing graph construction */
void test_create() {
    TestGraph<int> G1(true); // Create a directed graph
    TestGraph<int> G2(false); // Create an undirected graph

    // Testing that the adjacency list is initialised properly
    auto L1 = G1.get_private_adjacency_list();
    auto L2 = G2.get_private_adjacency_list();
    TEST_ASSERT(L1.size() == 0);
    TEST_ASSERT(L2.size() == 0);

    // Testing that the set is empty
    auto V1 = G1.get_vertices();
    auto V2 = G2.get_vertices();
    TEST_ASSERT(V1.empty() == true);
    TEST_ASSERT(V1.empty() == true); set<int> A;
}

/* Testing add_vertex method */
void test_add_vertex() {
    
    TestGraph<int> G1;

    // Add a vertex to each graph
    for (int i = 0; i < NUM; i++) {
        TEST_ASSERT(G1.add_vertex(i) == true);
    }

    // Get vertex set
    set<int> V1 = G1.get_vertices();

    // Get adjacency lists
    vector<vector<int>> L1 = G1.get_private_adjacency_list();

    // Check that the vertices have been added and everthing works as it should
    TEST_ASSERT(L1.size() == NUM);
    
    set<int>::iterator it = V1.begin();
    for (int i = 0; i < NUM; i++) {
        TEST_ASSERT(*it == i);              // Set is ordered in asacending order by deault
        TEST_ASSERT(L1[i][0] == i);         // The first element of each row is the vertex corresponding to the index
        it++;
    }

    // Test what happens when adding duplicates
    int countBefore = G1.get_vertices_count();
    TEST_ASSERT(G1.add_vertex(0) == false);
    TEST_ASSERT(G1.get_vertices_count() == countBefore);
}

/* Testing insert_sorted method */
void test_insert_sorted() {
    TestGraph<int> G;

    // Adding 10 vertices with v_index[v] = v
    for (int i = 0; i < 10; i++) { 
        G.add_vertex(i);
    }

    vector<int> v = {3, 4, 8};
    G.insert_sorted(v, 2);
    G.insert_sorted(v, 4);
    G.insert_sorted(v, 7);
    G.insert_sorted(v, 9);

    vector<int> test_v = {2, 3, 4, 4, 7, 8, 9};

    TEST_ASSERT(v == test_v);
}

/* Testing insert_sorted_in_adj_list method */
void test_insert_sorted_in_adj_list() {
    TestGraph<int> G;

    // Adding 10 vertices with v_index[v] = v
    for (int i = 0; i < 10; i++) { 
        G.add_vertex(i);
    }

    // Inserting at (the copy of) the adjacency list of vertex 5
    vector<int> adj = G.get_private_adjacency_list()[5];

    G.insert_sorted_in_adj_list(adj, 1);
    G.insert_sorted_in_adj_list(adj, 7);
    G.insert_sorted_in_adj_list(adj, 5);
    G.insert_sorted_in_adj_list(adj, 4);
    
    vector<int> test_adj = {5, 1, 4, 5, 7};

    TEST_ASSERT(adj == test_adj);

}

/* Testing add_edge method */
void test_add_edge() {
    // Testing for one directed and one undirected graph
    TestGraph<int> G1(true);
    TestGraph<int> G2(false);

    // Add NUM vertices and NUM edges and test that they have been added to both graphs as they should
    G1.add_vertex(0);
    G2.add_vertex(0);

    // Check that the function returns false when trying to add edge to non existing vertices
    // G1 and G2 don't have the vertex 1
    TEST_ASSERT(G1.add_edge(0, 1) == false && G2.add_edge(0, 1) == false);
    // G1 and G2 don't have neither vertex 1 nor vertex 2
    TEST_ASSERT(G1.add_edge(2, 1) == false && G2.add_edge(2, 1) == false);


    for (int i = 1; i < NUM; i++) {
        G1.add_vertex(i);
        G2.add_vertex(i);

        TEST_ASSERT(G1.add_edge(i-1, i) == true);
        TEST_ASSERT(G2.add_edge(i-1, i) == true);

        // Get adjacency lists on each loop to get the new one
        vector<vector<int>> L1 = G1.get_private_adjacency_list();
        vector<vector<int>> L2 = G2.get_private_adjacency_list();

        // Check that the adjacency list has the correct entries
        TEST_ASSERT(L1[i-1][1] == i);  
        TEST_ASSERT(L2[i][1] == i-1);
        
        if( i != 1 ) {
            TEST_ASSERT(L2[i-1][2] == i);
            TEST_ASSERT(L1[i-1].size() == 2);
            TEST_ASSERT(L2[i-1].size() == 3); // Undirected graph has entries for both directions
        }
    }

    vector<vector<int>> L1 = G1.get_private_adjacency_list();
    vector<vector<int>> L2 = G2.get_private_adjacency_list();

    TEST_ASSERT(L1.size() == NUM);
    TEST_ASSERT(L2.size() == NUM);

    
    // Testing the addition of existing edge. add_edge() should return true, but edge count should remain the same
    TestGraph<int> G;
    G.add_vertex(0); G.add_vertex(1);
    
    TEST_ASSERT(G.add_edge(0, 1) == true);
    int edge_count_1 = G.get_edge_count();
    bool ok = G.add_edge(0, 1);
    int edge_count_2 = G.get_edge_count();
    TEST_ASSERT(ok && edge_count_1 == edge_count_2);

    /*
    Printing for testing purposes
    G1.print_graph();
    G2.print_graph();
    */
}

void test_exist_edge() {
    // Testing for one directed and one undirected graph
    TestGraph<int> G1(true);
    TestGraph<int> G2(false);
    
    G1.add_vertex(1);
    G1.add_vertex(3);
    G1.add_vertex(5);
    
    G2.add_vertex(2);
    G2.add_vertex(4);
    
    // Add some random edges
    G1.add_edge(1,3);
    G2.add_edge(2,4);

    // Test that the method finds all the edges
    TEST_ASSERT(G1.exist_edge(1,3));
    TEST_ASSERT(G2.exist_edge(2,4) && G2.exist_edge(4,2));

    // Test that it returns false for all other edges
    TEST_ASSERT(G1.exist_edge(1,5) == false && G1.exist_edge(1,2) == false);
}

/* Terting remove_edge method */
void test_remove_edge() {
    // Testing for one directed and one undirected graph
    TestGraph<int> G1(true);
    TestGraph<int> G2(false);


    // Add NUM vertices and NUM edges and test theyre have been added to both graphs as they should
    G1.add_vertex(0);
    G2.add_vertex(0);

    // Check that the function returns false when trying to remove edge from non existing vertices
    // G1 and G2 don't have the vertex 1
    TEST_ASSERT(G1.remove_edge(0,1) == false && G2.remove_edge(0,1) == false);
    // G1 and G2 don't have neither vertex 1 nor vertex 2
    TEST_ASSERT(G1.remove_edge(2,1) == false && G2.remove_edge(2,1) == false);


    for (int i = 1; i < NUM; i++) {
        G1.add_vertex(i);
        G2.add_vertex(i);

        G1.add_edge(i-1, i);
        G2.add_edge(i-1, i);
    }


    for (int i = 1; i < NUM; i++) {
        TEST_ASSERT(G1.remove_edge(i-1, i) ==  true);
        TEST_ASSERT(G2.remove_edge(i-1, i) ==  true);
        

        // Get adjacency lists
        vector<vector<int>> L1 = G1.get_private_adjacency_list();
        vector<vector<int>> L2 = G2.get_private_adjacency_list();


        // Test that the equivalent entries have been deleted
        TEST_ASSERT(L1[i-1].size() == 1);
        TEST_ASSERT(L2[i-1].size() == 1);
        if( i!= NUM - 1 ){
            TEST_ASSERT(L1[i].size() == 2);
            TEST_ASSERT(L2[i].size() == 2);   // Edge from i to i+1 hasnt been reoved yet
        }
    }
}

/* Testing index to vertex method */
void test_index_to_vertex() {
    TestGraph<int> G1;

    // Add NUM vertices
    for (int i = 0; i < NUM; i++) {
        G1.add_vertex(i);
    }

    for (int i = 0; i < NUM; i++) {
        TEST_ASSERT(G1.get_index_from_vertex(i) == i); // In this case vertices and indices are all ints
    }
}

/* Testing vertex-->index method */
void test_vertex_to_index() {
    TestGraph<int> G1;

    // Add NUM vertices
    for (int i = 0; i < NUM; i++) { 
        G1.add_vertex(i);
    }

    for (int i = 0; i < NUM; i++) {
        TEST_ASSERT(G1.get_vertex_from_index(i) == i); // In this case vertices and indices are all ints
    }
}

/* Testing get edge count method */
void test_get_edge_count() {
    // Testing for one directed and one undirected graph
    TestGraph<int> G1(true);
    TestGraph<int> G2(false);

    TEST_ASSERT(G1.get_edge_count() == 0);
    TEST_ASSERT(G2.get_edge_count() == 0);

    // Add NUM vertices and NUM edges and test edge count
    G1.add_vertex(0);
    G2.add_vertex(0);
    for (int i = 1; i < NUM; i++) { 
        G1.add_vertex(i);
        G1.add_edge(i-1, i);

        G2.add_vertex(i);
        G2.add_edge(i-1, i);

        TEST_ASSERT(G1.get_edge_count() == i);
        TEST_ASSERT(G2.get_edge_count() == 2 * i);  // An undirected edge counts as 2 (???)
    }
}

/* Testing the get_vertices_count method*/
void test_get_vertices_count() {
    TestGraph<int> G1;

    TEST_ASSERT(G1.get_vertices_count() == 0);

    // Add NUM vertices and test the vertex count
    for (int i = 1; i <= NUM; i++) { 
        G1.add_vertex(i);

        TEST_ASSERT(G1.get_vertices_count() == i);
    }
}

/* Testing the get_neighbors method */
void test_get_neighbors() {
    // Create an example graph with certain neghbours
    Graph<int> G;
    for (int i = 0; i < 10; i++) G.add_vertex(i);

    G.add_edge(1,3);
    G.add_edge(1,7);
    G.add_edge(1,2);
    

    G.add_edge(2,3);
    G.add_edge(2,4);
    G.add_edge(2,5);
    G.add_edge(2,6);

    G.add_edge(7,1);
    G.add_edge(7,2);

    G.add_edge(9,5);

    vector<gIndex> S1 = G.get_neighbors(1);
    TEST_ASSERT(find(S1.begin(), S1.end(), 3) != S1.end() && find(S1.begin(), S1.end(), 7) != S1.end() && find(S1.begin(), S1.end(), 2) != S1.end() && S1.size() == 3);

    vector<gIndex> S2 = G.get_neighbors(2);
    TEST_ASSERT(find(S2.begin(), S2.end(), 3) != S2.end() && find(S2.begin(), S2.end(), 4) != S2.end() && find(S2.begin(), S2.end(), 5) != S2.end() && find(S2.begin(), S2.end(), 6) != S2.end() && S2.size() == 4);

    vector<gIndex> S3 = G.get_neighbors(7);
    TEST_ASSERT(find(S3.begin(), S3.end(), 1) != S3.end() && find(S3.begin(), S3.end(), 2) != S3.end() && S3.size() == 2);

    vector<gIndex> S4 = G.get_neighbors(9);
    TEST_ASSERT(find(S4.begin(), S4.end(), 5) != S4.end() && S4.size() == 1);

}


    
void test_graph_bin() {
    Graph<int> G1, G2;
    for (int i = 0; i < 4; i++) {
        G1.add_vertex(i);
        G2.add_vertex(i);
    }

    G1.add_edge(1, 2);
    G1.add_edge(1, 0);
    G1.add_edge(2, 3);
    G1.add_edge(3, 1);
    G1.add_edge(0, 3);
    
    TEST_ASSERT(G1.save_graph_to_bin("test"));

    int dummy;
    TEST_ASSERT(G2.get_graph_from_bin("test", dummy));

    TEST_ASSERT(G2.get_edge_count() == 5);

    TEST_ASSERT(G2.exist_edge(1, 2));
    TEST_ASSERT(G2.exist_edge(1, 0));
    TEST_ASSERT(G2.exist_edge(2, 3));
    TEST_ASSERT(G2.exist_edge(3, 1));
    TEST_ASSERT(G2.exist_edge(0, 3));
}

// List of all tests to be executed
TEST_LIST = {
    { "graph_create", test_create },
    { "add_vertex", test_add_vertex },
    { "insert_sorted", test_insert_sorted},
    { "insert_sorted_in_adj_list", test_insert_sorted_in_adj_list},
    { "add_edge", test_add_edge },
    { "exist_edge", test_exist_edge },
    { "remove_edge", test_remove_edge },
    { "index_to_vertex", test_index_to_vertex },
    { "vertex_to_index", test_vertex_to_index },
    { "add_edge", test_add_edge },
    { "get_edge_count", test_get_edge_count},
    { "get_vertices_count", test_get_vertices_count },
    { "get_neighbors", test_get_neighbors},
    { "test_graph_bin", test_graph_bin },
    { NULL, NULL }
};