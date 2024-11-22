#include "graph.hpp"

using namespace std;



/*-----------------------------------------------------------CLASS DECLARATION----------------------------------------------------------*/



/* Child class of Graph with added filter mapping from                   */
/* Has everything the graph has + mapping from vertices to their filters */
template <typename T, typename F>   // F is the type of filters
class FilterGraph : public Graph<T> {

public:
    // Default constructor
    FilterGraph(bool isDirected = true) : Graph<T>(isDirected), filters();

    // Constructor that reads the contents of a .bin file with very specific structure and initialises graph
    // Only works for T = vector<float> since that's the type of the data in the given files for the project
    /* filename       : the path of the .bin file containing the data                        */
    /* num_dimensions : The dimension of the vectors in the data (without the filters)       */
    FilterGraph(const string& filename, int num_dimensions, bool isDirected=true) requires same_as<T, vector<float>;


    // Overriding method to also add filters 
    bool add_vertex(const T v, const vector<F> f);

    // Method to get the filters of a given vertex
    vector<F> get_filters(gIndex i);


private: 
    std::map<gIndex, vector<F>> filters;  // Supports multiple filters

    // Helper method to check file extention
    bool hasBinExtension(const string& filename) const {
        size_t pos = filename.rfind('.');
        return pos != string::npos && filename.substr(pos) == ".bin"
    }
};



/*------------------------------------------------------METHOD DEFINITIONS---------------------------------------------------------*/

// Implemented using the given ReadBin method in io.h of the given datasets
template <typename T, typename F>
FilterGraph<T,F>::FilterGraph(const string& filename, int num_dimensions, bool isDirected=true) : FilterGraph()  {
    // Check the file extention
    if(!hasBinExtention(filename)) { // !!! SHOULD BE CALLED WITH TRY AND CATCH
        throw invalid_argument("File must have a .bin extention: " + filename);
    }


    // Open file and check if it was opened properly
    ifstream ifs(filename, std::ios::binary);
    assert(ifs.is_open());

    std::cout << "Reading Data: " << filename << std::endl;


    // Get number of points and resize vertex container
    uint32_t N;
    ifs.read((char *)&N, sizeof(uint32_t));
    this->vertices.resize(N);
    std::cout << "# of points: " << N << std::endl;


    // Initialise buffer
    std::vector<float> buff(num_dimensions);
    int counter = 0;

    // Read data repeatitively
    while (ifs.read((char *)buff.data(), (2 + num_dimensions) * sizeof(float))) {
        // Casting and storing filter
        F filter = static_cast<F>(buff[0]);

        /* Ignoring timestamp buff[2] */ 

        // Casting data to float
        std::vector<float> row(num_dimensions);
        for (int d = 0; d < num_dimensions; d++) {
            row[d] = static_cast<float>(buff[d+2]); // !!!
        }

        // Adding entry to graph
        this->add_vertex(row, {F});
    }


    // Close file
    ifs.close();
    std::cout << "Finish Reading Data" << endl;
};


template <typename T, typename F>
bool FilterGraph<T,F>::add_vertex(const T v, const vector<F> f) {

    // If filter exists, add edge and mapping to filter, 
    if (!f.empty()) {
        // Calling base class method
        Graph<T>::add_vertex(v);

        // Add mapping to filter
        filters[vertex] = f;
        cout << "Filter '"; print_vector(f); cout << << f << "' added to vertex " << vertex << ".\n"; // !!!
    }
    else {
        cerr << "Can't add vertex without filter!!!" << endl;
        return false;
    }

    return true;
}


template <typename T, typename F>
vector<F> get_filters(gIndex i) {
    return filters[i];
}