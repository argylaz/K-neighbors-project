#include "acutest.h"			// Unit testing library
#include "headers/graph.hpp"

/* Creating a testable graph that inherits from Graph to access protected members */
class TestGraph: public Graph<int> {  // Graph<int> for testing
public:
    auto get_private_adjacency_list() const {
        return get_adjacency_list();
    }
};

/* Testing graph construction */
void test_create() {
    TestGraph G1; // Create a directed graph
    TestGraph G2; // Create an undirected graph

    // Testing that their sizes are initialised to 0
    TEST_ASSERT(G1.get_vertices_count() == 0);
    TEST_ASSERT(G2.get_vertices_count() == 0);

    // Testing that the adjacency list is initialised properly
    auto L1 = G1.get_private_adjacency_list();
    auto L2 = G2.get_private_adjacency_list();
    TEST_ASSERT(L1.size() == 0);
    TEST_ASSERT(L2.size() == 0);
}

/* Testing add_vertex method */
void test_add_vertex() {

}

/* Testing add_edge method */
void test_add_edge() {

}

/* Testing the get_vertices_count method*/
void test_get_vertices_count() {

}


// List of all tests to be executed
TEST_LIST = {
    { "graph_create", test_create },
    { "add_vertex", test_add_vertex },
    { "add_edge", test_add_edge },
    { "get_vertices_count", test_get_vertices_count },
};