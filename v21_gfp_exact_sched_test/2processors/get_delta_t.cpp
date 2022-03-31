#include <iostream>
#include <climits>
#include "custom_types/ts.h"
#include "custom_types/state.h"




bool is_processor_available_to_tau_i_dt(const state& s, const unsigned short i, const unsigned short m) {
    
    unsigned short hpJobsNum = 0;
    
    for (unsigned short indx = 0; indx < i; indx++) {
        if (s.c[indx] > 0) {
            hpJobsNum++;
            if (hpJobsNum == m) return false;
        }
    }

    return true;
}




unsigned short get_dt_to_next_completion(const state& s, const unsigned short m, const unsigned short *perm){

    unsigned short dtToNextCompletion = 255;
    
    for (unsigned short i = 0; i < m; i++) {
        if (s.c[perm[i]] == 1) {dtToNextCompletion = 1; break;}
        else if (s.c[perm[i]] > 1) dtToNextCompletion = min((int)dtToNextCompletion, (int)(s.c[perm[i]]));
    }
    
    return dtToNextCompletion;
}




unsigned short get_dt_processor_available_to_tau_i(const state& s, const unsigned short m, const unsigned short taskIndx) {

    unsigned short* cs = new unsigned short[s.n];
    
    // assumption: at least m higher priority jobs at state s
    unsigned short n_hp = 0;
    for (unsigned short i = 0; i < taskIndx; i++) {
        if (s.c[i] > 0) {
            cs[n_hp] = s.c[i];
            n_hp++;
        }
    }
    sort(cs, cs + n_hp);

    unsigned short dtResump = cs[n_hp - m + 1];
    delete [] cs;
    
    return dtResump;
}





unsigned short get_dt_to_next_release(const state& s, const unsigned short m){
    
    unsigned short dtToNextRelease = max(1, (int)(s.p[0]));
    if (dtToNextRelease == 1) return dtToNextRelease;
    
    unsigned short dtToTimeWithLessThanMHPjobs;
    for (unsigned short i = 1; i <= s.n-1; i++) {
        
        if (i < m) {
            if (s.p[i] <= 1) {dtToNextRelease = 1; return dtToNextRelease;}
            else dtToNextRelease = min((int)(s.p[i]), (int)dtToNextRelease);

        } else if (i <= s.n-1) {
            // The corollary from a critical release instant condition for tau_i:
            // If tau_i, with i > m, releases a job at time $t$, then:
            // a processor should be available for tau_i at time $t-1$, if tau_i would release a job at time $t-1$
            //if (s.processorAvailableForTau_i[i]) {
            if (is_processor_available_to_tau_i_dt(s, i, m)) {
                dtToNextRelease = min((int)dtToNextRelease, max(1, (int)s.p[i]));
                if (dtToNextRelease == 1) return dtToNextRelease;
                else continue;
            } else { // s.processorAvailableForTau_i[i] == false
                dtToTimeWithLessThanMHPjobs = get_dt_processor_available_to_tau_i(s, m, i);
                dtToNextRelease = min((int)dtToNextRelease, ((int)dtToTimeWithLessThanMHPjobs + 1));
                if (dtToNextRelease == 1) return dtToNextRelease;
                else continue;
            }
            
        } /*else if (i == s.n-1) {
            // The corollary from Davis and Burns 2011 condition
            // for a critical release instant for an examined task tau_n:
            // If at time $t$ there is less than m pending jobs, then tau_n cannot release a job at time $t+1$
            // Assumption: an examined tau_n releases only one job in considered schedules
            if ((s.pendJobsNum < m) && (s.p[i] == 0)) {dtToNextRelease = 1; return dtToNextRelease;}
            else if ((s.pendJobsNum >= m) && (s.p[i] == 0)) dtToNextRelease = min((int)dtToNextRelease, ((int)dtToNextCompletion + 1));
            else dtToNextRelease = min((int)(s.p[i]), (int)dtToNextRelease);
        }*/
    }
    
    return dtToNextRelease;
}




unsigned short deltaT(const state& s, const unsigned short m, const unsigned short *perm) {
    
    unsigned short dtToNextCompletion = get_dt_to_next_completion(s, m, perm);
    
    if (dtToNextCompletion == 1) return dtToNextCompletion;
    else {
        unsigned short dtToNextRelease = get_dt_to_next_release(s, m);
        return min((int)dtToNextRelease, (int)dtToNextCompletion);
    }
}
