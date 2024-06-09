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
void simulate_random_surfer(Graph *graph, int steps, float p) {
    // Check if the graph has any nodes
    rand_init();
     if (graph == NULL || graph->num_nodes == 0 || steps <= 0 || p < 0.0 || p > 1.0) {
        return;
    }
    
    // Initialize random starting node
    int current_node_index = randu(graph->num_nodes);
    Node *current_node = graph->nodes[current_node_index];

    // Array to count visits to each node
    int *visit_counts = calloc(graph->num_nodes, sizeof(int));
    if (!visit_counts) {
        
        return;
    }

    // Simulate the random surfer for the specified number of steps
    for (int i = 0; i < steps; i++) {
        // With probability p%, jump to a random node
        if (randu(RAND_MAX)/ RAND_MAX < p) {
            current_node_index = randu(graph->num_nodes);
            current_node = graph->nodes[current_node_index];
        } else if (current_node->out_degree > 0) {
            // With probability (100 - p)%, move to a randomly chosen adjacent node
            int random_outgoing_index = randu(current_node->out_degree);
            Node *next_node = NULL;
            int edge_count = 0;

            // Find the target node for the random outgoing edge
            for (int j = 0; j < graph->num_nodes; j++) {
                Node *potential_next_node = graph->nodes[j];
                for (int k = 0; k < potential_next_node->in_degree; k++) {
                    if (edge_count == random_outgoing_index) {
                        next_node = potential_next_node;
                        break;
                    }
                    edge_count++;
                }
                if (next_node) break;
            }

            if (next_node) {
                current_node = next_node;
            }
        }

        // Count the visit
        visit_counts[current_node_index]++;
    }

    // Output the results
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("%s %.10f\n", graph->nodes[i]->name, (double)visit_counts[i] / steps);
    }

    free(visit_counts);
}


int main(int argc, char *argv[]) {
    
    int opt;
    int random_steps;
    int stats_flag = 0;
    
    float p = 0.1; 
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
                p = atoi(optarg) / 100.0;
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
        return 1;
    }

    if (filename) {
        Graph *graph = create_graph();
        int parse_result = parse_dot_file(filename, graph);

        if (parse_result == 1) {
            return 1; // Parsing error
        }

        if (stats_flag) {
            print_graph_stats(graph);
        } else if (random_steps > 0) {
            simulate_random_surfer(graph, random_steps, p);
        }

        free_graph(graph);
    }

    return 0;
}

