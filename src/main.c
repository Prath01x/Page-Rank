/*
 * main.c
 *
 * Programming 2 - Project 3 (PageRank)
 */

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include <stdlib.h>
#include <getopt.h>

void print_usage() {
    printf("Usage: ./pagerank [OPTIONS]... [FILENAME]\n");
    printf("Perform pagerank computations for a given file in the DOT format\n");
    printf("  -h        Print a brief overview of the available command line parameters\n");
    printf("  -r N      Simulate N steps of the random surfer and output the result\n");
    printf("  -m N      Simulate N steps of the Markov chain and output the result\n");
    printf("  -s        Compute and print the statistics of the graph as defined in section 3.4\n");
    printf("  -p P      Set the parameter p to P%%. (Default: P = 10)\n");
}

int main(int argc, char *argv[]) {
    int opt;
    void rand_init();
    while ((opt = getopt(argc, argv, "hr:m:sp:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            // Handle other flags here
            default:
                fprintf(stderr, "Usage: %s [-h] [-r N] [-m N] [-s] [-p P] [FILENAME]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    return 0;
}


  // initialize the random number generator
 


