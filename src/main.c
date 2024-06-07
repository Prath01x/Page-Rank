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
#include "parser.h"
#include "random.h"

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
    rand_init();
    int opt;
    int random_steps;
    int stats_flag = 0;
    
    int p = 10; // Default value for p
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "hr:msp")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'r':
                random_steps = optarg;
                break;
            case 's':
                stats_flag = 1;
                break;
            case 'p':
                p = optarg;
                break;
            default:
                print_usage();
                return 1;
        }
    }

    if (optind < argc) {
        filename = argv[optind];
    } else if (!stats_flag) {
        fprintf(stderr, "Filename required\n");
        print_usage();
        return 1;
    }

    if (filename) {
        Graph *graph = create_graph();
        parse_dot_file(filename, graph);

        if (stats_flag) {
            print_graph_stats(graph);
        }

        if (random_steps > 0) {
            simulate_random_surfer(graph, random_steps, p);
        }

        free_graph(graph);
    }

    return 0;
}

