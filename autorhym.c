#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "autorhym.h"

//make a function that generates a sine wave with decayed tail

//call the function whenever a rhythmic value is chosen

//perform the process only for the duration of 1 measure (adding multiple measures function later on)

void sine(double *y, int N, struct Automus *pam){
    int fs, i, k, ND, MD;
    double ampl, attack_factor, decay_factor, drop_level, attack_amp, decay_amp;
    float f0, phase, phase_inc;

    //pass data from flanger structure to local variables
    fs = pam->fs;
    f0 = pam->sine_f0;
    phase = pam->sine_phase;
    phase_inc = 2*PI*f0/fs;
    ND = pam->notedur;
    MD = pam->measuredursamp;

    //make sure we know when to change notes
    i = pam->notecounter;
    k = pam->meascounter;

    if (k < FRAMES_PER_BUFFER){
        ampl = AMPL_LOUD;
    } else {
        ampl = AMPL_SOFT;
    }
    attack_factor = ATTACK_FACTOR;
    decay_factor = DECAY_FACTOR;
    drop_level = DROP_LEVEL;
    attack_amp = pam->attack_amp;
    decay_amp = pam->decay_amp;

    //our sampled sine wave value for each instance
    double v;

    for (int n = 0; n < N; n++){
        v = 0;
        if (phase_inc > -1){
            v += ampl * sin(phase);
            phase += phase_inc;
        }
        v *= (1.0 - attack_amp) * (decay_amp);
        decay_amp *= DECAY_FACTOR;
        attack_amp *= ATTACK_FACTOR;
        if (decay_amp < DROP_LEVEL){
            v *= 0;
        }
        y[n] = v;

        if (k > MD){
            //note_choose(pam);
            choose(pam);
            i = 0;
            k = 0;
            attack_amp = 1.0;
            decay_amp = 1.0;
            printf("MEASURE\n-------\n");
            break;
        }

        if (i > ND){
            //note_choose(pam);
            choose(pam);
            i = 0;
            attack_amp = 1.0;
            decay_amp = 1.0;
            break;
        }
        i++;
        k++;
    }
    //keep track of what counter and phase position we are at
    pam->notecounter = i;
    pam->meascounter = k;
    pam->attack_amp = attack_amp;
    pam->decay_amp = decay_amp;
    pam->sine_phase = phase;
}

void choose(struct Automus *pam){
    int subdivisions = pam->subdivisions; //smallest rhythmic value
    int bog = pam->bog; //original beat value in samples
    float beatsub;

    float f0 = pam->sine_f0; //current note
    float i0 = pam->i0; //smallest interval
    float ibig = pam->ibig; //biggest interval
    int range = pam->range; //allowed range of melody
    float fog = pam->fog; //original chosen starting frequency

    //get val by sampling lfo, pink noise, input wav file, or ANYTHING
    float val = (rand() % 200 - 100)/100.0;

    //if gap between f0 and range boundaries is small, change big, is gap is big, change small
    if (f0 >= fog * pow(2,(((range/2)-i0)/12))){
        f0 *= powf(2,(-ibig/12));
    } else if (f0 >= fog * pow(2,(((range/2)-ibig)/12))){
        f0 *= powf(2,(-i0/12));
    } else if (f0 <= fog * pow(2,(((range/-2)+i0)/12))){
        f0 *= powf(2,(ibig/12));
    } else if (f0 <= fog * pow(2,(((range/-2)+ibig)/12))){
        f0 *= powf(2,(i0/12));
    } else if (-1 <= val && val < -0.5){ //if gap is not within range boundaries, proceed as normal
        f0 *= powf(2,(-i0/12));
    } else if (-0.5 <= val && val < 0){
        f0 *= powf(2,(-ibig/12));
    } else if (0 <= val && val < 0.5){
        f0 *= powf(2,(i0/12));
    } else if (0.5 <= val && val <= 1){
        f0 *= powf(2,(ibig/12));
    } else {
        printf("Frequency chaos ensues\n");
    }

    //replace sine tone for new frequency
    pam->sine_f0 = f0;
    printf("New f0: %fHz\n-------\n",f0);

    int randologna = rand() % 12 + 1;

    //printf("Rand is: %d\n",randologna);

    switch (subdivisions)
    {
    case 1:
        //quarter notes
        beatsub = 1.0;

        pam->notedur = floor(bog/beatsub);
        // printf("1/%f Note\n", (beatsub*4));
        break;
    case 2:
        //eighth notes
        if (randologna % 2 == 0){
            beatsub = 2.0;
        } else {
            beatsub = 1.0;
        }

        pam->notedur = floor(bog/beatsub);
        // printf("1/%f Note\n", (beatsub*4));
        break;
    case 3:
        //triplets
        if (randologna % 3 == 0){
            beatsub = 3.0;
        } else if (randologna % 2 == 0){
            beatsub = 1.5;
        } else {
            beatsub = 1.0;
        }

        pam->notedur = floor(bog/beatsub);
        // printf("1/%f Note\n", (beatsub*4));
        break;
    case 4:
        //sixteenth notes
        if (randologna % 4 == 0){
            beatsub = 4.0;
        } else if (randologna % 3 == 0){
            beatsub = 2.0;
        } else if (randologna % 2 == 0){
            beatsub = 4.0/3.0;
        } else {
            beatsub = 1.0;
        }

        pam->notedur = floor(bog/beatsub);
        // printf("1/%f Note\n", (beatsub*4));
        break;
    default:
        printf("Rhythmic chaos ensues\n");
        break;
    }
}