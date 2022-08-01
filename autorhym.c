#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "autorhym.h"

//make a function that generates a sine wave with decayed tail

//call the function whenever a rhythmic value is chosen

//perform the process only for the duration of 1 measure (adding multiple measures function later on)

void sine(double *y, int N, struct Automus *pam){
    int fs, bpm, i, ND;
    double ampl;
    float f0, phase, phase_inc;

    //pass data from flanger structure to local variables
    fs = pam->fs;
    f0 = pam->sine_f0;
    phase = pam->sine_phase;
    phase_inc = 2*PI*f0/fs;
    ampl = FS_AMPL;
    bpm = pam->bpm;
    ND = pam->measuredur;

    //our sampled sine wave value for each instance
    double v;

    //make sure we know when to change notes
    i = pam->counter;

    for (int n = 0; n < N; n++){
        v = 0;
        if (phase_inc > -1){
            v += ampl * sin(phase);
            phase += phase_inc;
        }
        y[n] = v;
        if (i > ND){
            choose(pam);
            i = 0;
        }
        i++;
    }
    //keep track of what counter and phase position we are at
    pam->counter = i;
    pam->sine_phase = phase;
}

void choose(struct Automus *pam){
    float f0 = pam->sine_f0; //current note
    float i0 = pam->i0; //smallest interval
    float ibig = pam->ibig; //biggest interval
    int range = pam->range; //allowed range of melody
    float fog = pam->fog; //original chosen starting frequency
    float rand = pam->rand; //random number generator

    //get val by sampling lfo, pink noise, input wav file, or ANYTHING
    float val = rand/100;
    // printf("Val: %f\n",val);

    //if gap between f0 and range boundaries is small, change big, is gap is big, change small
    if (f0 >= fog * pow(2,(((range/2)-i0)/12))){
        f0 *= powf(2,(-ibig/12));
        // printf("1 New f0: %f\n",f0);
    } else if (f0 >= fog * pow(2,(((range/2)-ibig)/12))){
        f0 *= powf(2,(-i0/12));
        // printf("2 New f0: %f\n",f0);
    } else if (f0 <= fog * pow(2,(((range/-2)+i0)/12))){
        f0 *= powf(2,(ibig/12));
        // printf("3 New f0: %f\n",f0);
    } else if (f0 <= fog * pow(2,(((range/-2)+ibig)/12))){
        f0 *= powf(2,(i0/12));
        // printf("4 New f0: %f\n",f0);
    } else if (-1 <= val && val < -0.5){ //if gap is not within range boundaries, proceed as normal
        f0 *= powf(2,(-i0/12));
        // printf("5 New f0: %f\n",f0);
    } else if (-0.5 <= val && val < 0){
        f0 *= powf(2,(-ibig/12));
        // printf("6 New f0: %f\n",f0);
    } else if (0 <= val && val < 0.5){
        f0 *= powf(2,(i0/12));
        // printf("7 New f0: %f\n",f0);
    } else if (0.5 <= val && val <= 1){
        f0 *= powf(2,(ibig/12));
        // printf("8 New f0: %f\n",f0);
    } else {
        printf("Chaos ensues\n");
    }

    //warning if outside of range
    if (f0 < fog * pow(2,(((range/-2)-1)/12))){
        printf("Too low\n");
    } else if (f0 > fog * pow(2,(((range/2)+1)/12))) {
        printf("Too high\n");
    }

    //replace sine tone for new frequency
    pam->sine_f0 = f0;
    printf("New f0: %fHz\n",f0);
}