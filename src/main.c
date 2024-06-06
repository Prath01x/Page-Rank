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
    int stats_flag = 0;
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "hs")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 's':
                stats_flag = 1;
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
            print_graph_statastics(graph);
        }
        free_graph(graph);
    }

    return 0;
}
