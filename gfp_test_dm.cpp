#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "custom_types/ts.h"
#include "utilities.h"



using namespace std;






bool gfp_test_dm(const unsigned short m, const TS& ts){
    
    // tasks in ts are assumed to be sorted by DM priorities
    return test_schedulability(m, ts);
    
}
