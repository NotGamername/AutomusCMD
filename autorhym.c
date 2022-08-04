#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "autorhym.h"

//make a function that generates a sine wave with decayed tail

//call the function whenever a rhythmic value is chosen

//perform the process only for the duration of 1 measure (adding multiple measures function later on)

void sine(double *y, int N, struct Automus *pam){
    int fs, i, ND;
    double ampl, attack_factor, decay_factor, drop_level, attack_amp, decay_amp;
    float f0, phase, phase_inc;

    //pass data from flanger structure to local variables
    fs = pam->fs;
    f0 = pam->sine_f0;
    phase = pam->sine_phase;
    phase_inc = 2*PI*f0/fs;
    ND = pam->notedur;

    ampl = FS_AMPL;
    attack_factor = ATTACK_FACTOR;
    decay_factor = DECAY_FACTOR;
    drop_level = DROP_LEVEL;
    attack_amp = 1.0;
    decay_amp = 1.0;

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
        v *= (1 - attack_amp) * (decay_amp);
        decay_amp *= DECAY_FACTOR;
        attack_amp *= ATTACK_FACTOR;
        if (decay_amp < DROP_LEVEL){
            v *= 0;
        }
        y[n] = v;

        if (i > ND){
            rhym_choose(pam);
            i = 0;
        }
        i++;
    }
    //keep track of what counter and phase position we are at
    pam->counter = i;
    pam->sine_phase = phase;
}

void rhym_choose(struct Automus *pam){
    int subdivisions = pam->subdivisions; //smallest rhythmic value
    int bog = pam->bog; //original beat value in samples
    // float beatsub = pam->beatsub; //fraction of bog
    float beatsub;

    int randologna = rand() % 12 + 1;

    printf("Rand is: %d\n",randologna);

    switch (subdivisions)
    {
    case 1:
        //quarter notes
        beatsub = 1.0;

        pam->notedur = floor(bog/beatsub);
        printf("Note Duration is: %d\n", pam->notedur);

        break;
    case 2:
        //eighth notes
        if (randologna % 2 == 0){
            beatsub = 2.0;
        } else {
            beatsub = 1.0;
        }

        pam->notedur = floor(bog/beatsub);
        printf("Note Duration is: %d\n", pam->notedur);

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
        printf("Note Duration is: %d\n", pam->notedur);

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
        printf("Note Duration is: %d\n", pam->notedur);

        break;
    default:
        printf("Chaos ensues\n");
        break;
    }
}