#include <iostream>
#include <bits/stdc++.h>
#include "lib/utils.hpp"

using namespace std;


int main(){

    vector<int> v1,v2;

    // for( int i = 1 ; i < 3 ; i++ ){
    //     v1.push_back(i+10);
    //     v2.push_back(i);
    // }

    v2.push_back(12);
    v2.push_back(2);
    v2.push_back(0);

    v1.push_back(10);
    v1.push_back(2);
    v1.push_back(0);

    float d = Euclidean_Distance(v1, v2);

    cout << "Euclidean_Distance: " << d << endl;

    return 1;
}