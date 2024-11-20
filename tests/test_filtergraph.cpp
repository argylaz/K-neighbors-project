#include "../lib/acutest.h"			// Unit testing library
#include "../lib/filterGraph.hpp"




void test_get_filters() {
    
    // First template is Graph data Type, the second tempalet is the Filter data Type 
    FilterGraph<vector<int>,int> G(true);
    TEST_ASSERT( G.add_vertex({10}, {1}) == true );
    TEST_ASSERT( G.add_vertex({20}, {1}) == true );
    TEST_ASSERT( G.add_vertex({30}, {2}) == true );
    TEST_ASSERT( G.add_vertex({40}, {2}) == true );
    
    vector<int> filter;
    filter = G.get_filters(G.get_index_from_vertex({10}));
    // TEST_ASSERT( filter == {1});
    filter = G.get_filters(G.get_index_from_vertex({20}));
    // TEST_ASSERT( filter == {1});
    filter = G.get_filters(G.get_index_from_vertex({30}));
    // TEST_ASSERT( filter == {2});
    filter = G.get_filters(G.get_index_from_vertex({40}));
    // TEST_ASSERT( filter == {2});


};


void test_add_vertex() {

    // First template is Graph data Type, the second tempalet is the Filter data Type 
    FilterGraph<vector<int>,char> G(true);
    TEST_ASSERT( G.add_vertex({10}, {'a'}) == true );
    TEST_ASSERT( G.add_vertex({20}, {'b'}) == true );
    TEST_ASSERT( G.add_vertex({30}, {'c'}) == true );

    TEST_ASSERT(G.get_vertices_count() == 3);

    // Empty filter vector --> return false
    TEST_ASSERT( G.add_vertex({30},{}) == false );


};


// List of all tests to be executed
TEST_LIST = {
    { "get_filters", test_get_filters },
    { "add_vertex", test_add_vertex },
    { NULL, NULL }
};