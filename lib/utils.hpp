#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
using namespace std;

// Function for the calculation of the Euclidean distance

template<typename Type>             // Type is either integer or float
float Euclidean_Distance( vector<Type> a, vector<Type> b) {
    
    int sum = 0;

    // Iterators of the vectors
    auto i = a.begin();
    auto j = b.begin();
    
    while( i != a.end() || j != b.end() ){
        sum += pow(*j-*i,2);           // sum = (i1 -j1)^2 + ... + (i_n-j_n)^2
        i++;
        j++;
    }
    float x = sqrt(sum);                        // [(i1 -j1)^2 + ... + (i_n-j_n)^2] ^ 1/2
    return x;

}