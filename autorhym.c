#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "autorhym.h"

//make a function that generates a sine wave with decayed tail

//call the function whenever a rhythmic value is chosen

//perform the process only for the duration of 1 measure (adding multiple measures function later on)

void rhythm(double *y, int N, struct Automus *pam){
    int ND = pam->notedur;

    //make sure we know when to change notes
    int i = pam->counter;

    for (int n = 0; n < N; n++){
        if (i > ND){
            sine(y, N, pam);
            rhym_choose(pam);
            printf("Chosen\n");
            i = 0;
        }
        i++;
    }
}

void sine(double *y, int N, struct Automus *pam){
    int fs;
    double ampl, attack_factor, decay_factor, drop_level, attack_amp, decay_amp;
    float f0, phase, phase_inc;

    //pass data from flanger structure to local variables
    fs = pam->fs;
    f0 = pam->sine_f0;
    phase = pam->sine_phase;
    phase_inc = 2*PI*f0/fs;
    ampl = FS_AMPL;
    attack_factor = ATTACK_FACTOR;
    decay_factor = DECAY_FACTOR;
    drop_level = DROP_LEVEL;
    attack_amp = 1.0;
    decay_amp = 1.0;

    //our sampled sine wave value for each instance
    double v;

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
    }
    //keep track of what counter and phase position we are at
    pam->sine_phase = phase;
}

void rhym_choose(struct Automus *pam){
    int subdivisions = pam->subdivisions; //smallest rhythmic value
    int bog = pam->bog; //original beat value in samples
    float beatsub = pam->beatsub; //fraction of bog
    float rand = pam->rand; //random number generator

    //get val by sampling lfo, pink noise, input wav file, or ANYTHING
    float val = rand/100;
    // printf("Val: %f\n",val);

    switch (subdivisions)
    {
    case 1:
        //quarter notes
        beatsub = 1;

        pam->notedur = beatsub * bog;

        break;
    case 2:
        //eighth notes
        if (-1.0 <= val && val < 0.0){
            beatsub = 1/subdivisions;
        } else if (0.0 <= val && val <= 1.0){
            beatsub = 2/subdivisions;
        }

        pam->notedur = beatsub * bog;

        break;
    case 3:
        //triplets
        if (-1.0 <= val && val < -0.33){
            beatsub = 1/subdivisions;
        } else if (-0.33 <= val && val < 0.33){
            beatsub = 2/subdivisions;
        } else if (0.33 <= val && val <= 1.0){
            beatsub = 3/subdivisions;
        }

        pam->notedur = beatsub * bog;

        break;
    case 4:
        //sixteenth notes
        if (-1.0 <= val && val < -0.5){
            beatsub = 1/subdivisions;
        } else if (-0.5 <= val && val < 0.0){
            beatsub = 2/subdivisions;
        } else if (0.0 <= val && val < 0.5){
            beatsub = 3/subdivisions;
        } else if (0.5 <= val && val <= 1.0){
            beatsub = 4/subdivisions;
        }

        pam->notedur = beatsub * bog;

        break;
    default:
        printf("Chaos ensues\n");
        break;
    }
}