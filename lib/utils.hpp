#include <bits/stdc++.h>
#include <cmath>
#include "graph.hpp"
#include <ctype.h>
#include <string.h>



/*--------------------------------------------------Namespaces and Type aliases-----------------------------------------------------*/

using namespace std;

/* Defining a filter as a vector of pairs int(dimension) and Type(value)        */
/* These filters will be used to check certain values of a given vector(point)  */
// template <typename Type>
// using filter = <pair<int, Type>;


/*----------------------------------------------------Function Declarations---------------------------------------------------------*/



template<typename Type>
/* Function for the calculation of the Euclidean distance             */
/* Returns INFINITY as error value indicating problems in calculation */
inline float Euclidean_Distance(vector<Type> a, vector<Type> b);


template <typename Type>
/* Function that given a set S and a point xquery, finds the point p in S with the min Euclidean distance with xquery*/
inline vector<Type> find_min_Euclidean(Graph<vector<Type>>& G, set<gIndex>& S, vector<Type> xquery);


template <typename Type>
/* Given a set of vectors and a target vector (xquery), only keep the L vectors closest to xquery */
inline void retain_closest_points(Graph<vector<Type>>& G , set<gIndex> &output_set, vector<Type> xquery, int L);


template <typename Type>
/* Given a set of vectors and a target vector (xquery), only keep the L vectors closest to xquery */
inline void retain_closest_points(Graph<vector<Type>>& G , set<gIndex> &output_set, vector<Type> xquery, int L);


template <typename Type>
/* Method which finds the medoid of a given graph */
vector<Type> medoid(Graph<vector<Type>>& G);


// /*  Filter is pair<int, Type>
//     Function returns a map M, mapping filters to the equivalent medoid node
// */ 
// template <typename Type, typename F>
// map<vector<F> , gIndex> FindMedoid(Graph<vector<Type>>& G, int threshold);


template <typename T>
/* Method which adds randomly exactly R outgoing neighbors to each vertex of the graph */
void rDirectional(Graph<T>& G, int R);


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


// Function that checks if a string is a positive integer (for checking the command line arguments)
bool isPositiveInteger(char *str);


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


/*----------------------------------------------------Function Definitions-----------------------------------------------------------*/



template<typename Type>
inline float Euclidean_Distance(vector<Type> a, vector<Type> b) {
    /* Implemented with separate for loops instead of iterators so that the compiler can vectorise the operations */
    /* Vectorized operation: d = sqrt(sum(a^2 - b^2))                                                             */
    
    // If the sizes of the vectors don't match throw error and return INFINITY
    if(a.size() != b.size()) {
        cerr << "Vector sizes for Euclidean don't match!" << endl;
        return INFINITY;
    }

    // This vector will hold a^2 - b^2
    vector<Type> temp;
    temp.resize(a.size());

    // Calculate vector (a - b)^2
    for(size_t i = 0; i < a.size(); i++) {
        temp[i] = a[i] - b[i];
        temp[i] = temp[i] * temp[i];
    }

    // Find the sum of the calculated vector
    float sum = 0.0f;
    for(size_t i = 0; i < a.size(); i++) {
        sum += temp[i];
    }

    // Get the square root of the sum
    float x = sqrt(sum);               // [(i1 -j1)^2 + ... + (i_n-j_n)^2] ^ 1/2
    return x;
}



template <typename Type>
inline vector<Type> find_min_Euclidean(Graph<vector<Type>>& G, set<gIndex>& S, vector<Type> xquery) {
    
    // Find the element with the minimum Euclidean distance from xquery
    gIndex min_index = *min_element(S.begin(), S.end(), 
                        [&xquery, &G](const gIndex a, const gIndex b) {
                            return Euclidean_Distance<Type>(G.get_vertex_from_index(a), xquery) < Euclidean_Distance<Type>(G.get_vertex_from_index(b), xquery);
    });

    return G.get_vertex_from_index(min_index);
}


/* Given a set of vectors and a target vector (xquery), only keep the L vectors closest to xquery */
template <typename Type>
inline void retain_closest_points(Graph<vector<Type>>& G , set<gIndex> &output_set, vector<Type> xquery, int L) {

    // cout <<"I"<< endl;
    // // Erasing the vertex itself from the set in order to avoid mistaking it for a neighbor
    // gIndex xquery_index = G.get_index_from_vertex(xquery);
    // output_vec.erase(find(output_vec.begin(), output_vec.end(), xquery_index)); 

    output_set.erase(G.get_index_from_vertex(xquery)); 
    
    // Create vector with the elements of the set
    vector<gIndex> output_vec(output_set.begin(), output_set.end());

    // Sort the vector by comparing the Euclidean Distance of each element with xquery
    sort(output_vec.begin(), output_vec.end(), [&xquery, &G](const gIndex a, const gIndex b) {
        return Euclidean_Distance<Type>(G.get_vertex_from_index(a), xquery) < Euclidean_Distance<Type>(G.get_vertex_from_index(b), xquery);
    });

    // Resize the output to size L to only keep the L closest neighbors
    if( output_vec.size() >= (long unsigned int) L )
        output_vec.resize(L);
    else   
        cout << "L is greater than the size of the vector/set\n";

    output_set.clear();
    output_set.insert(output_vec.begin(), output_vec.end());
}


/* Method which finds the medoid of a given graph */
template <typename Type>
vector<Type> medoid(Graph<vector<Type>>& G){
    
    // Initialise variables
    float min = numeric_limits<float>::max();
    vector<Type> medoid_vertice; 

    // Check if the graph is empty
    if ( G.get_vertices_count() == 0  ) { 
        cerr << "\nThe Graph is empty. The Medoid can't be calculated\n" << endl;
        return {};  // Returns an empty vector
    }

    // Iterate through all the vertices of the graph(dataset)
    set<vector<Type>> vertices = G.get_vertices();
    for (vector<Type> vertex : vertices) {

        // Calculate the sum of the Euclidean Distances of this vertex with all the others 
        float sum = 0;
        for (vector<Type> x : vertices) {
            sum += Euclidean_Distance<Type>(vertex, x);
        }

        // Check if this sum is the minimum
        if ( sum < min ) {
            min = sum;
            medoid_vertice = vertex;
        }

    }

    // Print the medoid
    cout << "Medoid point ";  
    print_vector(medoid_vertice);
    cout << "\nFound at index " << G.get_index_from_vertex(medoid_vertice) << endl;

    return medoid_vertice;
}

// template <typename Type, typename F>
// /*  Filter is pair<int, Type>
//     Function returns a map M, mapping filters to the equivalent medoid node 
//     Argument F is the set of all filters
// */
// // We suppose that the set of all filters is given as a vector of filters(pair) 
// map<vector<F> , gIndex> FindMedoid(FilterGraph<vector<Type>, F>& G,  int threshold){
//     map<vector<F> , gIndex> M;

//     set<vector<F>> Filters = G.get_filters_set(); 
    
//     // Νομίζω ότι εξ αρχής αρχικοποιείται σαν zero map
//     // εναλλακτικά θα πρέπει να κάνουμε αρχικοποίηση σε 0 κάθε στοιχείο του map 
//     map<gIndex, int> T;               // Zero map T is intended as a counter

//     set<vector<Type>> vertices = G.get_vertices();
 
//     // For each filter in the set
//     for( vector<F> f : Filters ){

//         // contains the gIndices of all points matching filter in question
//         vector<gIndex> Pf;

//         // Find all the gIndices matching the filter f
//         for( vector<Type> v : vertices ){

//             // int dimension = F[i].first
//             // F value = F[i].second

//             vector<F> filter = G.get_filters(G.get_index_from_vertex(v));              
            
//             if( filter == f ){
//                 Pf.push_back(G.get_index_from_vertex(v));
//             }

//             // if( v[dimension] == value ){
//             //     Pf.push_back(G.get_index_from_vertex(v));
//             // }      
//         }
        

//         // To do :: Check for optimization
//         // Let Rf <- threshold randomly sampled data point ids from Pf        
//         // Create a vector with all the elements of Pf
//         vector<gIndex> temp_vector(vertices.begin(), vertices.end());

//         // To obtain a time-based seed 
//         unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        
//         // Shuffle the temp vector Pf
//         shuffle(temp_vector.begin(), temp_vector.end(), default_random_engine(seed));


//         // Keep the first threshold items of the shuffled vector
//         vector<gIndex> Rf(temp_vector.begin(), temp_vector.begin() + threshold);


//         if( Rf.size() <= 0 ){
//             cerr << "The set Rf in medoid calculation is empty";
//         }

//         // Finding p_min point, where p_min is min{T[p], for each p in Rf};
//         gIndex p_min_index;

//         p_min_index = Rf[0];
//         for( size_t i = 1 ; i < Rf.size() ; i++ ){
//             if( T[Rf[i]] < T[p_min_index] ){
//                 p_min_index = Rf[i];
//             }
//         }
        
//         M[f] = p_min_index;
//         T[p_min_index]++;

//     }


//     return M;

// }

/* Method which adds randomly exactly R outgoing neighbors to each vertex of the graph */
template <typename T>
void rDirectional(Graph<T>& G, int R) {
    
    // Check if R is larger than the number of vertices (task impossible)
    if ( R > G.get_vertices_count() ) {
        cerr << "R-Directional Graph initialization failed...\n" << endl;
        return;
    }

    // Get vertices
    set<T> vertices = G.get_vertices();

    // If the graph already has edges, remove them and construct the graph from the beginning 
    if (G.get_edge_count() > 0) {
        cerr << "\nGraph already has edges\n" << endl;
        
        for (T vertex: vertices) {
            vector<gIndex> neighbors = G.get_neighbors(vertex);
            for (gIndex neighborIndex: neighbors) {
                G.remove_edge(vertex, G.get_vertex_from_index(neighborIndex));
            }
        }
        
    }

    // For each vertex of the graph
    for ( T v : vertices ) {
        
        // Create a vector with all the elements of the graph
        vector<T> shuffled_vertices(vertices.begin(), vertices.end());

        // To obtain a time-based seed 
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        
        // Shuffle the vector 
        shuffle(shuffled_vertices.begin(), shuffled_vertices.end(), default_random_engine(seed));


        // Add R random outgoing neighbors
        int i = 0;
        int count = 0;
        while ( count < R ){
            if ( shuffled_vertices[i] != v ){
                G.add_edge(v, shuffled_vertices[i]);  
                count++;  
            }
            i++;
        }

    }

}


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



/*--------------------------------------Methods used in main.cpp-------------------------------------*/



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


// Function that checks if a string is a positive integer (for checking the command line arguments)
bool isPositiveInteger(const char *str) {
    int sz = strlen(str);
    for (int i = 0; i < sz; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
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
map<vector<float>, float> read_queries(const string& filename, int num_queries, int num_dimensions) {
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
    vector<float> buff(4 + num_dimensions);
    map<vector<float>, float> M;

    // Read query data repeatitively
    while (ifs.read((char *)buff.data(), (4 + num_dimensions) * sizeof(float))) {
        
        // Casting and storing query value (not used)
        int query_value = static_cast<float>(buff[0]);

        // Casting and storing the filter (categorical attribute)
        float filter = static_cast<float>(buff[1]);

        /* Ignoring timestamps (buff[2] and buff[3]) */

        // Casting query vector data to float
        vector<float> q(num_dimensions);
        for (int d = 0; d < num_dimensions; d++) {
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


/*---------------------------Utility functions and classes for the tests-----------------------------------*/



/* Creating a testable graph that inherits from Graph to access protected attributes */
template<typename T>
class TestGraph: public Graph<T> {  // Graph<int> for testing
public:
    TestGraph(bool isDirected=true) : Graph<T>(isDirected) {}

    vector<vector<T>> get_private_adjacency_list() const {
        // Just returns the protected element
        return this->get_adjacency_list();
    }
};


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
    ofstream file(filename, std::ios::binary);
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
