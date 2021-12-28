#include <iostream>
#include <algorithm>
#include <vector>
#include "custom_types/ts.h"
#include "utilities.h"




using namespace std;






bool pfp_test_ffd(const unsigned short m, const TS& ts){
    
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
    
    
    // c. Assign task to processor with the smallest index,
    // such that schedulability holds
    unsigned short curTaskIndex;
    float curDensity;
    float curUtilization;
    for (unsigned short itrTask = 0; itrTask < n; ++itrTask) {
        
        curTaskIndex = tasksSortedByDecrDensity[itrTask];
        curDensity = (float)ts.C[curTaskIndex]/ts.D[curTaskIndex];
        curUtilization = (float)ts.C[curTaskIndex]/ts.P[curTaskIndex];
        
        bool taskAssigned = false;
        for (unsigned short itrProc = 0; itrProc < m; ++itrProc) {
            
            // check necessary unsched. cond.
            if (procAccumUtil[itrProc] + curDensity <= 1) {
            //if (procAccumUtil[itrProc] + curUtilization <= 1) {
                
                // assign task to processor and test schedulability
                procTasks[itrProc].push_back(curTaskIndex);
                if (test_partition_schedulability(ts, procTasks[itrProc])) {
                    // assign task to procItr
                    taskAssigned = true;
                    
                    //procAccumUtil[itrProc] += curDensity;
                    procAccumUtil[itrProc] += curUtilization;
                    break; // for itrProc loop
                } else {
                    // task cannot be assigned to processor[procItr],
                    // as schedulability is violated
                    
                    // thus, removing appended task
                    auto it = find(procTasks[itrProc].begin(), procTasks[itrProc].end(), curTaskIndex);
                    swap(*it, procTasks[itrProc].back());
                    procTasks[itrProc].pop_back();
                }
                
            } else {
                // necessary sched. test does not hold,
                // attempting to assign to another processor
                continue;
            }
        }
        
        // Check that task is assigned to some processor,
        // otherwise report unschedulability
        if (!taskAssigned) return false; // ts is unschedulable by FFD
        /*else if (itrTask == n-1) {
            cout << "An optimal partition is found:" << endl;
            for (int itrProc = 0; itrProc < m; ++itrProc) {
                cout << "proc. num:" << itrProc << endl;
                for (int itrTask = 0; itrTask < procTasks[itrProc].size(); itrTask++) {
                    cout << "( " << ts.C[procTasks[itrProc][itrTask]] << ", " << ts.D[procTasks[itrProc][itrTask]] << ", " << ts.P[procTasks[itrProc][itrTask]] << " )" << endl;
                }
            }
        }*/
    }
    
    
    return true; // schedulable by FFD
}
