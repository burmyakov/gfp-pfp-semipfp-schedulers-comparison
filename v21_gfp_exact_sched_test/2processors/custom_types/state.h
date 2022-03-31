#include <iostream>
#include <string>
#include <algorithm>
//#include "ts.h"

using namespace std;


const unsigned short PROC_NUM = 2;


struct state {
    //TS& tsLocal;
    unsigned short n; // tasks number
    unsigned short* c; // remaining execution time
    unsigned short* p; // time to next period
    unsigned short* timePassedFromLastRelease;
    unsigned short* wrts;
    bool* firstJobReleased;
    //bool* releaseAtEarliest;
    bool* interferred; // interference flags
    bool* jobCanBeReleasedBefore;
    bool* processorAvailableForTau_i;
    bool* prevState_processorAvailableForTau_i;
    unsigned short pendJobsNum;
    unsigned short lockedJobsNum;
    unsigned short sumCs;
    bool* tauI_jobJustCompleted;
    int_fast8_t* tauI_releaseNextJobBefore;
    
    // constructor 1
    //state(TS& _ts): tsLocal(_ts) {
    state(const unsigned short _n): n(_n) {
        c = new unsigned short[n];
        p = new unsigned short[n];
        timePassedFromLastRelease = new unsigned short[n];
        wrts = new unsigned short[n];
        firstJobReleased = new bool[n];
        //releaseAtEarliest = new bool[n];
        for (int i = 0; i < n; i++) c[i] = p[i] = timePassedFromLastRelease[i] = wrts[i] = 0;
        for (int i = 0; i < n; i++) firstJobReleased[i] = false;
        //for (int i = 0; i < n; i++) releaseAtEarliest[i] = false;

        jobCanBeReleasedBefore = new bool[n];
        processorAvailableForTau_i = new bool[n];
        prevState_processorAvailableForTau_i = new bool[n];
        for (unsigned short i = 0; i < n; i++) {
            jobCanBeReleasedBefore[i] = false; // assumption for a start state
            processorAvailableForTau_i[i] = true;
            prevState_processorAvailableForTau_i[i] = true; // check this assumption !!!
        }
        
        interferred = new bool[n-1];
        for (int i = 0; i < n - 1; i++) interferred[i] = 0;
        
        pendJobsNum = 0;
        lockedJobsNum = 0;
        sumCs = 0;
        
        tauI_jobJustCompleted = new bool[n];
        for (int i = 0; i < n; i++) tauI_jobJustCompleted[i] = false;
        tauI_releaseNextJobBefore = new int_fast8_t[n];
        for (int i = 0; i < n; i++) tauI_releaseNextJobBefore[i] = -1;
    }
    
    // copy constructor
    state(const state& s) : n(s.n) {
        c = new unsigned short[n];
        p = new unsigned short[n];
        timePassedFromLastRelease = new unsigned short[n];
        wrts = new unsigned short[n];
        firstJobReleased = new bool[n];
        //releaseAtEarliest = new bool[n];
        
        for (int i = 0; i < n; i++) {
            c[i] = s.c[i];
            p[i] = s.p[i];
            timePassedFromLastRelease[i] = s.timePassedFromLastRelease[i];
            wrts[i] = s.wrts[i];
            firstJobReleased[i] = s.firstJobReleased[i];
            //releaseAtEarliest[i] = s.releaseAtEarliest[i];
        }

        jobCanBeReleasedBefore = new bool[n];
        processorAvailableForTau_i = new bool[n];
        prevState_processorAvailableForTau_i = new bool[n];
        for (int i = 0; i < n; i++) {
            jobCanBeReleasedBefore[i] = s.jobCanBeReleasedBefore[i];
            processorAvailableForTau_i[i] = s.processorAvailableForTau_i[i];
            prevState_processorAvailableForTau_i[i] = s.prevState_processorAvailableForTau_i[i];
        }
        
        interferred = new bool[n - 1];
        for (int i = 0; i < n-1; i++) interferred[i] = s.interferred[i];

        pendJobsNum = s.pendJobsNum;
        lockedJobsNum = s.lockedJobsNum;
        sumCs = s.sumCs;
        
        tauI_jobJustCompleted = new bool[n];
        for (int i = 0; i < n; i++) tauI_jobJustCompleted[i] = s.tauI_jobJustCompleted[i];
        tauI_releaseNextJobBefore = new int_fast8_t[n];
        for (int i = 0; i < n; i++) tauI_releaseNextJobBefore[i] = s.tauI_releaseNextJobBefore[i];
    }
    
    state& operator=(const state& s) {
        if(this != &s) {
            for(unsigned short i = 0; i < n; i++) {
                c[i] = s.c[i];
                p[i] = s.p[i];
                timePassedFromLastRelease[i] = s.timePassedFromLastRelease[i];
                wrts[i] = s.wrts[i];
                firstJobReleased[i] = s.firstJobReleased[i];
                //releaseAtEarliest[i] = s.releaseAtEarliest[i];
                jobCanBeReleasedBefore[i] = s.jobCanBeReleasedBefore[i];
            }
            
            for (unsigned short i = 0; i < n; i++) {
                processorAvailableForTau_i[i] = s.processorAvailableForTau_i[i];
                prevState_processorAvailableForTau_i[i] = s.prevState_processorAvailableForTau_i[i];
            }

            pendJobsNum = s.pendJobsNum;
            lockedJobsNum = s.lockedJobsNum;
            sumCs = s.sumCs;

            for (int i = 0; i < n; i++) tauI_jobJustCompleted[i] = s.tauI_jobJustCompleted[i];
            for (int i = 0; i < n; i++) tauI_releaseNextJobBefore[i] = s.tauI_releaseNextJobBefore[i];

            for (unsigned short i = 0; i < n-1; i++) interferred[i] = s.interferred[i];
        }
        return *this;
    }
    
    void updatePendJobsNum() {
        pendJobsNum = 0;
        for (int i = 0; i < n; i++) if (c[i] > 0) pendJobsNum++;
        return;
    }
    
    void updateLockedJobsNum() {
        lockedJobsNum = 0;
        for (int i = 0; i < n; i++) if (p[i] > 0) lockedJobsNum++;
        return;
    }
    
    void updateSumCs() {
        sumCs = 0;
        for (int i = 0; i < n; i++) sumCs += c[i];
        return;
    }
    
    void updateCounters() {
        
        pendJobsNum = 0;
        lockedJobsNum = 0;
        sumCs = 0;
        
        for (unsigned short i = 0; i < n; ++i) {
            if (p[i] > 0) {
                lockedJobsNum++;
                
                if (c[i] > 0) {
                    pendJobsNum++;
                    sumCs += c[i];
                }
            }
        }
        
        return;
    }

    
    
    
    
    bool operator<(const state& s) const {
        
        for (int i = n-1; i >= 0; i--) {
            if ((c[i] > 0) && (s.c[i] > 0)) {
            //if ((p[i] > 0) && (s.p[i] > 0)) {
                if (i == n-1) {
                    if (p[i] - wrts[i] > s.p[i] - s.wrts[i]) return true;
                    if (p[i] - wrts[i] < s.p[i] - s.wrts[i]) return false;
                } else {
                    if (p[i] - c[i] > s.p[i] - s.c[i]) return true;
                    if (p[i] - c[i] < s.p[i] - s.c[i]) return false;
                }
            }
        }
        
        if (lockedJobsNum < s.lockedJobsNum) return true;
        if (lockedJobsNum > s.lockedJobsNum) return false;
        if (pendJobsNum < s.pendJobsNum) return true;
        if (pendJobsNum > s.pendJobsNum) return false;
        
        
        /*for (int i = 0; i < PROC_NUM; i++) {
            if(c[i] > s.c[i]) return true;
            if(c[i] < s.c[i]) return false;
            if(p[i] < s.p[i]) return true; // false
            if(p[i] > s.p[i]) return false; // true
        }*/
        
        for (int i = n-1; i >= 0; i--) {
            if(c[i] > s.c[i]) return true;
            if(c[i] < s.c[i]) return false;
            if(p[i] < s.p[i]) return true; // false
            if(p[i] > s.p[i]) return false; // true
        }
        
        return false;
    }
    
    
    
    
    
    // Time to next deadline of a task
    inline int d(const unsigned short i, const unsigned short Pi, const unsigned short Di) const {
        return max(p[i] - (Pi - Di), 0);
    }
    
    // Printing functions
    void printCmpct(const TS& ts) const {
        cout << "{";
        for(int i = 0; i < n; i++) {
            cout << "(" << (int)c[i] << ", " << (int)d(i, ts.P[i], ts.D[i]) << ", " << (int)p[i] << ") ";
        }
        cout << "}";
        cout << endl;
    }
    
    void print(const TS& ts) const {
        cout << "==============" << endl;
        cout << "{";
        for(int i = 0; i < n; i++) {
            cout << "(" << (int)c[i] << ", " << (int)d(i, ts.P[i], ts.D[i]) << ", " << (int)p[i] << ") ";
        }
        cout << "}" << endl;
        cout << "pendJobsNum:\t" << (int)pendJobsNum << endl;
        cout << "canReleaseBefore:\t";
        for (int i = 0; i < n; i++) cout << "[" << i << "]: " << (int)jobCanBeReleasedBefore[i] << "\t";
        cout << endl;
        cout << "processorAvailable:\t";
        for (int i = 0; i < n; i++) cout << "[" << i << "]: " << (int)processorAvailableForTau_i[i] << "\t";
        cout << endl;
        cout << "procAvailablePrSt:\t";
        for (int i = 0; i < n; i++) cout << "[" << i << "]: " << (int)prevState_processorAvailableForTau_i[i] << "\t";
        cout << endl;
        cout << "=============" << endl << endl;
    }
    
    void print_until_tau_i(int indx, const TS& ts) const {
        cout << "==============" << endl;
        cout << "{";
        for(int i = 0; i <= indx; i++) {
            cout << "(" << c[i] << ", " << d(i, ts.P[i], ts.D[i]) << ", " << p[i] << ") ";
        }
        cout << "}" << endl;
        cout << "pendJobsNum:\t" << (int)pendJobsNum << endl;
        cout << "canReleaseBefore:\t";
        for (int i = 0; i <= indx; i++) cout << "[" << i << "]: " << (int)jobCanBeReleasedBefore[i] << "\t";
        cout << endl;
        cout << "processorAvailable:\t";
        for (int i = 0; i <= indx; i++) cout << "[" << i << "]: " << (int)processorAvailableForTau_i[i] << "\t";
        cout << endl;
        cout << "procAvailablePrSt:\t";
        for (int i = 0; i <= indx; i++) cout << "[" << i << "]: " << (int)prevState_processorAvailableForTau_i[i] << "\t";
        cout << endl;
        cout << "=============" << endl << endl;
    }
    
    // destructor
    ~state() {
        delete [] c;
        delete [] p;
        delete [] timePassedFromLastRelease;
        delete [] wrts;
        delete [] firstJobReleased;
        //delete [] releaseAtEarliest;
        delete [] interferred;
        delete [] jobCanBeReleasedBefore;
        delete [] processorAvailableForTau_i;
        delete [] prevState_processorAvailableForTau_i;
        delete [] tauI_jobJustCompleted;
        delete [] tauI_releaseNextJobBefore;
    }
};
