#define MAX_CHAN	1
#define MAX_COEF	256
#define FRAMES_PER_BUFFER   1024
#define PI       3.14159265358979323846 //pi
#define SoS        343.0 //speed of sound, meters per second
#define BUF_LEN  8*1024
#define BUF_MID  (BUF_LEN/2)
#define FS_AMPL             0.5 /* -6 dB FS */

struct Automus {
    float output[FRAMES_PER_BUFFER];
    float fs; //sampling freq, Hz
    float buffer[BUF_LEN]; //delay buffer
    int beatsperbar; //user-defined meter
    int subdivisions; //user-defined subdivisions of a beat
    int bpm; //user-defined rate of new note generation
    float sine_f0; //user-defined starting frequency, is updated as random notes are generated
    float sine_phase;
    float sine_phase_inc;
    double ampl;
    int measuredur; //duration of each measure in seconds
    int counter; //keeps track of how many samples have passed for a given note duration
    float rand; //random number used to calculate new note
};

//function prototypes
void sine(double *y, int N, struct Automus *pam);
void choose(struct Automus *pam);
int user_beats();
int user_subdivisions();
int user_bpm();