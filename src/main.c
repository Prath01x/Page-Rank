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
#include <string.h>

void print_usage() {
    printf("Usage: ./pagerank [OPTIONS]... [FILENAME]\n");
    printf("Perform pagerank computations for a given file in the DOT format\n");
    printf("  -h        Print a brief overview of the available command line parameters\n");
    printf("  -r N      Simulate N steps of the random surfer and output the result\n");
    printf("  -m N      Simulate N steps of the Markov chain and output the result\n");
    printf("  -s        Compute and print the statistics of the graph as defined in section 3.4\n");
    printf("  -p P      Set the parameter p to P%%. (Default: P = 10)\n");
}
void simulate_random_surfer(Graph *graph, int steps, float p) {
    rand_init();

    if (graph == NULL || graph->num_nodes == 0 || steps <= 0 || p < 0.0 || p > 1.0) {
        return;
    }

    int *visit_counts = calloc(graph->num_nodes, sizeof(int));
    if (!visit_counts) {
        return;
    }

    int current_node_index = randu(graph->num_nodes);
    Node *current_node = graph->nodes[current_node_index];

    for (int i = 0; i < steps; i++) {
        if ((float)randu(RAND_MAX) / (float)RAND_MAX < p || current_node->out_degree == 0) {
            current_node_index = randu(graph->num_nodes);
            current_node = graph->nodes[current_node_index];
        } else {
            int random_outgoing_index = randu(current_node->out_degree);
            int count = 0;
            for (int j = 0; j < graph->num_nodes; j++) {
                Node *potential_next_node = graph->nodes[j];
                if (strcmp(potential_next_node->name, current_node->name) != 0) {
                    count += potential_next_node->in_degree;
                    if (count > random_outgoing_index) {
                        current_node = potential_next_node;
                        current_node_index = j;
                        break;
                    }
                }
            }
        }
        visit_counts[current_node_index]++;
    }

    for (int i = 0; i < graph->num_nodes; i++) {
        printf("%s %.10f\n", graph->nodes[i]->name, (double)visit_counts[i] / steps);
    }

    free(visit_counts);
}

int main(int argc, char *argv[]) {
    
    int opt;
    int random_steps;
    int stats_flag = 0;
    
    float p = 0.10; 
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "hr:sp:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'r':
                random_steps = atoi(optarg);
                break;
            case 's':
                stats_flag = 1;
                break;
            case 'p':
            p=atof(optarg)/100.0;
                break;
            default:
                print_usage();
                return 1;
        }
    }

    if (optind < argc) {
        filename = argv[optind];
    } else if (!stats_flag) {
        print_usage();
        return 0;
    }

    if (filename) {
        Graph *graph = create_graph();
       int x= parse_dot_file(filename, graph);
       if (x==1){
        free_graph(graph);
        return 1;
       }
        if (stats_flag) {
            print_graph_stats(graph);
        }

        else if (random_steps > 0) {
            simulate_random_surfer(graph, random_steps, p);
        }

        free_graph(graph);
    }

    return 0;

}
