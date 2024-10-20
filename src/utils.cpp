#include "../lib/utils.hpp"
using namespace std;

void fvec_to_graph(const std::string& filename, TestGraph<vector<float>>& G) {
    return;
}

void ivec_to_graph(const std::string& filename, TestGraph<vector<int>>& G) {
    return;
}

/*--------------------------------------------Utility functions for the tests--------------------------------------------*/

/* Creates an fvec file with the given vectors for testing */
void make_fvec(const string& filename, const std::vector<std::vector<float>>& vectors) {
    // Create file and check it was created successfully
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when creating file " << filename << endl;
        return;
    }

    // Read vectors and wite them in the files
    for (const auto& v: vectors) {
        int d = v.size();         // Get the dimension of the vector

        // First write the dimension as an int
        file.write(reinterpret_cast<const char*>(&d), sizeof(int));

        // Then write the data as floats
        file.write(reinterpret_cast<const char*>(v.data()), d*sizeof(float));
    }

    // Close the files
    file.close();
}

/* Creates an ivec file with the given vectors for testing */
void make_ivec(const string& filename, const std::vector<std::vector<int>>& vectors) {
    // Create file and check it was created successfully
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error when creating file " << filename << endl;
        return;
    }

     // Read vectors and wite them in the files
    for (const auto& v: vectors) {
        int d = v.size();         // Get the dimension of the vector

        // First write the dimension as an int
        file.write(reinterpret_cast<const char*>(&d), sizeof(int));

        // Then write the data as floats
        file.write(reinterpret_cast<const char*>(v.data()), d*sizeof(int));
    }

    // Close the files
    file.close();
}
