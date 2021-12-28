#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <math.h>
#include "custom_types/ts.h"
#include "utilities.h"



using namespace std;



unsigned int get_rik(unsigned int rik_prev, const TS& ts, const unsigned short taskIndx) {
    
    unsigned int rik = ts.C[taskIndx];
    
    for (int i = 0; i < taskIndx; i++) {
        rik += ts.C[i]*ceil((float)rik_prev / ts.P[i]);
    }
    
    return rik;
}





bool test_task(const TS& ts, const unsigned short taskIndx){
    
    // m = 1 - uniprocessor case
    unsigned int rik = 0;
    unsigned int rik_new;
    
    
    for (rik = ts.C[taskIndx]; rik <= ts.D[taskIndx]; rik++) {
        rik_new = get_rik(rik, ts, taskIndx);
        
        if (rik == rik_new) {
            return true;
        }
    }
    
    return false;
}



bool fp_test_joseph_pandya(const TS& ts) {
    
    for (unsigned short taskItr = 1; taskItr < ts.n; taskItr++) {
        if (!test_task(ts, taskItr)) return false;
    }
    
    return true;
}
