#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autorhym.h"

//fgets() and printf() user I/O
#define LEN_STR 80

//clears screen using ANSI escape sequences
void clear(){
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void print_info(char *msg){
    //user I/O using printf() and getchar()
    clear();
    printf("Welcome to my Automated Music Generation Experience!\n"); //line 0
    printf("Enter an interval size below\n");
    printf("%s", msg);
    fflush(stdout);
}

int user_beats(){
    char in_str[LEN_STR]; //user input string
    print_info("Enter the number of beats in a measure: ");

    //read input line, including trailing \n
    fgets(in_str, LEN_STR, stdin); //wait for input
    
    int user_beats = atoi(in_str);
    clear();

    return user_beats;
}

int user_meas(){
    char in_str[LEN_STR]; //user input string
    print_info("Enter the total number of measures: ");

    //read input line, including trailing \n
    fgets(in_str, LEN_STR, stdin); //wait for input
    
    int user_meas = atoi(in_str);
    clear();

    return user_meas;
}

int user_subdivisions(){
    char in_str[LEN_STR]; //user input string
    print_info("Enter the rhythmic division factor: ");

    //read input line, including trailing \n
    fgets(in_str, LEN_STR, stdin); //wait for input

    int user_subdivisions = atoi(in_str);
    clear();

    return user_subdivisions;
}

int user_bpm(){
    char in_str[LEN_STR]; //user input string
    print_info("Enter beats per minute: ");

    //read input line, including trailing \n
    fgets(in_str, LEN_STR, stdin); //wait for input

    int user_bpm = atoi(in_str);
    clear();

    return user_bpm;
}