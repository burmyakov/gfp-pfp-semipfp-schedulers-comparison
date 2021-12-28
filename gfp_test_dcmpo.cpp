#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "custom_types/ts.h"
#include "utilities.h"



using namespace std;








bool gfp_test_dcmpo(const unsigned short m, const TS& ts){

    //cout << "inside gfp_test_dcmpo(...)" << endl;
    
    const unsigned short n = ts.n;
    
    TS ts_sorted;
    ts_sorted.n = n;
    for (unsigned short i = 0; i < ts.n; i++) {
        ts_sorted.C[i] = ts.C[i];
        ts_sorted.D[i] = ts.D[i];
        ts_sorted.P[i] = ts.P[i];
    }
    
    
    // Sort tasks according to D-CMPO order
    for (unsigned short i = 0; i < ts.n; i++) {
        for (unsigned short j = i+1; j < ts.n; j++) {
            if (ts_sorted.D[i] - ts_sorted.C[i] > ts_sorted.D[j] - ts_sorted.C[j]) {
                swap(ts_sorted.C[i], ts_sorted.C[j]);
                swap(ts_sorted.D[i], ts_sorted.D[j]);
                swap(ts_sorted.P[i], ts_sorted.P[j]);
            }
        }
    }
    
    /*cout << "Tasks sorted by DCMPO:" << endl;
    for (unsigned short i = 0; i < ts.n; i++) {
        cout << "i: " << i << " ( " << ts_sorted.C[i] << ", " << ts_sorted.D[i] << ", " << ts_sorted.P[i] << " )" << endl;
    }*/
    
    
    // Check schedulability
    if (test_schedulability(m, ts_sorted)) return true;
    else return false;
}
