#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "custom_types/ts.h"
#include "utilities.h"
//#include <tuple>



using namespace std;






void permute(const unsigned short m, const TS& ts, string select, string remain, vector<string>& permutations){
    
    /*cout << "permute iteration" << endl;
    cout << "select: " << select << endl;
    cout << "remain: " << remain << endl;
    cout << "permutations: [" << endl;
    for (unsigned short itr = 0; itr < permutations.size(); ++itr) {
        cout << permutations[itr] << endl;
    }
    cout << "]" << endl;*/
    
    if (remain == ""){
        permutations.push_back(select);
        return;
    }
    for (int i = 0; remain[i]; ++i){
        string wk(remain);
        
        // check that indices for m highest-priority jobs are decreasing
        // (optimization when searching an optimal priority assignment)
        if ((select.size() > 0) && (select.size() < m)) {
            if (select[select.size()-1] > remain[i]) continue;
        }
        
        
        /*bool skipPermutation = false;
        for (unsigned short itr = 0; itr < select.size(); ++itr) {
            if (ts.T[(int)select[itr] - '0'] == ts.T[(int)remain[i] - '0'])
                if (remain[i] > select[itr]) {
                    skipPermutation = true;
                    break;
                }
        }
        if (skipPermutation) continue; // iterate i for remain[i]
        
        for (unsigned short itr = 0; itr < select.size(); ++itr) {
            if (ts.C[(int)select[itr] - '0'] == ts.C[(int)remain[i] - '0'])
                if (remain[i] > select[itr]) {
                    skipPermutation = true;
                    break;
                }
        }
        if (skipPermutation) continue; // iterate i for remain[i]*/
        
        permute(m, ts, select + remain[i], wk.erase(i, 1), permutations);
    }
}







bool gfp_test_opa(const unsigned short m, const TS& ts, const bool verbose, unsigned int& casesGenerated, unsigned int& casesAnalyzed){

    const unsigned short n = ts.n;
    
    TS ts_permuted;
    ts_permuted.n = n;
    
    vector<string> permutations;
    
    
    //double casesAnalyzed = 0;
    casesGenerated = 0;
    casesAnalyzed = 0;
    
    
    // 1. Check schedulability of task sets with permuted priorities
    
    // 1a. Get possible priority permutations
    string taskIndices;
    for (unsigned short i = 0; i < n; ++i) taskIndices += int_to_string((float)i);
    sort(taskIndices.begin(), taskIndices.end());
    permute(m, ts, "", taskIndices, permutations);
    
    casesGenerated = permutations.size();
    if (verbose) cout << "cases generated: " << casesGenerated << endl;
    
    // 1b. Permute priorities
    for (unsigned short itrPriorityAssignment = 0; itrPriorityAssignment < permutations.size(); ++itrPriorityAssignment) {

        // To check!!!!
        if (itrPriorityAssignment == 0) continue; // case corresponds to DM PA, that has been already tested
        
        for (unsigned short itrTask = 0; itrTask < n; ++itrTask) {
            
            stringstream ss_index;
            int index;
            ss_index << (permutations[itrPriorityAssignment])[itrTask];
            ss_index >> index;

            ts_permuted.setTask(itrTask, ts.C[index], ts.D[index], ts.P[index]);
        }
        
        // 1c. Print selected priority assignment
        /*cout << "analyzing priority assignment:" << endl;
        for (unsigned short itrTask = 0; itrTask < n; ++itrTask)
            cout << "( " << ts_permuted.C[itrTask] << ", " << ts_permuted.T[itrTask] << " )" << endl;*/
        
        
        casesAnalyzed++;
        
        // 1d. Check schedulability for selected priority assignment
        if (test_schedulability(m, ts_permuted)) {
            if (verbose) {
                cout << "optimal priority assignment:" << endl;
                cout << permutations[itrPriorityAssignment] << endl;
                cout << endl;
                cout << "cases analyzed: " << casesAnalyzed << endl;
            }
            return true;
        } else {
            // Task set is unschedulable
            // Keep searching for an optimal priority assignment
            
            // As current \tau_k is unschedulable,
            // then any tau'_k with C'_k >= C_k and P'_k <= P_k is also unschedulable;
            // thus, removing those cases from permutations to be analyzed
            
            for (unsigned short itrPrAss2 = itrPriorityAssignment + 1; itrPrAss2 < permutations.size();) {
                
                stringstream ss_index;
                int index;
                ss_index << (permutations[itrPrAss2])[n-1];
                ss_index >> index;
                
                if (((ts.C[index] >= ts_permuted.C[n-1]) && (ts.D[index] < ts_permuted.D[n-1])) || ((ts.C[index] > ts_permuted.C[n-1]) && (ts.D[index] <= ts_permuted.D[n-1]))) {
                    permutations.erase(permutations.begin() + itrPrAss2);
                } else {
                    itrPrAss2++;
                }
            }
            
            continue;
        }
    }
    
    if (verbose) cout << "cases analyzed: " << casesAnalyzed << endl;
    return false;
}
