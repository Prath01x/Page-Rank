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
            // Random jump to any node
            current_node_index = randu(graph->num_nodes);
            current_node = graph->nodes[current_node_index];
        } else {
            // Randomly select one of the outgoing edges
            int random_outgoing_index = randu(current_node->out_degree);
            Edge *edge = current_node->edges;
            for (int j = 0; j < random_outgoing_index && edge != NULL; j++) {
                edge = edge->next;
            }
            if (edge != NULL) {
                current_node = edge->target;
                current_node_index = -1;
                for (int k = 0; k < graph->num_nodes; k++) {
                    if (graph->nodes[k] == current_node) {
                        current_node_index = k;
                        break;
                    }
                }
            }
        }
        visit_counts[current_node_index]++;
    }

    for (int i = 0; i < graph->num_nodes; i++) {
        printf("%s %.10f\n", graph->nodes[i]->name, (float)visit_counts[i] / steps);
    }

    free(visit_counts);
}
void msp1(Graph *graph, int msteps, float p) {
      // Convert percentage to a probability

    if (graph == NULL || graph->num_nodes == 0 || msteps <= 0 || p < 0.0 || p > 1.0) {
        return;
    }

    int n = graph->num_nodes;
    double *pi = calloc(n, sizeof(double));
    double *pi_next = calloc(n, sizeof(double));

    for (int i = 0; i < n; i++) {
        pi[i] = 1.0 / n;  // Initial uniform distribution
    }

    for (int step = 0; step < msteps; step++) {
        for (int i = 0; i < n; i++) {
            pi_next[i] = 0.0;  // Reset next state probabilities
        }

        for (int j = 0; j < n; j++) {
            Node *current_node = graph->nodes[j];
            if (current_node->out_degree == 0) {
                // If no outgoing edges, distribute probability equally to all nodes
                for (int k = 0; k < n; k++) {
                    pi_next[k] += pi[j] / n;
                }
            } else {
                Edge *edge = current_node->edges;
                while (edge != NULL) {
                    Node *target_node = edge->target;
                    int target_index = -1;
                    for (int k = 0; k < n; k++) {
                        if (graph->nodes[k] == target_node) {
                            target_index = k;
                            break;
                        }
                    }
                    if (target_index != -1) {
                        pi_next[target_index] += (1.0 - p) * (pi[j] / current_node->out_degree);
                    }
                    edge = edge->next;
                }
            }
        }
        // Apply random jump factor
        for (int k = 0; k < n; k++) {
            pi_next[k] += p / n;
        }

        // Normalize to prevent numerical instability
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            sum += pi_next[i];
        }
        for (int i = 0; i < n; i++) {
            pi_next[i] /= sum;
        }

        memcpy(pi, pi_next, n * sizeof(double));
    }

    for (int i = 0; i < n; i++) {
        printf("%s %.10f\n", graph->nodes[i]->name, pi[i]);
    }

    free(pi);
    free(pi_next);
}

int main(int argc, char *argv[]) {
    
    int opt;
    int random_steps=0;
    int msteps=0;
    int stats_flag = 0;
    
    
    float p = 0.10; 
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "hr:m:sp:")) != -1) {
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
            case 'm':
                
                msteps=atoi(optarg);
                if(msteps==0){
                    return 1;
                }
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

        if (random_steps > 0) {
            simulate_random_surfer(graph, random_steps, p);
        }
        else if(msteps>0&&p==0){
            return 1;
        }
        else if(msteps>0){
            msp1(graph,msteps,p);
        }
       
       
        

        free_graph(graph);
    }

    return 0;

}