#include "../headers/acutest.h"			// Unit testing library
#include "../headers/graph.hpp"

/* Creating a testable graph that inherits from Graph to access protected members */
class TestGraph: public Graph<int> {  // Graph<int> for testing
public:
    TestGraph(bool isDirected=true) : Graph<int>(isDirected) {}

    vector<vector<int>> get_private_adjacency_list() const {
        return this->get_adjacency_list();
    }

    set<int> get_private_vertex_set() const{
        return this->get_vertices();
    }
};

/* Testing graph construction */
void test_create() {
    TestGraph G1(true); // Create a directed graph
    TestGraph G2(false); // Create an undirected graph

    // Testing that the adjacency list is initialised properly
    auto L1 = G1.get_private_adjacency_list();
    auto L2 = G2.get_private_adjacency_list();
    TEST_ASSERT(L1.size() == 0);
    TEST_ASSERT(L2.size() == 0);

    // Testing that the set is empty
    auto V1 = G1.get_private_vertex_set();
    auto V2 = G2.get_private_vertex_set();
    TEST_ASSERT(V1.empty() == true);
    TEST_ASSERT(V1.empty() == true); set<int> A;
}

/* Testing add_vertex method */
void test_add_vertex() {
    TestGraph G1;

    // Get vertex set
    set<int> V1 = G1.get_private_vertex_set();

    // Get adjacency lists
    vector<vector<int>> L1 = G1.get_private_adjacency_list();

    // Add a vertex to each graph
    for (int i = 0; i < 1000; i++) {
        G1.add_vertex(i);
    }

    // Check that the vertices have been added and everthing works as it should
    TEST_ASSERT(L1.size() == 1000);
    
    set<int>::iterator it = V1.begin();
    for (int i = 0; i < 1000; i++) {
        TEST_ASSERT(*it == i);   // Set is ordered in asacending order by deault
        TEST_ASSERT(L1.at(i).at(i) == i);

        it++;
    }
}

/* Testing add_edge method */
void test_add_edge() {
    // Testing for one directed and one undirected graph
    TestGraph G1(true);
    TestGraph G2(false);

    // Add 1000 vertices and 100 edges and test theyre have been added to both graphs as they should
    G1.add_vertex(0);
    G2.add_vertex(0);

    for (int i = 1; i < 1000; i++) {
        G1.add_vertex(i);
        G2.add_vertex(i);

        G1.add_edge(i-1,i);
        G2.add_edge(i-1,i);

        // Get adjacency lists on each loop to get the new one
        vector<vector<int>> L1 = G1.get_private_adjacency_list();
        vector<vector<int>> L2 = G2.get_private_adjacency_list();

        // Check that the adjacency list has the correct entries
        TEST_ASSERT(L1.at(i).at(1) == i-1);  
        TEST_ASSERT(L2.at(i).at(1) == i-1);
        TEST_ASSERT(L2.at(i-1).at(2) == i);

        TEST_ASSERT(L1.at(i).size() == 2);
        TEST_ASSERT(L2.at(i).size() == 3); // Undirected graph has entries for both directions
    }

    vector<vector<int>> L1 = G1.get_private_adjacency_list();
    vector<vector<int>> L2 = G2.get_private_adjacency_list();

    TEST_ASSERT(L1.size() == 1000);
    TEST_ASSERT(L2.size() == 1000);
}

/* Terting remove_edge method */
void test_remove_edge() {
    // Testing for one directed and one undirected graph
    TestGraph G1(true);
    TestGraph G2(false);

    // Get vertex set
    set<int> V1 = G1.get_private_vertex_set();
    set<int> V2 = G2.get_private_vertex_set();


    // Add 1000 vertices and 100 edges and test theyre have been added to both graphs as they should
    G1.add_vertex(0);
    G2.add_vertex(0);

    for (int i = 1; i < 1000; i++) {
        G1.add_vertex(i);
        G2.add_vertex(i);

        G1.add_edge(i-1,i);
        G2.add_edge(i-1,i);
    }

    for (int i = 1; i < 1000; i++) {
        G1.remove_edge(i-1, i);
        G2.remove_edge(i-1, i);

        // Get adjacency lists
        vector<vector<int>> L1 = G1.get_private_adjacency_list();
        vector<vector<int>> L2 = G2.get_private_adjacency_list();

        // Test that the equivalent entries have been deleted
        TEST_ASSERT(L1.at(i-1).size() == 1);
        TEST_ASSERT(L1.at(i).size() == 1);
        TEST_ASSERT(L2.at(i-1).size() == 1);
        TEST_ASSERT(L2.at(i).size() == 2);   // Edge from i to i+1 hasnt been reoved yet
    }
}

/* Testing index to vertex method */
void test_index_to_vertex() {
    TestGraph G1;

    // Add 1000 vertices
    for (int i = 0; i < 1000; i++) {
        G1.add_vertex(i);
    }

    for (int i = 0; i < 1000; i++) {
        TEST_ASSERT(G1.get_index_from_vertex(i) == i); // In this case vertices and indices are all ints
    }
}

/* Testing vertex-->index method */
void test_vertex_to_index() {
    TestGraph G1;

    // Add 1000 vertices
    for (int i = 0; i < 1000; i++) { 
        G1.add_vertex(i);
    }

    for (int i = 0; i < 1000; i++) {
        TEST_ASSERT(G1.get_vertex_from_index(i) == i); // In this case vertices and indices are all ints
    }
}

/* Testing get edge count method */
void test_get_edge_count() {
    // Testing for one directed and one undirected graph
    TestGraph G1(true);
    TestGraph G2(false);

    TEST_ASSERT(G1.get_edge_count() == 0);
    TEST_ASSERT(G2.get_edge_count() == 0);

    // Add 1000 vertices and 1000 edges and test edge count
    G1.add_vertex(0);
    G2.add_vertex(0);
    for (int i = 1; i < 1000; i++) { 
        G1.add_vertex(i);
        G1.add_edge(i-1, i);

        G2.add_vertex(i);
        G2.add_edge(i-1, i);

        TEST_ASSERT(G1.get_edge_count() == i);
        TEST_ASSERT(G2.get_edge_count() == 2*i);  // An undirected edge counts as 2 (???)
    }
}

/* Testing the get_vertices_count method*/
void test_get_vertices_count() {
    TestGraph G1;

    TEST_ASSERT(G1.get_vertices_count() == 0);

    // Add 1000 vertices and test the vertex count
    for (int i = 0; i < 1000; i++) { 
        G1.add_vertex(i);

        TEST_ASSERT(G1.get_vertices_count() == i);
    }
}


// List of all tests to be executed
TEST_LIST = {
    { "graph_create", test_create },
    { "add_vertex", test_add_vertex },
    { "add_edge", test_add_edge },
    { "remove_edge", test_remove_edge },
    { "index_to_vertex", test_index_to_vertex },
    { "vertex_to_index", test_vertex_to_index },
    { "add_edge", test_add_edge },
    { "get_vertices_count", test_get_vertices_count },
    { NULL, NULL }
};