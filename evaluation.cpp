#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <utility>
#include <tuple>
#include "custom_types/ts.h"
#include "gfp_test_dm.h"
#include "gfp_test_opa.h"
#include "pfp_test_ffd.h"
#include "pfp_test_wfd.h"
#include "pfp_test_optimal.h"
#include "semipfp_test.h"
#include "gfp_test_dcmpo.h"
#include "gfp_test_dkc.h"
#include "gfp_test_dc.h"
#include "gfp_test_dd.h"
#include <random>
#include "math.h"
#include <sys/time.h>
#include <chrono>
#include "fp_test_joseph_pandya.h"



using namespace std;

unsigned int gfpDMSchedCasesNum[100] = { 0 };
unsigned int gfpDCMPOSchedCasesNum[100] = { 0 };
unsigned int gfpDkCSchedCasesNum[100] = { 0 };
unsigned int gfpDCSchedCasesNum[100] = { 0 };
unsigned int gfpDDSchedCasesNum[100] = { 0 };
unsigned int gfpOPASchedCasesNum[100] = { 0 };

unsigned int pfpFFDSchedCasesNum[100] = { 0 };
unsigned int pfpWFDSchedCasesNum[100] = { 0 };
unsigned int pfpOPTSchedCasesNum[100] = { 0 };
unsigned int semiPfpSchedCasesNum[100] = { 0 };

/*unsigned int gfpDM[100] = { 0 };
unsigned int gfpDCMPO[100] = { 0 };
unsigned int gfpDkC[100] = { 0 };
unsigned int gfpDC[100] = { 0 };
unsigned int gfpOPA[100] = { 0 };

unsigned int pfpFFD[100] = { 0 };
unsigned int pfpWFD[100] = { 0 };
unsigned int pfpOPA[100] = { 0 };*/

unsigned int schedOnlyGfp[100] = { 0 };
unsigned int schedOnlyPfp[100] = { 0 };
unsigned int schedBySemiButNotPfp[100] = { 0 };

unsigned short caseNum = -1;






// Generate input and output file names
pair<string, string> generate_file_names() {
    
    /*ostringstream ss_m;
    ss_m << ((unsigned short)m);
    string m_str(ss_m.str());
    
    ostringstream ss_n;
    ss_n << ((unsigned short)n);
    string n_str(ss_n.str());
    
    ostringstream ss_U;
    ss_U << ((float)U);
    string U_str(ss_U.str());
    
    ostringstream ss_Umax;
    ss_Umax << ((float)Umax);
    string Umax_str(ss_Umax.str());
    
    ostringstream ss_n_heavy;
    ss_n_heavy << ((float)n_heavy);
    string n_heavy_str(ss_n_heavy.str());
    
    ostringstream ss_Pratio;
    ss_Pratio << ((float)Pratio);
    string Pratio_str(ss_Pratio.str());
    
    
    
    string fileNameTasks;
    string fileNameResults;
    
    if (expType == 0) {
        fileNameTasks = string("generate_tasks_by_cplex/task_sets_varying_nheavy[n_") + n_str + "_U_" + U_str + "_Umax_" + Umax_str + "_Pratio_" + Pratio_str + "].txt";
        fileNameResults = string("comparison_gfp_vs_pfp_varying_nheavy[m_") + m_str + "_n_" + n_str + "_U_" + U_str + "_Umax_" + Umax_str + "_Pratio_" + Pratio_str + "].txt";
    } else if (expType == 1) {
        fileNameTasks = string("generate_tasks_by_cplex/task_sets_varying_Umax[n_") + n_str + "_U_" + U_str + "_nheavy_" + n_heavy_str + "_Pratio_" + Pratio_str + "].txt";
        fileNameResults = string("comparison_gfp_vs_pfp_varying_Umax[m_") + m_str + "_n_" + n_str + "_U_" + U_str + "_nheavy_" + n_heavy_str + "_Pratio_" + Pratio_str + "].txt";
    } else if (expType == 2) {
        fileNameTasks = string("generate_tasks_by_cplex/task_sets_varying_U[n_") + n_str + "_Umax_" + Umax_str + "_nheavy_" + n_heavy_str + "_Pratio_" + Pratio_str + "].txt";
        fileNameResults = string("comparison_gfp_vs_pfp_varying_U[m_") + m_str + "_n_" + n_str + "_Umax_" + Umax_str + "_nheavy_" + n_heavy_str + "_Pratio_" + Pratio_str + "].txt";
    } else if (expType == 3) {
        fileNameTasks = string("generate_tasks_by_cplex/task_sets_varying_Pratio[n_") + n_str + "_U_" + U_str + "_Umax_" + Umax_str + "_nheavy_" + n_heavy_str + "].txt";
        fileNameResults = string("comparison_gfp_vs_pfp_varying_Pratio[m_") + m_str + "_n_" + n_str + "_U_" + U_str + "_Umax_" + Umax_str + "_nheavy_" + n_heavy_str + "].txt";
    }*/
    
    string fileNameTasks = string("input_task_sets/task_sets_varying_Dratio[n_3_nheavy_2U1.75_PDratio_1].txt");
    string fileNameResults = string("results/results_varying_Dratio[n_3_nheavy_2U1.75_PDratio_1].txt");
    
    return make_pair(fileNameTasks, fileNameResults);
}







// Test schedulability for global and partitioned schedulers
tuple<bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, unsigned int, unsigned int, long, long, long, long, long, long, long, long, long, long> test_schedulers(const unsigned short m, const unsigned short n, const unsigned short n_heavy, TS& ts, const bool verbose) {
    
    unsigned long t_GFP_OPA = 0, t_DM = 0, t_DCMPO = 0, t_DkC = 0, t_DC = 0, t_DD = 0, t_PFP_OPA = 0, t_FFD = 0, t_WFD = 0, t_SemiPFP = 0;
    
    auto start = std::chrono::system_clock::now();
    bool gfpDCMPO_sched = false;
    gfpDCMPO_sched = gfp_test_dcmpo(m, ts);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DCMPO = elapsed.count();
    
    cout << "GFP DCMPO sched?: " << gfpDCMPO_sched << ", exec. time: " << t_DCMPO << endl;
    gfpDCMPOSchedCasesNum[caseNum] = gfpDCMPOSchedCasesNum[caseNum] + (unsigned int)gfpDCMPO_sched;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDkC_sched = false;
    //gfpDkC_sched = gfp_test_dkc(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DkC = elapsed.count();
    cout << "GFP DkC sched?: " << gfpDkC_sched << ", exec. time: " << t_DkC << endl;
    gfpDkCSchedCasesNum[caseNum] = gfpDkCSchedCasesNum[caseNum] + (unsigned int)gfpDkC_sched;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDC_sched = false;
    //gfpDC_sched = gfp_test_dc(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DC = elapsed.count();
    cout << "GFP DC sched?: " << gfpDC_sched << ", exec. time: " << t_DC << endl;
    gfpDCSchedCasesNum[caseNum] = gfpDCSchedCasesNum[caseNum] + (unsigned int)gfpDC_sched;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDM_sched = false;
    gfpDM_sched = gfp_test_dm(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DM = elapsed.count();
    cout << "GFP DM sched?: " << gfpDM_sched << ", exec. time: " << t_DM << endl;
    gfpDMSchedCasesNum[caseNum] = gfpDMSchedCasesNum[caseNum] + (unsigned int)gfpDM_sched;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDD_sched = false;
    //gfpDD_sched = gfp_test_dd(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DD = elapsed.count();
    cout << "GFP DD sched?: " << gfpDD_sched << ", exec. time: " << t_DD << endl;
    gfpDDSchedCasesNum[caseNum] = gfpDDSchedCasesNum[caseNum] + (unsigned int)gfpDD_sched;
    
    
    
    bool gfpOpa_sched = ((((gfpDM_sched || gfpDCMPO_sched) || gfpDkC_sched) || gfpDC_sched) || gfpDD_sched);
    unsigned int opaCasesGenerated = 0;
    unsigned int opaCasesAnalyzed = 0;
    if (!gfpOpa_sched) {
        start = std::chrono::system_clock::now();
        gfpOpa_sched = gfp_test_opa(m, ts, verbose, opaCasesGenerated, opaCasesAnalyzed);
        end = std::chrono::system_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        t_GFP_OPA = elapsed.count();
    }
    cout << "GFP OPA sched?: " << gfpOpa_sched << ", exec. time: " << t_GFP_OPA << "\t\t" << "cases generated: " << opaCasesGenerated << ", cases analyzed: " << opaCasesAnalyzed << endl;
    gfpOPASchedCasesNum[caseNum] = gfpOPASchedCasesNum[caseNum] + (unsigned int)gfpOpa_sched;
    
    
    // Partitioned FP
    bool pfpFFD_sched = false;
    bool pfpWFD_sched = false;
    bool pfpOpt_sched = false;
    
    
    start = std::chrono::system_clock::now();
    pfpFFD_sched = pfp_test_ffd(m, ts);
    pfpFFDSchedCasesNum[caseNum] = pfpFFDSchedCasesNum[caseNum] + (unsigned int)pfpFFD_sched;
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_FFD = elapsed.count();
    cout << "PFP FFD sched?: " << pfpFFD_sched << ", exec. time: " << t_FFD << endl;
    
    start = std::chrono::system_clock::now();
    pfpWFD_sched = pfp_test_wfd(m, ts);
    pfpWFDSchedCasesNum[caseNum] = pfpWFDSchedCasesNum[caseNum] + (unsigned int)pfpWFD_sched;
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_WFD = elapsed.count();
    cout << "PFP WFD sched?: " << pfpWFD_sched << ", exec. time: " << t_WFD << endl;
    
    pfpOpt_sched = pfpFFD_sched || pfpWFD_sched;
    if (!pfpOpt_sched) {
        start = std::chrono::system_clock::now();
        pfpOpt_sched = pfp_test_optimal(m, ts);
        end = std::chrono::system_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        t_PFP_OPA = elapsed.count();
    }
    pfpOPTSchedCasesNum[caseNum] = pfpOPTSchedCasesNum[caseNum] + (unsigned int)pfpOpt_sched;
    cout << "PFP OPT sched?: " << pfpOpt_sched << ", exec. time: " << t_PFP_OPA << endl;
    
    
    // Semi-Partitioned scheduling by Kato and Yamasaki
    bool semipfp_sched = false;
    start = std::chrono::system_clock::now();
    semipfp_sched = semipfp_test(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_SemiPFP = elapsed.count();
    semiPfpSchedCasesNum[caseNum] = semiPfpSchedCasesNum[caseNum] + (unsigned int)semipfp_sched;
    cout << "Semi-PFP sched?: " << semipfp_sched << ", exec. time: " << t_SemiPFP << endl;
    
    
    if (!pfpOpt_sched && semipfp_sched) schedBySemiButNotPfp[caseNum]++;
    
    
    if (gfpOpa_sched && !pfpOpt_sched) schedOnlyGfp[caseNum]++;
    if (!gfpOpa_sched && pfpOpt_sched) schedOnlyPfp[caseNum]++;
    
    return make_tuple(gfpDM_sched, gfpDCMPO_sched, gfpDkC_sched, gfpDC_sched, gfpDD_sched, gfpOpa_sched, semipfp_sched, pfpFFD_sched, pfpWFD_sched, pfpOpt_sched, opaCasesGenerated, opaCasesAnalyzed, t_GFP_OPA, t_DM, t_DCMPO, t_DkC, t_DC, t_DD, t_SemiPFP, t_PFP_OPA, t_FFD, t_WFD);
}










int main(){

    int m(3);
    int n(0);
    float U(0);
    int n_heavy(0);
    float Dratio(1);
    float Dratio_prev(0);
    float PDratio(1);
    bool verbose(false);
    
    
    cerr << "Number of processors? " << endl;
    cin >> m;
    
    
    // Prepare file handlers
    string fileNameTasks, fileNameResults;
    tie(fileNameTasks, fileNameResults) = generate_file_names();
    
    ofstream fileResults;
    
    ifstream fileTaskSets(fileNameTasks.c_str());
    if (!fileTaskSets) {
        cerr << "Unable to open file containing task set parameters: " << fileNameTasks << endl;
        abort();
    }
    
    
    while (fileTaskSets.good()) {
        
        // a. Read task set parameters from file
        fileTaskSets >> n >> n_heavy >> U >> Dratio >> PDratio;

        cout << endl << endl << endl << "m: " << m << endl;
        cout << "n: " << n << endl;
        cout << "n_heavy: " << n_heavy << endl;
        cout << "U: " << U << endl;
        cout << "Dratio: " << Dratio << endl;
        TS ts;
        ts.n = n;
        for (unsigned short i = 0; i < n; i++) {
            int Ci, Di, Pi;
            fileTaskSets >> Ci;
            fileTaskSets >> Di;
            fileTaskSets >> Pi;
            cout << "i: " << i << ", Ci: " << Ci << ", Di: " << Di << ", Pi: " << Pi << endl;
            ts.setTask(i, Ci, Di, Pi);
        }
        
        // To print the summary of the results into console
        if (fabs(Dratio_prev - Dratio) >= 0.01) {
            caseNum++;
            Dratio_prev = Dratio;
        }
        
        
        // b. Test schedulability
        bool gfpDM_sched, gfpOpa_sched, gfpDCMPO_sched, gfpDkC_sched, gfpDC_sched, gfpDD_sched, semipfp_sched, pfpFFD_sched, pfpWFD_sched, pfpOpt_sched;
        unsigned long t_OPA, t_DM, t_DCMPO, t_DkC, t_DC, t_DD, t_SemiPFP, t_PFP, t_FFD, t_WFD;
        
        unsigned int opaCasesGenerated, opaCasesAnalyzed;
        tie(gfpDM_sched, gfpDCMPO_sched, gfpDkC_sched, gfpDC_sched, gfpDD_sched, gfpOpa_sched, semipfp_sched, pfpFFD_sched, pfpWFD_sched, pfpOpt_sched, opaCasesGenerated, opaCasesAnalyzed, t_OPA, t_DM, t_DCMPO, t_DkC, t_DC, t_DD, t_SemiPFP, t_PFP, t_FFD, t_WFD) = test_schedulers(m, n, n_heavy, ts, verbose);
        
        if ((gfpDCMPO_sched) && (!gfpDM_sched)) {
            abort();
        }
        
        
        // c. Store results into output file
        fileResults.open(fileNameResults.c_str(), ios::app);
        fileResults << m << "\t" << n << "\t" << n_heavy << "\t" << U << "\t" << Dratio << "\t" << gfpOpa_sched << "\t" << gfpDM_sched << "\t" << gfpDCMPO_sched << "\t" << gfpDkC_sched << "\t" << gfpDC_sched << "\t" << gfpDD_sched << "\t" << semipfp_sched << "\t" << pfpOpt_sched << "\t" << pfpFFD_sched << "\t" << pfpWFD_sched << "\t" << opaCasesGenerated << "\t" << opaCasesAnalyzed;
        fileResults << "\t" << t_OPA << "\t" << t_DM << "\t" << t_DCMPO << "\t" << t_DkC << "\t" << t_DC << "\t" << t_DD << "\t" << t_SemiPFP << "\t" << t_PFP << "\t" << t_FFD << "\t" << t_WFD;
        
        for (unsigned short taskItr = 0; taskItr < n; taskItr++) fileResults << "\t" << ts.C[taskItr] << "\t" << ts.D[taskItr] << "\t" << ts.P[taskItr];
        
        fileResults << endl;
        fileResults.close();
    } //end of while fileTaskSets.good()
    
    cout << endl << "======================" << endl;
    cout << "Results:" << endl;
    for (int i = 0; i <= caseNum; i++) {
        cout << endl << endl;
        cout << "Dratio: " << i << endl;
        cout << "DM:\t\t" << gfpDMSchedCasesNum[i] << endl;
        cout << "DCMPO:\t\t" << gfpDCMPOSchedCasesNum[i] << endl;
        cout << "DkC:\t\t" << gfpDkCSchedCasesNum[i] << endl;
        cout << "DC:\t\t" << gfpDCSchedCasesNum[i] << endl;
        cout << "DD:\t\t" << gfpDDSchedCasesNum[i] << endl;
        cout << "GOPA:\t\t" << gfpOPASchedCasesNum[i] << endl;
        cout << "PFFD:\t\t" << pfpFFDSchedCasesNum[i] << endl;
        cout << "PWFD:\t\t" << pfpWFDSchedCasesNum[i] << endl;
        cout << "POPA:\t\t" << pfpOPTSchedCasesNum[i] << endl;
        cout << "Semi-PFP:\t" << semiPfpSchedCasesNum[i] << endl;
        
        cout << "Sched Only GFP: "  << schedOnlyGfp[i]  << endl;
        cout << "Sched Only PFP: "  << schedOnlyPfp[i]  << endl;
        cout << "Sched By SemiPFP, but not PFP: " << schedBySemiButNotPfp[i] << endl;
    }
}
