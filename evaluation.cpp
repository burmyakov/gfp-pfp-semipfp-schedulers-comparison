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







// Test schedulability for global and partitioned schedulers
tuple<bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, unsigned int, unsigned int, long, long, long, long, long, long, long, long, long, long> test_schedulers(const unsigned short m, const unsigned short n, TS& ts, const bool verbose) {
    
    unsigned long t_GFP_OPA = 0, t_DM = 0, t_DCMPO = 0, t_DkC = 0, t_DC = 0, t_DD = 0, t_PFP_OPA = 0, t_FFD = 0, t_WFD = 0, t_SemiPFP = 0;
    
    auto start = std::chrono::system_clock::now();
    bool gfpDCMPO_sched = false;
    gfpDCMPO_sched = gfp_test_dcmpo(m, ts);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DCMPO = elapsed.count();
    cout << "GFP DCMPO sched?: " << gfpDCMPO_sched << ", exec. time: " << t_DCMPO << endl;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDkC_sched = false;
    gfpDkC_sched = gfp_test_dkc(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DkC = elapsed.count();
    cout << "GFP DkC sched?: " << gfpDkC_sched << ", exec. time: " << t_DkC << endl;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDC_sched = false;
    gfpDC_sched = gfp_test_dc(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DC = elapsed.count();
    cout << "GFP DC sched?: " << gfpDC_sched << ", exec. time: " << t_DC << endl;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDM_sched = false;
    gfpDM_sched = gfp_test_dm(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DM = elapsed.count();
    cout << "GFP DM sched?: " << gfpDM_sched << ", exec. time: " << t_DM << endl;
    
    
    start = std::chrono::system_clock::now();
    bool gfpDD_sched = false;
    gfpDD_sched = gfp_test_dd(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_DD = elapsed.count();
    cout << "GFP DD sched?: " << gfpDD_sched << ", exec. time: " << t_DD << endl;
    
    
    
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
    
    
    // Partitioned FP
    bool pfpFFD_sched = false;
    bool pfpWFD_sched = false;
    bool pfpOpt_sched = false;
    
    
    start = std::chrono::system_clock::now();
    pfpFFD_sched = pfp_test_ffd(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_FFD = elapsed.count();
    cout << "PFP FFD sched?: " << pfpFFD_sched << ", exec. time: " << t_FFD << endl;
    
    start = std::chrono::system_clock::now();
    pfpWFD_sched = pfp_test_wfd(m, ts);
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
    cout << "PFP OPT sched?: " << pfpOpt_sched << ", exec. time: " << t_PFP_OPA << endl;
    
    
    // Semi-Partitioned scheduling by Kato and Yamasaki
    bool semipfp_sched = false;
    start = std::chrono::system_clock::now();
    semipfp_sched = semipfp_test(m, ts);
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    t_SemiPFP = elapsed.count();
    cout << "Semi-PFP sched?: " << semipfp_sched << ", exec. time: " << t_SemiPFP << endl;
    
    return make_tuple(gfpDM_sched, gfpDCMPO_sched, gfpDkC_sched, gfpDC_sched, gfpDD_sched, gfpOpa_sched, semipfp_sched, pfpFFD_sched, pfpWFD_sched, pfpOpt_sched, opaCasesGenerated, opaCasesAnalyzed, t_GFP_OPA, t_DM, t_DCMPO, t_DkC, t_DC, t_DD, t_SemiPFP, t_PFP_OPA, t_FFD, t_WFD);
}










int main(){
    
    TS ts;
    int m(2);
    int n(0);
    bool verbose(false);
    
    
    cerr << "The version for 2 processors" << endl;
    cerr << "Number of tasks? " << endl;
    cin >> n;
    ts.n = n;
    
    for (unsigned short i = 0; i < n; i++) {
        int Ci, Di, Pi;
        cout << "C[" << i << "] ?" << endl;
        cin >> Ci;
        cout << "D[" << i << "] ?" << endl;
        cin >> Di;
        cout << "P[" << i << "] ?" << endl;
        cin >> Pi;
        ts.setTask(i, Ci, Di, Pi);
    }
    
    
    // b. Test schedulability
    bool gfpDM_sched, gfpOpa_sched, gfpDCMPO_sched, gfpDkC_sched, gfpDC_sched, gfpDD_sched, semipfp_sched, pfpFFD_sched, pfpWFD_sched, pfpOpt_sched;
    unsigned long t_OPA, t_DM, t_DCMPO, t_DkC, t_DC, t_DD, t_SemiPFP, t_PFP, t_FFD, t_WFD;
    
    unsigned int opaCasesGenerated, opaCasesAnalyzed;
    tie(gfpDM_sched, gfpDCMPO_sched, gfpDkC_sched, gfpDC_sched, gfpDD_sched, gfpOpa_sched, semipfp_sched, pfpFFD_sched, pfpWFD_sched, pfpOpt_sched, opaCasesGenerated, opaCasesAnalyzed, t_OPA, t_DM, t_DCMPO, t_DkC, t_DC, t_DD, t_SemiPFP, t_PFP, t_FFD, t_WFD) = test_schedulers(m, n, ts, verbose);
    
}
