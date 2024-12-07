#include <cmath>
#include <ctype.h>
#include <string.h>

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
        while ( count < R ) {
            if ( shuffled_vertices[i] != v ) {
                G.add_edge(v, shuffled_vertices[i]);  
                count++;  
            }
            i++;
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



