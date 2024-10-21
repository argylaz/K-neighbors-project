#include "../lib/utils.hpp"
using namespace std;

void fvec_to_graph(const std::string& filename, TestGraph<vector<float>>& G) {
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        std::cerr << "Error when opening file " << filename << std::endl;
        return;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    

    // Read the contents of the file
    while(file) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));
        

        // If there is no data following the dimension the file format is incorrect.
        if(!file) {
            correctFormat = false;
            break;
        }

        // Read the float vector
        vector<float> v;
        file.read(reinterpret_cast<char*>(v.data()), d*sizeof(float));
        if(!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if(v.size() != d) {
            correctFormat = false;
            break;
        }

        G.add_vertex(v);
    }

    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors.";
    file.close();
}

void ivec_to_graph(const std::string& filename, TestGraph<vector<int>>& G) {
    // First we open the file and check if it was opened properly
    ifstream file(filename, ios::binary);
    if (!file) {
        std::cerr << "Error when opening file " << filename << std::endl;
        return;
    }

    // This variable will indicate that the format of the file is correct and as expected.
    bool correctFormat = true;

    // Read the contents of the file
    while(file) {
        // Read the dimension of the vector 
        int d;
        file.read(reinterpret_cast<char*>(&d), sizeof(int));

        // If there is no data following the dimension the file format is incorrect.
        if(!file) {
            correctFormat = false; 
            break;
        }

        // Read the float vector
        vector<int> v;
        file.read(reinterpret_cast<char*>(v.data()), d*sizeof(int));
        if(!file) break;

        // If the number of floats read and the dimension read don't match, the format is incorrect
        if(v.size() != d) {
            correctFormat = false;
            break;
        }

        G.add_vertex(v);
    }

    if (!correctFormat) cerr << "Input file format incorrect! Graph might have errors.";
    file.close();
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
