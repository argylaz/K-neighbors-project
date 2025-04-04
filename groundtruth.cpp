#include "lib/filterGraph.hpp"

using namespace std;

/*  The data structure of the groundtruth.bin file is explained below:
    Begins with an integer num_queries (uint32_t) indicating the number of queries which covers the groundtruth
    This is followed by data for each query, stored consecutively, with each query occupying (size_t) k, which indicates how many k-nearests neighbors exist
    and then the vector by itself (vector_num_dimension) x sizeof(float32) bytes
    summing up to num_queries x [(size_t)k + (vector_num_dimension) x sizeof(float32)] bytes in total


*/

void write_to_bin(ofstream& groundtruth_file, vector<gIndex>& k_nearests){
    // Write k, because some queries might have less than k-nearests neighbors
    size_t k = k_nearests.size();
    groundtruth_file.write(reinterpret_cast<const char*>(&k), sizeof(k));
    
    cout << "Writing index vector of size " << k << endl; 


    for ( gIndex neighbor : k_nearests ) {
        // Write the neighbors index into the file
        groundtruth_file.write(reinterpret_cast<const char*>(&neighbor), sizeof(gIndex));
    }
    cout << "Finished writing data" << endl;
}


 


/* Main Program which calculates the groundtruth of a .bin file*/

// To do:: flags arguments
int main(void) {

    // Read queries from dummy-queries.bin
    pair<vector<vector<float>>, vector<float>> queries_data = read_queries("sift/dummy-queries.bin", 100, 100);  
    vector<vector<float>> queries = queries_data.first;
    vector<float> queries_filter = queries_data.second;


    // Create a FilterGraph with the data from dummy-data.bin
    FilterGraph<vector<float>, float> G("sift/dummy-data.bin", 100, true);

    // Get the data vectors from the graph
    set<vector<float>> vertices = G.get_vertices();
    int n = G.get_vertices_count();

    int k = 100;

    

    // Open binary file for writing
    ofstream groundtruth_file("sift/groundtruth.bin", ios::binary);
    if (!groundtruth_file) {
        cerr << "Error opening file for writing!" << endl;
        return 1;
    }


    // Write the number of queries - groudtruth
    uint32_t N = (uint32_t) queries.size();
    groundtruth_file.write(reinterpret_cast<const char*>(&N), sizeof(N));


    // Initialising temp vector for the calculations
    set<gIndex> temp1;
    for (vector<float> l : vertices) {
        temp1.insert(G.get_index_from_vertex(l));
    }

    /* Repeatitively find nearest neighbors for each query and write into groudtruth.bin */
    // for( auto it = queries_data.begin(); it != queries_data.end(); it++ ){
    for ( size_t q = 0; q < queries.size(); q++ ) {
        // Set with all the gIndices
        set<gIndex> temp = temp1;

        // Erase the vertex itself from the search list (temp vector)
        auto p = find(temp.begin(), temp.end(), G.get_index_from_vertex(queries[q])); 
        if ( p != temp.end() ) {                        // Only remove if it was found within temp
            temp.erase(p);
        }

        
        // vector with the k-nearests points of the point in question
        vector<gIndex> k_nearests;

        cout << endl << k << " nearests of ";
        print_vector(queries[q]);
        cout << endl << " with filter ";
        cout << queries_filter[q];
        cout << " are:\n";
        
        // k nearest points, so k iterations
        int j = 0;
        int count = 0; 
        while ( count < k && j < n) {
            
            // Find the nearest each time 
            vector<float> min = find_min_Euclidean<float>(G, temp, queries[q]); 

            set<float> filters_min = G.get_filters(G.get_index_from_vertex(min));
            set<float> filter_temp;
            filter_temp.insert(queries_filter[q]);

            // The point in question is one of the k-nearest neighbors only if has the same filter as the query
            if ( filter_temp == filters_min  || queries_filter[q] == -1.0f) {       // If the query has no categorical attribute (== -1) then ignore the filter
                // --------------
                // print_vector(min);
                // cout << " with filter ";
                // print_vector(G.get_filters(G.get_index_from_vertex(min)));
                // cout << endl;
                // --------------
                k_nearests.push_back(G.get_index_from_vertex(min));
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





