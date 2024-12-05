#include "utils.hpp"
// #include <concepts>

using namespace std;



/*-----------------------------------------------------------CLASS DECLARATION----------------------------------------------------------*/



/* Child class of Graph with added filter mapping from                   */
/* Has everything the graph has + mapping from vertices to their filters */
template <typename T, typename F>   // F is the type of filters
class FilterGraph : public Graph<T> {

public:
    // Default constructor
    FilterGraph<T,F>(bool isDirected = true);

    // Constructor that reads the contents of a .bin file with very specific structure and initialises graph
    // Only works for T = vector<float> since that's the type of the data in the given files for the project
    /* filename       : the path of the .bin file containing the data                        */
    /* num_dimensions : The dimension of the vectors in the data (without the filters)       */
    FilterGraph<T,F>(const string& filename, int num_dimensions, bool isDirected=true) /*requires same_as<T, vector<float>*/;


    // Overriding method to also add filters 
    bool add_vertex(const T v, const set<F>& f);

    // Method to get the filters of a given vertex
    set<F> get_filters(gIndex i);

    // Method to get a set with all the discrete filters used in the graph
    set<F> get_filters_set();

    // Destructor doing nothing
    ~FilterGraph<T,F>();


private: 
    map<gIndex, set<F>> filters;  // Supports multiple filters
    
    // Set with all the discrete filters
    set<F> filters_set;
    
};

// Helper method to check file extention
bool hasBinExtension(const string& filename) {
    size_t pos = filename.rfind('.');
    return pos != string::npos && filename.substr(pos) == ".bin";
}



/*------------------------------------------------------METHOD DEFINITIONS---------------------------------------------------------*/



/* Default constructor definition */
template <typename T, typename F>
FilterGraph<T,F>::FilterGraph(bool isDirected): Graph<T>(isDirected) {
    // No actual code, just the definition
}


// Implemented using the given ReadBin method in io.h of the given datasets
template <typename T, typename F>
FilterGraph<T,F>::FilterGraph(const string& filename, int num_dimensions, bool isDirected)  : Graph<T>(isDirected) {
    // Check the file extention
    if(!hasBinExtension(filename)) { // !!! SHOULD BE CALLED WITH TRY AND CATCH
        throw invalid_argument("File must have a .bin extention: " + filename);
    }


    // Open file and check if it was opened properly
    ifstream ifs(filename, ios::binary);
    assert(ifs.is_open());

    cout << "Reading Data: " << filename << endl;


    // Get number of points
    uint32_t N;
    ifs.read((char *)&N, sizeof(uint32_t));
    cout << "# of points: " << N << endl;


    // Initialise buffer
    vector<float> buff(num_dimensions);

    // Read data repeatitively
    while (ifs.read((char *)buff.data(), (2 + num_dimensions) * sizeof(float))) {
        // Casting and storing filter
        F filter = static_cast<F>(buff[0]);

        /* Ignoring timestamp buff[2] */ 

        // Casting data to float
        vector<float> row(num_dimensions);
        for (int d = 0; d < num_dimensions; d++) {
            row[d] = static_cast<float>(buff[d+2]); // !!!
        }
                
        // Adding entry to graph
        this->add_vertex(row, {filter});
    }


    // Close file
    ifs.close();
    cout << "Finish Reading Data" << endl;
};

// Add_vertex function for adding a vertex with a specific filter in the graph
template <typename T, typename F>
bool FilterGraph<T,F>::add_vertex(const T v, const set<F>& f) {

    // If filter exists, add edge and mapping to filter, 
    if (!f.empty()) {
        // Calling base class method
        Graph<T>::add_vertex(v);

        // Add mapping to filter
        filters[this->get_index_from_vertex(v)] = f;

        // Add filter  to the set with all the discrete filters
        set_union(filters_set.begin(), filters_set.end(), f.begin(), f.end(), inserter(filters_set, filters_set.begin()));
        // filters_set.insert(f);

        //     cout << "Filter '"; print_vector(f);
        //     print_vector(v); 
        //     cout << "' added to vertex " << v << ".\n"; // !!!
    }
    else {
        cerr << "Can't add vertex without filter!!!" << endl;
        return false;
    }

    return true;
}


// Getter Function for getting the filter of the node with gIndex i
template <typename T, typename F>
set<F> FilterGraph<T,F>:: get_filters(gIndex i){
    return this->filters[i];
}

template <typename T, typename F>
// Method to get a set with all the discrete filters used in the graph
set<F> FilterGraph<T,F>:: get_filters_set(){
    return this->filters_set;
}

template<typename T, typename F>
FilterGraph<T,F>::~FilterGraph() {}