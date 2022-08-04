#define MAX_CHAN	1
#define MAX_COEF	256
#define FRAMES_PER_BUFFER   1024
#define PI       3.14159265358979323846 //pi
#define SoS        343.0 //speed of sound, meters per second
#define BUF_LEN  8*1024
#define BUF_MID  (BUF_LEN/2)
#define FS_AMPL             0.5 /* -6 dB FS */
#define ATTACK_FACTOR       0.99800 /* attack time constant of 10 ms */
#define DECAY_FACTOR        0.99980 /* decay time constant of 1.0 sec */
#define DROP_LEVEL          0.001  /* -60 dBFS */

struct Automus {
    float output[FRAMES_PER_BUFFER];
    float fs; //sampling freq, Hz
    float buffer[BUF_LEN]; //delay buffer

    int beatsperbar; //user-defined meter
    int nummeasures; //number of measures
    int subdivisions; //user-defined subdivisions of a beat
    int bpm; //user-defined rate of new note generation

    int i0; //the lower of the two chosen intervals
    int ibig; //higher of the chosen intervals
    int i1; //user-defined first interval
    int i2; //user-defined second interval
    int range; //user-defined frequency range of random note generation
    float sine_f0; //user-defined starting frequency, is updated as random notes are generated
    float sine_phase;
    float sine_phase_inc;

    double ampl;
    float beatsub; //fraction of subdivision of the beat relative to bog
    int bog; //original beat value in samples
    float fog; //starting frequency kept in memory for range calculation

    int notedur; //duration of one note in samples
    int measuredursamp; //length of measure in samples
    float measuredursec; //duration of each measure in seconds
    float totaldur; //total clip duration in samples
    int notecounter; //keeps track of how many samples have passed for a given note duration
    int meascounter;

    double attack_factor;
    double decay_factor;
    double attack_amp; /* save attack amplitude for next sample */
    double decay_amp; /* save decay amplitude for next sample */
};

//function prototypes
//void rhythm(double *y, int N, struct Automus *pam);
void sine(double *y, int N, struct Automus *pam);
void note_choose(struct Automus *pam);
void rhym_choose(struct Automus *pam);
int user_beats();
int user_meas();
int user_subdivisions();
int user_bpm();
int user_i1();
int user_i2();
int user_fstart();
int user_mrange();