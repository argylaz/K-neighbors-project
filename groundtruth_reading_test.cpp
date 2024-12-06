#include "lib/filterGraph.hpp"
// #include "lib/utils.hpp"

using namespace std;
 

/* Main Program which tests the reading of a groundtruth file groundtruth of a .bin file*/


// To do:: flags arguments
int main(void){
    
    string filename = "sift/groundtruth.bin" ; 
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

    for( int i = 0 ; i < (int)N ; i++ ){

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


    return 0;

}





