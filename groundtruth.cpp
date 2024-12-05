#include "lib/filterGraph.hpp"
// #include "lib/utils.hpp"

using namespace std;
 

/* Run with make groundtruth */


void create_test_bin_files(){
    // Create vector of vectors with data
    vector<vector<float>> vectors = { 
        /* F  |     |Data*/
        {1.0f, 0.0f, 0.0f},                   // second entry is always 0 (timestamp that is ignored)
        {4.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 2.0f},
        {1.0f, 0.0f, 3.0f},
        {1.0f, 0.0f, 4.0f},
        {1.0f, 0.0f, 5.0f},
        {2.0f, 0.0f, 6.0f},
        {2.0f, 0.0f, 7.0f},
        {3.0f, 0.0f, 8.0f},
        {3.0f, 0.0f, 9.0f},
        {2.0f, 0.0f, 10.0f},
    };

    /* Create a .bin file with the expected format */
    try {
        make_bin("sift/test_data.bin", vectors);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
    }
      
}


/*  The data structure of the groundtruth.bin file is explained below:
    Begins with an integer num_queries (uint32_t) indicating the number of queries which covers the groundtruth
    This is followed by data for each query, stored consecutively, with each query occupying (size_t) k, which indicates how many k-nearests neighbors exist
    and then the vector by itself (vector_num_dimension) x sizeof(float32) bytes
    summing up to num_queries x [(size_t)k + (vector_num_dimension) x sizeof(float32)] bytes in total


*/

void write_to_bin(ofstream& groundtruth_file, vector<vector<float>>& k_nearests){
    // Write k, because some queries might have less than k-nearests neighbors
    size_t k = k_nearests.size();
    groundtruth_file.write(reinterpret_cast<const char*>(&k), sizeof(k));
    

    for( const auto& neighbor : k_nearests ){
        // Write the k-neighbors, the elementys of the vector
        if (!neighbor.empty()) {
            groundtruth_file.write(reinterpret_cast<const char*>(neighbor.data()), neighbor.size() * sizeof(float));
        }

    }

}


 


/* Main Program which calculates the groundtruth of a .bin file*/


// To do:: flags arguments
int main(void){
    
    // It will be replaced with the reading of the dummy_query.bin
    // -------------------------------------------------------------------------------- 
    create_test_bin_files();


    vector<vector<float>> queries_data    = { {0.0f}, {4.0f}, {10.0f}, {8.0f}};
    vector<vector<float>> queries_filters = { {1.0f}, {1.0f}, {2.0f} , {3.0f}};


    // --------------------------------------------------------------------------------
            

    // FilterGraph<vector<float>, float> G("sift/dummy-data.bin", 100, true);
    FilterGraph<vector<float>, float> G("sift/test_data.bin", 1, true);
    int k = 2;

    set<vector<float>> vertices = G.get_vertices();
    int n = G.get_vertices_count();

    // for( vector<float> v : vertices ){
    //     vector<float> filter = G.get_filters(G.get_index_from_vertex(v));
    //     cout << "Point ";
    //     print_vector(v);
    //     cout << " has filter ";
    //     print_vector(filter);
    //     cout << endl;
    // }



    // Contains the k nearest points of each point
    // vector<vector<vector<float>>> groundtruth;
    // Calculation of the groundtruth for k nearest points

    

    // Open binary file for writing
    ofstream groundtruth_file("sift/groundtruth.bin", ios::binary);
    if (!groundtruth_file) {
        cerr << "Error opening file for writing!" << endl;
        return 1;
    }



    // Write the number of queries - groudtruth
    uint32_t N = (uint32_t) queries_data.size();
    groundtruth_file.write(reinterpret_cast<const char*>(&N), sizeof(N));


    // temp vector for the calculations
    set<gIndex> temp1;
    for( vector<float> l : vertices){
        temp1.insert(G.get_index_from_vertex(l));
    }

    for( size_t i = 0 ; i < queries_data.size() ; i++ ){

        set<gIndex> temp = temp1;
        // cout << "tesmp size is " << temp.size() << endl;
        // Set with all the gIndices
        // erase the vertex itself from the search list (temp vector)
        auto p = find(temp.begin(), temp.end(), G.get_index_from_vertex(queries_data[i])); 
        if ( p != temp.end() ) {                        // Only remove if it was found within temp
            temp.erase(p);
        }

        
        // vector with the k-nearests points of the point in question
        vector<vector<float>> k_nearests;

        cout << endl << k << " nearests of ";
        print_vector(queries_data[i]);
        cout << " with filter ";
        print_vector(queries_filters[i]);
        cout << " are:\n";
        
        // k nearest points, so k iterations
        int j = 0;
        int count = 0; 
        while( count < k && j < n){
            
            // Find the nearest each time 
            vector<float> min = find_min_Euclidean<float>(G, temp, queries_data[i]); 

            // cout << "Min is ";
            // print_vector(min);
            // cout << endl;


            // The point in question is one of the k-nearest neighbors only if has the same filter as the query
            if( queries_filters[i] == G.get_filters(G.get_index_from_vertex(min)) ){
                // --------------
                print_vector(min);
                cout << " with filter ";
                print_vector(G.get_filters(G.get_index_from_vertex(min)));
                cout << endl;
                // --------------
                k_nearests.push_back(min);
                count++;
            }

            // erase the min from the temp set
            auto t = find(temp.begin(), temp.end(), G.get_index_from_vertex(min)); 
            if ( t != temp.end() ) {                        // Only remove if it was found within V
                temp.erase(t);
            }

            j++;
        }

        cout << endl;

        write_to_bin(groundtruth_file, k_nearests);


        k_nearests.clear();
    }

    // Close groundtruth.bin
    groundtruth_file.close();


    return 0;

}





