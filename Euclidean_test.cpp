#include <iostream>
#include <bits/stdc++.h>
#include "lib/utils.hpp"

using namespace std;


int main(){

    vector<float> v1,v2,v3,v4;

    // for( int i = 1 ; i < 3 ; i++ ){
    //     v1.push_back(i+10);
    //     v2.push_back(i);
    // }

    // v2.push_back(12);
    // v2.push_back(2);
    // v2.push_back(0);

    // v1.push_back(10);
    // v1.push_back(2);
    // v1.push_back(0);

    v1.push_back(1.1);
    v1.push_back(3.5);

    v2.push_back(7.7);
    v2.push_back(4.4);


    v3.push_back(3);
    v3.push_back(5);

    v4.push_back(6);
    v4.push_back(8);




    float d1 = Euclidean_Distance(v1, v2);
    float d2 = Euclidean_Distance(v3, v4);
    cout << "Euclidean_Distance: " << d1 << endl;
    cout << "Euclidean_Distance: " << d2 << endl;

    return 1;
}