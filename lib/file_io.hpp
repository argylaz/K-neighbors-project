/* This file contains all the reading/writing from files used within the project*/
// #include <iostream>
// #include <bits/stdc++.h>
#include "utils.hpp"

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
int get_arguments(int argc, char* argv[], int& k, int& L, float& a, int& R,string& base_name, string& query_name, string& groundtruth_name);


/* Creates an fvec file with the given vectors for testing */
template <typename type>
void make_vec(const string& filename, const vector<vector<type>>& vectors);


// Helper method to check file extention
bool hasBinExtension(const string& filename);


/* Reads queries from sift/dummy-queries.bin */
/* Returns a map from query nodes to their floating type filter value (-1 for no filter) */
map<vector<float>, float> read_queries(const string& filename, int num_queries);  // !!! Throws exception if file extention is not .bin (call with try/catch)

/* Creates a .bin file with the given data vectors that starts with the number of vectors */
template <typename T>
void make_bin(const string& filename, const vector<vector<T>> vectors);


/*--------------------------------------Methods used in main.cpp-------------------------------------*/


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
int get_arguments(int argc, const char* argv[], int& k, int& L, float& a, int& R,string& base_name, string& query_name, string& groundtruth_name){
    // We need at least 8 arguments (Filename, k, L, R and maybe a)
    if (!(argc == 9 || argc == 11)) {     
        cerr << "ERROR: Malformed input at command line\n";
        return -1;
    }

    
    // Get the filename
    const char* flag_small;      // Flag for siftsmall
    if (!strcmp(argv[1], "-f")) flag_small = argv[2];
    else if (!strcmp(argv[3], "-f")) flag_small = argv[4];
    else if (!strcmp(argv[5], "-f")) flag_small = argv[6];
    else if (!strcmp(argv[7], "-f")) flag_small = argv[8];
    else if (argc == 11 && !strcmp(argv[9], "-f")) flag_small = argv[10];
    else {
        cerr << "ERROR: Should include the file name as command line argument like \"-f filename\"\n";
        return -1;
    }

    // string base_name;
    // string groundtruth_name;
    // string query_name;
    if( !strcmp(flag_small, "small") ){
        base_name =  "sift/siftsmall_base.fvecs";
        query_name = "sift/siftsmall_query.fvecs";
        groundtruth_name = "sift/siftsmall_groundtruth.ivecs";
    }
    else if( !strcmp(flag_small, "large") ){
        base_name =  "sift/sift_base.fvecs";
        query_name = "sift/sift_query.fvecs";
        groundtruth_name = "sift/sift_groundtruth.ivecs";
    }
    else{
        cerr << "ERROR: Should include the file name as command line argument like \"-f small\" or \"-f large\"\n";
        return -1;
    }

    // Get K argument
    // int k;
    const char *tempk;
    if (!strcmp(argv[1], "-k")) tempk = argv[2];
    else if (!strcmp(argv[3], "-k")) tempk = argv[4];
    else if (!strcmp(argv[5], "-k")) tempk = argv[6];
    else if (!strcmp(argv[7], "-k")) tempk = argv[8];
    else if (argc == 11 && !strcmp(argv[9], "-k")) tempk = argv[10];
    else {
        cerr << "ERROR: Should include the k command line argument like \"-k k_neighbors\"\n";
        return -1;
    }
    k = atoi(tempk);
    if (k <= 0 || !isPositiveInteger(tempk)) {
        cerr << "ERROR: Bucket capacity b should be a positive integer number\n";
        return -1;
    }

    // Get L argument
    // int L;
    const char *tempL;
    if (!strcmp(argv[1], "-l")) tempL = argv[2];
    else if (!strcmp(argv[3], "-l")) tempL = argv[4];
    else if (!strcmp(argv[5], "-l")) tempL = argv[6];
    else if (!strcmp(argv[7], "-l")) tempL = argv[8];
    else if (argc == 11 && !strcmp(argv[9], "-l")) tempL = argv[10];
    else {
        cerr << "ERROR: Should include the l command line argument like \"-l l_argument\"\n";
        return -1;
    }
    L = atoi(tempL);
    if (L <= 0 || !isPositiveInteger(tempL)) {
        cerr << "ERROR: L argument should be a positive integer number\n";
        return -1;
    }


    // Get R argument
    // int R;
    const char *tempR;
    if (!strcmp(argv[1], "-r")) tempR = argv[2];
    else if (!strcmp(argv[3], "-r")) tempR = argv[4];
    else if (!strcmp(argv[5], "-r")) tempR = argv[6];
    else if (!strcmp(argv[7], "-r")) tempR = argv[8];
    else if (argc == 11 && !strcmp(argv[9], "-r")) tempR = argv[10];
    else {
        cerr << "ERROR: Should include the r command line argument like \"-r r_argument\"\n";
        return -1;
    }
    R = atoi(tempR);
    if (R <= 0 || !isPositiveInteger(tempR)) {
        cerr << "ERROR: R argument should be a positive integer number\n";
        return -1;
    }

    // Get A argument
    // float a;
    const char *tempA;
    if (!strcmp(argv[1], "-a")) tempA = argv[2];
    else if (!strcmp(argv[3], "-a")) tempA = argv[4];
    else if (!strcmp(argv[5], "-a")) tempA = argv[6];
    else if (!strcmp(argv[7], "-a")) tempA = argv[8];
    else if (argc == 11 && !strcmp(argv[9], "-a")) tempA = argv[10];
    else if (argc == 11 ){
        cerr << "ERROR: Should include the a command line argument like \"-a a_argument\"\n";
        return -1;
    }
    if( argc == 11) {
        a = atof(tempA);
        if (a <= 0 ) {
            cerr << "ERROR: a argument should be a positive integer number\n";
            return -1;
        }
    } 

    return 1;
}


// Helper method to check file extention
bool hasBinExtension(const string& filename) {
    size_t pos = filename.rfind('.');
    return pos != string::npos && filename.substr(pos) == ".bin";
}


/* Reads queries from sift/dummy-queries.bin */
/* Returns a map from query nodes to their floating type filter value (-1 for no filter) */
map<vector<float>, float> read_queries(const string& filename, int num_queries) {
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
    cout << "# of pueries: " << N << endl;


    // Initialise buffer and map
    vector<float> buff(104);
    map<vector<float>, float> M;

    // Read query data repeatitively
    while (ifs.read((char *)buff.data(), (104) * sizeof(float))) {
        
        // Casting and storing query value (not used)
        // int query_value = static_cast<float>(buff[0]);

        // Casting and storing the filter (categorical attribute)
        float filter = static_cast<float>(buff[1]);

        /* Ignoring timestamps (buff[2] and buff[3]) */

        // Casting query vector data to float
        vector<float> q(100);
        for (int d = 0; d < 100; d++) {
            q[d] = static_cast<float>(buff[d+4]);
        }

        /* ADDING ENTRY TO MAP */
        M[q] = filter; // filter == -1 for no filter
    }


    // Close file
    ifs.close();
    cout << "Finish Reading Queries" << endl;

    return M;
}



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
