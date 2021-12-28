#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "custom_types/ts.h"
#include "utilities.h"



using namespace std;






bool gfp_test_dd(const unsigned short m, const TS& tsNotSorted){
    
    // sort tasks in the order of decreasing density
    unsigned short tasksSortedByDecrDensity[tsNotSorted.n];
    sort_tasks_by_decreasing_density(tsNotSorted, tasksSortedByDecrDensity);
    
    TS ts;
    ts.n = tsNotSorted.n;
    for (unsigned i = 0; i < ts.n; i++) {
        ts.setTask(i, tsNotSorted.C[tasksSortedByDecrDensity[i]], tsNotSorted.D[tasksSortedByDecrDensity[i]], tsNotSorted.P[tasksSortedByDecrDensity[i]]);
    }
    
    return test_schedulability(m, ts);
    
}
