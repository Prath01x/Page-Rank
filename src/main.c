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
Node* get_node_by_index(Graph *graph, int index) {
    if (index >= 0 && index < graph->num_nodes) {
        return graph->nodes[index];
    }
    return NULL;
}

int get_index_by_node(Graph *graph, Node *node) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[i] == node) {
            return i;
        }
    }
    return -1; // Node not found
}

void simulate_random_surfer(Graph *graph, int steps, double p) {
    if (graph == NULL || graph->num_nodes == 0 || steps <= 0 || p < 0.0 || p > 1.0) {
        return;
    }

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize random starting node
    int current_node_index = randu(graph->num_nodes);
    Node *current_node = get_node_by_index(graph, current_node_index);

    // Array to count visits to each node
    int *visit_counts = calloc(graph->num_nodes, sizeof(int));
    if (visit_counts == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Simulate the random surfer for the specified number of steps
    for (int i = 0; i < steps; i++) {
        // Count the visit
        visit_counts[current_node_index]++;

        // With probability (1 - p), move to a randomly chosen adjacent node
        if ((double)randu(100) / 100.0 >= p && current_node->out_degree > 0) {
            int random_outgoing_index = randu(current_node->out_degree);
            Node *next_node = current_node->next;

            // Traverse to the randomly chosen adjacent node
            for (int j = 0; j < random_outgoing_index; j++) {
                if (next_node->next != NULL) {
                    next_node = next_node->next;
                }
            }

            current_node = next_node;
            current_node_index = get_index_by_node(graph, current_node);
        } else {
            // With probability p, jump to a random node
            current_node_index = randu(graph->num_nodes);
            current_node = get_node_by_index(graph, current_node_index);
        }
    }

    // Output the results as probabilities
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("%s: %.4f\n", graph->nodes[i]->name, (double)visit_counts[i] / steps);
    }

    // Free allocated memory
    free(visit_counts);
}

int main(int argc, char *argv[]) {
    rand_init();
    int opt;
    int random_steps;
    int stats_flag = 0;
    
    int p = 10; // Default value for p
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
                p = atoi(optarg);
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

