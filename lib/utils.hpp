#include <cmath>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "./graph.hpp"



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


// Function that checks if a string is a positive integer (for checking the command line arguments)
bool isPositiveInteger(char *str);

// Function that returns the float as string without excess decimal zeros
string to_string_trimmed(float value);

// Function that returns the user time of the calling function
void measure_user_time(); 


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

    // Find the sum of the calculated vector
    float sum = 0.0f;
    #pragma omp simd reduction(+:sum) // Enable SIMD with reduction for parallel summation
    for(size_t i = 0; i < a.size(); i++) {

        float diff = a[i] - b[i];
        sum += diff * diff;

    }

    // Get the square root of the sum
    float x = sqrt(sum);               // [(i1 -j1)^2 + ... + (i_n-j_n)^2] ^ 1/2
    return x;
}



template <typename Type>
inline vector<Type> find_min_Euclidean(Graph<vector<Type>>& G, set<gIndex>& S, vector<Type> xquery) {
    
    // Find the element with the minimum Euclidean distance from xquer
    gIndex min_index = *min_element(S.begin(), S.end(), 
                        [&xquery, &G](const gIndex a, const gIndex b) {
                            return Euclidean_Distance<Type>(G.get_vertex_from_index(a), xquery) < Euclidean_Distance<Type>(G.get_vertex_from_index(b), xquery);
    });

    return G.get_vertex_from_index(min_index);
}


/* Given a set of vectors and a target vector (xquery), only keep the L vectors closest to xquery */
template <typename Type>
inline void retain_closest_points(Graph<vector<Type>>& G , set<gIndex> &output_set, vector<Type> xquery, int L) {

    // Erasing the vertex itself from the set in order to avoid mistaking it for a neighbor
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

    // Get the vertices
    set<vector<Type>> vertices = G.get_vertices();
    vector<vector<Type>> vertex_list(vertices.begin(), vertices.end());
    int n = vertex_list.size();

    // Thread function to compute the sum for a range of vertices
    auto compute_sum = [&](int start, int end, float& local_min, vector<Type>& local_medoid) {
        for (int i = start; i < end; ++i) {
            float sum = 0;
            for (const auto& x : vertex_list) {
                sum += Euclidean_Distance<Type>(vertex_list[i], x);
            }
            // The section below is critical, so we need to protect it with a mutex
            std::lock_guard<std::mutex> lock(mutex);  // Protect shared variables (one thread at a time)
            if (sum < local_min) {
                local_min = sum;
                local_medoid = vertex_list[i];
            }
        }
    };

    // Create threads
    int num_threads = std::thread::hardware_concurrency(); // Get the number of threads
    vector<std::thread> threads;
    vector<float> local_mins(num_threads, numeric_limits<float>::max()); // One local_min for each thread
    vector<vector<Type>> local_medoids(num_threads);

    int chunk_size = (n + num_threads - 1) / num_threads; // Divide into chunks of size ceil(count_vertices/num_threads) chunks
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = std::min(start + chunk_size, n);
        threads.emplace_back(compute_sum, start, end, ref(local_mins[t]), ref(local_medoids[t]));
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Find the global minimum by comparing the results of each thread
    for (int t = 0; t < num_threads; ++t) {
        if (local_mins[t] < min) {
            min = local_mins[t];
            medoid_vertice = local_medoids[t];
        }
    }

    return medoid_vertice;
}



/* Method which adds randomly exactly R outgoing neighbors to each vertex of the graph */
template <typename T>
void rDirectional(Graph<T>& G, int R) {
    const long int vertex_count = G.get_vertices_count();
    
    // Check if R is larger than the number of vertices (task impossible)
    if ( R > vertex_count ) {
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

    // Random number generator setup
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    uniform_int_distribution<int> dist(0, vertex_count - 1);

    // For each vertex of the graph
    for (T v : vertices) {
        gIndex v_index = G.get_index_from_vertex(v); // Get the index of the current vertex
        unordered_set<gIndex> chosen_neighbors; // Track selected neighbors to ensure no duplicates

        // Add R random outgoing neighbors
        while (chosen_neighbors.size() < static_cast<size_t>(R)) {
            gIndex random_index = dist(rng) % G.get_vertices_count(); // Get a random index

            // Ensure the random neighbor is not the current vertex and hasn't been chosen already
            if (random_index != v_index && chosen_neighbors.insert(random_index).second) {
                G.add_edge(v, G.get_vertex_from_index(random_index)); // Add edge using indices
            }   
        }
    }

}


// Function that checks if a string is a positive integer (for checking the command line arguments)
bool isPositiveInteger(const char *str) {
    int sz = strlen(str);
    for (int i = 0; i < sz; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// Function that returns the float as string without excess decimal zeros 
string to_string_trimmed(float value) {
    ostringstream oss;
    oss << fixed << setprecision(5) << value; // Set a high precision
    string result = oss.str();

    // Remove trailing zeros
    result.erase(result.find_last_not_of('0') + 1);

    // Remove the decimal point if it ends up being the last character
    if (result.back() == '.') {
        result.pop_back();
    }

    return result;
}

void measure_user_time() {
    struct rusage usage;

    // Get resource usage for the calling process
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        struct timeval user_time = usage.ru_utime;

        cout << " " << (long)user_time.tv_sec << "." << (long)user_time.tv_usec;
    } else {
        perror("getrusage failed");
    }
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



