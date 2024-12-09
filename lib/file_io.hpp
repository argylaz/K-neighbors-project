/* This file contains all the reading/writing from files used within the project*/
// #include <iostream>
// #include <bits/stdc++.h>
#include "utils.hpp"

#define MAX_ARGS 15

using namespace std;



template <typename type>
/* Given an empty graph, reads an fvec file and fills the graph with all the vectors read as its vertices and no edges */
/* This functions assumes that all the vectors given in the file will be of the same dimension                         */
/* Otherwise, the entries inside the graph will be uneven (vectors of different dimensions)                            */
void vec_to_graph(const string& filename, Graph<vector<type>>& G);


template <typename Type>
/* Method that reads vectors from an fvec or ivec and returns a vector containing all of them */
vector<vector<Type>> read_vecs(string& filename);


template <typename Type>
/* Same as the above but returns a set containing all the vectors instead */
set<vector<Type>> read_sets(string& filename);


// Function that reads the command line input arguments. Returns 1 or -1
bool get_arguments(int argc, const char* argv[], int& k, int& L, float& a, int& R, int& Rstitched, string& data_set, string& base_name, string& query_name, string& groundtruth_name, string& vamana_type);


// Helper method to check file extention
bool hasBinExtension(const string& filename);


/* Reads queries from sift/dummy-queries.bin */
/* Returns a map from query nodes to their floating type filter value (-1 for no filter) */
// num_dimensions is only given for testing purposes, all the query datasets with filters have num_dimensions = 100 so that's the default value
pair<vector<vector<float>>, vector<float>> read_queries(const string& filename, int num_queries, int num_dimensions=100);  // !!! Throws exception if file extention is not .bin (call with try/catch)


// Return 1 if it succeed, else returns 0
vector<vector<gIndex>> read_groundtruth(string filename);


/* Creates an fvec file with the given vectors for testing */
template <typename type>
void make_vec(const string& filename, const vector<vector<type>>& vectors);


/* Creates a .bin file with the given data vectors that starts with the number of vectors */
template <typename T>
void make_bin(const string& filename, const vector<vector<T>> vectors);


/* Creates a .bin file with the given data vectors that starts with the number of vectors */
template <typename T>
void make_queries(const string& filename, const vector<vector<T>> vectors);


/* Method to save the medoid map of the graph in a file <prefix>_medoid_map.bin */
template <typename F>
bool save_medoid_map_to_bin(string file_prefix, map<F, gIndex> medoid_map);

/* Method to get the adjacency list of the graph from a file <prefix>_graph.bin */
template <typename F>
bool get_medoid_map_from_bin(string file_prefix, map<F, gIndex>& medoid_map);


/*--------------------------------------------METHOD DEFINITIONS----------------------------------------*/



/* Given an empty graph, reads an fvec file and fills the graph with all the vectors read as its vertices and no edges */
/* This functions assumes that all the vectors given in the file will be of the same dimension                         */
/* Otherwise, the entries inside the graph will be uneven (vectors of different dimensions)                            */
template <typename type>
void vec_to_graph(const string& filename, Graph<vector<type>>& G) {
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when opening file " << filename << endl;
        return;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while (file.peek() != EOF) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if (!file) {
            correctFormat = false; 
            break;
        }

        // Create vector to hold the values and resize to the correct dimension
        vector<type> v; v.resize(d);

        // Read the data from file
        file.read(reinterpret_cast<char*>(v.data()), d * sizeof(type));
        if (!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if (v.size() != (size_t) d) {
            correctFormat = false;
            break;
        }

        G.add_vertex(v);
    }

    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors." << endl;
    file.close();
}




/* Method that reads vectors from an fvec or ivec and returns a vector containing all of them */
template <typename Type>
vector<vector<Type>> read_vecs(string& filename) {
    
    vector<vector<Type>> result;
    
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when opening file " << filename << endl;
        return result;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while (file.peek() != EOF) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if (!file) {
            correctFormat = false; 
            break;
        }

        // Create vector to hold the values and resize to the correct dimension
        vector<Type> v; v.resize(d);

        // Read the data from file
        file.read(reinterpret_cast<char*>(v.data()), d * sizeof(Type));
        if (!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if (v.size() != (size_t) d) {
            correctFormat = false;
            break;
        }

        result.push_back(v);
    }

    // Throw error if the format of the file was not as expected
    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors." << endl;
    file.close();

    return result;
}


/* Same as the above but returns a set containing all the vectors instead */
template <typename Type>
set<vector<Type>> read_sets(string& filename) {
    
    set<vector<Type>> result;
    
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when opening file " << filename << endl;
        return result;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while (file.peek() != EOF) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if (!file) {
            correctFormat = false; 
            break;
        }

        // Create vector to hold the values and resize to the correct dimension
        vector<Type> v; v.resize(d);

        // Read the data from file
        file.read(reinterpret_cast<char*>(v.data()), d * sizeof(Type));
        if (!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if (v.size() != (size_t) d) {
            correctFormat = false;
            break;
        }

        result.push_back(v);
    }

    // Throw error if the format of the file was not as expected
    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors." << endl;
    file.close();

    return result;
}



// Function that reads the command line input arguments. Returns 1 or -1
bool get_arguments(int argc, const char* argv[], int& k, int& L, float& a, int& R, int& Rstitched, string& data_set, string& base_name, string& query_name, string& groundtruth_name, string& vamana_type) {
    // We need at least 8 arguments (Filename, vamana_type, k, L, R and maybe a and Rstitched if we use StitchedVamana)
    
    if (!(argc == MAX_ARGS || argc == MAX_ARGS - 2 || argc == MAX_ARGS - 4)) {     
        cerr << "ERROR: Malformed input at command line\n";
        return false;
    }

    // What is the max argument count, depending on whether the default argument a is given or not  
    int final_flag;
    if (argc == MAX_ARGS) {
        final_flag = MAX_ARGS - 2;
    } else if (argc == MAX_ARGS - 2) {
        final_flag = MAX_ARGS - 4;
    } else {
        final_flag = MAX_ARGS - 6;
    }

    bool arg_found = false;

    // Get v argument, the Vamana type
    // string vamana_type;
    const char *tempV;
    arg_found = false;
    for (int i = 1; i <= final_flag; i += 2) {
        if (!strcmp(argv[i], "-v")) {
            tempV = argv[i + 1];
            arg_found = true;
        }
    }
    if (!arg_found) {
        cerr << "ERROR: Should include the vamana command line argument like \"-v vamana_type\" (simple/filtered/stitched)\n";
        return -1;
    }

    vamana_type = string(tempV);
    if (vamana_type != "simple" && vamana_type != "filtered" && vamana_type != "stitched") {
        cerr << "Vamana type should be simple/filtered/stitched\n";
        return false;
    }

    
    // Get the filename
    const char* data_set_flag;      // Flag for dataset used 
    arg_found = false;
    for (int i = 1; i <= final_flag; i += 2) {
        if (!strcmp(argv[i], "-f")) {
            data_set_flag = argv[i + 1];
            arg_found = true;
        }
    } 
    if (!arg_found) {
        cerr << "ERROR: Should include the file name as command line argument like \"-f filename\"\n";
        return false;
    }

    data_set = string(data_set_flag);
    // string base_name;
    // string groundtruth_name;
    // string query_name;
    if (data_set == "small") {
        base_name =  "sift/siftsmall_base.fvecs";
        query_name = "sift/siftsmall_query.fvecs";
        groundtruth_name = "sift/siftsmall_groundtruth.ivecs";
    }
    else if (data_set == "large") {
        base_name =  "sift/sift_base.fvecs";
        query_name = "sift/sift_query.fvecs";
        groundtruth_name = "sift/sift_groundtruth.ivecs";
    }
    else if (data_set == "dummy") {
        base_name =  "sift/dummy-data.bin";
        query_name = "sift/dummy-queries.bin";
        groundtruth_name = "sift/groundtruth.bin";
    }
    else if (data_set == "contest1m") {
        base_name =  "contest-data-release-1m.bin";
        query_name = "contest-queries-release-1m.bin";
        groundtruth_name = "sift/groundtruth1m.bin";
    }
    else if (data_set == "contest10m") {
        base_name =  "contest-data-release-10m.bin";
        query_name = "contest-queries-release-10m.bin";
        groundtruth_name = "sift/groundtruth10m.bin";
    }
    else {
        cerr << "ERROR: Should include the file name as command line argument like \"-f small\" or \"-f large\"\n";
        return false;
    }


    // Get K argument
    // int k;
    const char *tempk;
    arg_found = false;
    for (int i = 1; i <= final_flag; i += 2) {
        if (!strcmp(argv[i], "-k")) {
            tempk = argv[i + 1];
            arg_found = true;
        }
    }
    if (!arg_found) {
        cerr << "ERROR: Should include the k command line argument like \"-k k_neighbors\"\n";
        return false;
    }

    k = atoi(tempk);
    if (k <= 0 || !isPositiveInteger(tempk)) {
        cerr << "ERROR: k should be a positive integer number\n";
        return false;
    }


    // Get L argument
    // int L;
    const char *tempL;
    arg_found = false;
    for (int i = 1; i <= final_flag; i += 2) {
        if (!strcmp(argv[i], "-L")) {
            tempL = argv[i + 1];
            arg_found = true;
        }
    }
    if (!arg_found) {
        cerr << "ERROR: Should include the l command line argument like \"-l l_argument\"\n";
        return false;
    }

    L = atoi(tempL);
    if (L <= 0 || !isPositiveInteger(tempL)) {
        cerr << "ERROR: L argument should be a positive integer number\n";
        return false;
    }


    // Get R argument
    // int R;
    const char *tempR;
    arg_found = false;
    for (int i = 1; i <= final_flag; i += 2) {
        if (!strcmp(argv[i], "-R")) {
            tempR = argv[i + 1];
            arg_found = true;
        }
    }
    if (!arg_found) {
        cerr << "ERROR: Should include the r command line argument like \"-r r_argument\"\n";
        return false;
    }

    R = atoi(tempR);
    if (R <= 0 || !isPositiveInteger(tempR)) {
        cerr << "ERROR: R argument should be a positive integer number\n";
        return false;
    }


    // Get optional A argument
    // float a;
    a = 1.2;
    if ((vamana_type == "stitched" && argc == MAX_ARGS) || (vamana_type != "stitched" && argc == MAX_ARGS - 2)) {
        const char *tempA;
        arg_found = false;
        for (int i = 1; i <= final_flag; i += 2) {
            if (!strcmp(argv[i], "-a")) {
                tempA = argv[i + 1];
                arg_found = true;
            }
        }
        if (!arg_found) {
            cerr << "ERROR: Should include the a command line argument like \"-a a_argument\"\n";
            return false;
        }
    
        a = atof(tempA);
        if (a <= 0 ) {
            cerr << "ERROR: a argument should be a positive number\n";
            return false;
        }
    
    }

    // Get optional Rstitched argument
    Rstitched = -1;
    if (vamana_type == "stitched") {
        const char *tempRst;
        arg_found = false;
        for (int i = 1; i <= final_flag; i += 2) {
            if (!strcmp(argv[i], "-Rst")) {
                tempRst = argv[i + 1];
                arg_found = true;
            }
        }
        if (!arg_found) {
            cerr << "ERROR: Should include the Rstitched command line argument like \"-Rst Rstitched_argument\"\n";
            return false;
        }
    
        Rstitched = atoi(tempRst);
        if (Rstitched <= 0 || !isPositiveInteger(tempRst)) {
            cerr << "ERROR: Rstitched argument should be a positive integer number\n";
            return false;
        }
    
    }

    return true;
}


// Helper method to check file extention
bool hasBinExtension(const string& filename) {
    size_t pos = filename.rfind('.');
    return pos != string::npos && filename.substr(pos) == ".bin";
}


/* Reads queries from sift/dummy-queries.bin */
/* Returns a map from query nodes to their floating type filter value (-1 for no filter) */
pair< vector<vector<float>> , vector<float>> read_queries(const string& filename, int num_queries, int num_dimensions) {
    // Check the file extention
    if(!hasBinExtension(filename)) { // !!! SHOULD BE CALLED WITH TRY AND CATCH
        throw invalid_argument("File must have a .bin extention: " + filename);
    }


    // Open file and check if it was opened properly
    ifstream ifs(filename, ios::binary);
    assert(ifs.is_open());

    cout << "Reading Queries: " << filename << endl;


    // Get number of queries
    uint32_t N;
    ifs.read((char *)&N, sizeof(uint32_t));
    cout << "# of queries: " << N << endl;


    // Initialise buffer and map
    vector<float> buff(num_dimensions + 4);
    vector<vector<float>> v;                    // Holds all the query vector data
    vector<float> f;                            // Holds all the equivalent query filter data (categorical attributes)


    // Read query data repetitively
    int i = 0;
    while (ifs.read((char *)buff.data(), (num_dimensions + 4) * sizeof(float)) && i < num_queries) {
        
        // Casting and storing query value (not used)
        // int query_value = static_cast<float>(buff[0]);

        // Casting and storing the filter (categorical attribute)
        float filter = static_cast<float>(buff[1]);
        f.push_back(filter);


        /* Ignoring timestamps (buff[2] and buff[3]) */

        // Casting query vector data to float
        vector<float> q(num_dimensions);
        for (int d = 0; d < num_dimensions; d++) {
            q[d] = static_cast<float>(buff[d + 4]);
        }

        /* ADDING ENTRY TO RETURN VECTOR */
        // v[i] = q;
        v.push_back(q);
        i++;
        cout << i << endl;
    }

    // Close file
    ifs.close();
    cout << "Finish Reading Queries" << endl;

    cout << "v " << v.size();
    cout << "f " << f.size();


    return {v, f};
}

// Return 1 if it succeed, else returns 0
vector<vector<gIndex>> read_groundtruth(string filename) {
    
    // Open file and check if it was opened properly
    ifstream groundtruth(filename, ios::binary);
    assert(groundtruth.is_open());

    cout << "Reading Data: " << filename << endl;


    // Get number of points
    uint32_t N;
    groundtruth.read((char *)&N, sizeof(uint32_t));
    cout << "# of points: " << N << endl;

    int num_dimensions = 1;
    // Initialize buffer
    vector<gIndex> buff(num_dimensions);
    
    vector<vector<gIndex>> gt_data;

    for (int i = 0 ; i < (int)N ; i++ ){

        // Read k (number of nearest neighbors for this query)
        size_t k;
        groundtruth.read(reinterpret_cast<char*>(&k), sizeof(k));

        cout << "Query " << i + 1 << ": (k neighbors found = " << k << ")"<< endl;

        // Read the vector of dimension k
        vector<gIndex> nearest_neighbor(k);
        groundtruth.read(reinterpret_cast<char*>(nearest_neighbor.data()), k * sizeof(float));

        // Print the groundtruth of the query
        cout << "Groundtruth: ";
        print_vector(nearest_neighbor);
        cout << endl;

        // Store the vector in the groundtruthData
        gt_data.push_back(nearest_neighbor);


    }

    // Close file
    groundtruth.close();
    cout << "Finish Reading Data" << endl;


    return gt_data;

}


/*-----------------------------------------------------------------------------USED FOR TESTING-----------------------------------------------------------------------------------------*/



/* Creates an fvec file with the given vectors for testing */
template <typename type>
void make_vec(const string& filename, const vector<vector<type>>& vectors) {
    // Create file and check it was created successfully
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when creating file " << filename << endl;
        return;
    }

    // Read vectors and write them in the files
    for (const auto& v: vectors) {
        int d = v.size();         // Get the dimension of the vector

        // First write the dimension as an int
        file.write(reinterpret_cast<const char*>(&d), sizeof(int));

        // Then write the data as floats
        file.write(reinterpret_cast<const char*>(v.data()), d * sizeof(type));
    }

    // Close the files
    file.close();
}


/* Creates a .bin file with the given data vectors that starts with the number of vectors */
template <typename T>
void make_bin(const string& filename, const vector<vector<T>> vectors) {
    // Create/Open file
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Failed to open file for writing: " + filename); // !!! Used with try/catch
    }

    // First we write the number of vectors as the first entry of the file
    uint32_t num_vectors = vectors.size();                                     // Assuming all vectors will have the same number of dimensions as the first one
    file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));

    // Write data from vectors to .bin file
    for (const auto& vector : vectors) {        
        
        float filter = vector[0];                                             // Filter
        file.write(reinterpret_cast<const char*>(&filter), sizeof(float));
       
        float timestamp = vector[1];                                          // Timestamp
        file.write(reinterpret_cast<const char*>(&timestamp), sizeof(float)); 

        // Write the remaining vector data (excluding the filter)             // Vector
        file.write(reinterpret_cast<const char*>(&vector[2]), (vector.size() - 2) * sizeof(T));
    }

    // Close file
    file.close();
    cout << "Data written to " << filename << " successfully.\n";
}




/* Creates a .bin file with the given data vectors that starts with the number of vectors */
template <typename T>
void make_queries(const string& filename, const vector<vector<T>> vectors) {
    // Create/Open file
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Failed to open file for writing: " + filename); // !!! Used with try/catch
    }

    // First we write the number of vectors as the first entry of the file
    uint32_t num_vectors = vectors.size();                                     // Assuming all vectors will have the same number of dimensions as the first one
    file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));

    // Write data from vectors to .bin file
    for (const auto& vector : vectors) {      
        float query_type = vector[0];                                         // Query Type
        file.write(reinterpret_cast<const char*>(&query_type), sizeof(float));  
        
        float filter = vector[1];                                             // Filter
        file.write(reinterpret_cast<const char*>(&filter), sizeof(float));
       
        float timestamp1 = vector[2];                                          // Timestamp1
        file.write(reinterpret_cast<const char*>(&timestamp1), sizeof(float)); 
        
        float timestamp2 = vector[3];                                          // Timestamp2
        file.write(reinterpret_cast<const char*>(&timestamp2), sizeof(float)); 

        // Write the remaining vector data (excluding the filter)              // Vector
        file.write(reinterpret_cast<const char*>(&(vector[4])), (vector.size() - 4) * sizeof(T));
    }

    // Close file
    file.close();
    cout << "Data written to " << filename << " successfully.\n";
}


/* Method to save the medoid map of the graph in a file <prefix>_medoid_map.bin */
template <typename F>
bool save_medoid_map_to_bin(string file_prefix, map<F, gIndex> medoid_map) {
    string filename = "./bin/" + file_prefix + "_medoid_map.bin";

    // Create/Open file filename_medoid_map.bin
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return false;
    }
    

    // For each pair of the map
    for (auto it = medoid_map.begin() ; it != medoid_map.end() ; it++ ) {
        F filter = it->first;
        gIndex starting_node = it->second;

        // Save filter
        file.write(reinterpret_cast<const char*>(&filter), sizeof(filter));
        // Save starting node
        file.write(reinterpret_cast<const char*>(&starting_node), sizeof(starting_node)); 
    }


    return true;
}




/* Method to get the medoid map of the graph from a file <prefix>_medoid_map.bin */
template <typename F>
bool get_medoid_map_from_bin(string file_prefix, map<F, gIndex>& medoid_map) {
    string filename = "./bin/" + file_prefix + "_medoid_map.bin";

    // Create/Open file filename_medoid_map.bin
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file for reading!" << endl;
        return false;
    }


    while (file) {
        F filter;
        gIndex starting_node;

        // Read the filter
        file.read(reinterpret_cast<char*>(&filter), sizeof(filter));

        // Read the starting node
        file.read(reinterpret_cast<char*>(&starting_node), sizeof(starting_node));

        medoid_map[filter] = starting_node;
    }

    return true;
}
