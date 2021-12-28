#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include "custom_types/ts.h"
#include "utilities.h"



using namespace std;








bool pfp_test_wfd(const unsigned short m, const TS& ts){

    
    const unsigned short n = ts.n;
    

    // a. Sort tasks by decreasing densities
    unsigned short tasksSortedByDecrDensity[n];
    sort_tasks_by_decreasing_density(ts, tasksSortedByDecrDensity);
    
    
    
    // b. Init aggregated processor utilization
    vector<double> procAccumUtil;
    vector<vector<unsigned short> > procTasks;
    for (unsigned short itrProc = 0; itrProc < m; ++itrProc) {
        procAccumUtil.push_back((double)0);
        procTasks.push_back(*(new vector<unsigned short>));
    }
    
    
    
    // c. Allocation of task densities to processors
    unsigned short curTaskIndex;
    float curDensity;
    float curUtilization;
    for (unsigned short itrTask = 0; itrTask < n; ++itrTask) {
        
        curTaskIndex = tasksSortedByDecrDensity[itrTask];
        curDensity = (float)ts.C[curTaskIndex]/ts.D[curTaskIndex];
        curUtilization = (float)ts.C[curTaskIndex]/ts.P[curTaskIndex];
        
        // Assign task to all partitions
        // and then choose the one with a minimal aggregated utilization
        for (unsigned short itrProc = 0; itrProc < m; ++itrProc) {
            //procAccumUtil[itrProc] += curDensity;
            procAccumUtil[itrProc] += curUtilization;
            procTasks[itrProc].push_back(curTaskIndex);
        }
        
        // Identify partition with a minimal aggregated density
        double Umin = *min_element(procAccumUtil.begin(), procAccumUtil.end());
        if ((Umin - curUtilization + curDensity) > 1) return false; // TS is unschedulable by WFD, as neccessary sched. cond. violated
        
        // Assign task to processor with a minimal aggregated density
        // and test schedulability of that partition
        bool taskAssigned = false;
        for (unsigned short itrProc = 0; itrProc < m; ++itrProc) {
            
            if ((!taskAssigned) && (procAccumUtil[itrProc] == Umin)) {
                taskAssigned = true;
                
                // test schedulability
                if (!test_partition_schedulability(ts, procTasks[itrProc]))
                    return false; // TS is unschedulable by WFD
            }
            else {
                // Restore partition settings,
                // as task is assigned to another partition
                
                // thus, removing appended task
                auto it = find(procTasks[itrProc].begin(), procTasks[itrProc].end(), curTaskIndex);
                swap(*it, procTasks[itrProc].back());
                procTasks[itrProc].pop_back();
                
                // and reducing density
                //procAccumUtil[itrProc] -= curDensity;
                procAccumUtil[itrProc] -= curUtilization;
            }
        }
    }
    
    return true; // schedulable by WFD
}
