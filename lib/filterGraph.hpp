#include "file_io.hpp" // Also includes utils

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

    // Method to get the filter count (number of vertices with the given filter)
    long get_filter_count(F filter);

    // Destructor doing nothing
    ~FilterGraph<T,F>();


private: 
    map<gIndex, set<F>> filters;  // Supports multiple filters
    
    // Set with all the discrete filters
    set<F> filters_set;

    // Map from filters to filter specificity
    map<F, long> filterCount;
    
};


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

    // cout << "Reading Data: " << filename << endl;


    // Get number of points
    uint32_t N;
    ifs.read((char *)&N, sizeof(uint32_t));
    // cout << "# of points: " << N << endl;


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
    // cout << "Finish Reading Data" << endl;
};

// Add_vertex function for adding a vertex with a specific filter in the graph
template <typename T, typename F>
bool FilterGraph<T,F>::add_vertex(const T v, const set<F>& f) {

    // If filter exists, add edge and mapping to filter, 
    if (!f.empty()) {
        // Calling base class method
        Graph<T>::add_vertex(v);

        // Add mapping to filter
        // Whenever a duplicate vector is added with another set of filters, we just add the new filters to the set
        set_union(filters[this->get_index_from_vertex(v)].begin(), filters[this->get_index_from_vertex(v)].end(), f.begin(), f.end(), inserter(filters[this->get_index_from_vertex(v)], filters[this->get_index_from_vertex(v)].begin()));
        
        // Increment count of filters
        for (F filter : f) {
            filterCount[filter]++;
        }

        // Add filter  to the set with all the discrete filters
        set_union(filters_set.begin(), filters_set.end(), f.begin(), f.end(), inserter(filters_set, filters_set.begin()));
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

template <typename T, typename F>
// Method to get the filter count (number of vertices with the given filter)
long FilterGraph<T,F>::get_filter_count(F filter) {
    return this->filterCount[filter];
}

template<typename T, typename F>
FilterGraph<T,F>::~FilterGraph() {}