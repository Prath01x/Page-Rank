#include "random.h"
#include <stdio.h>
#include <stdlib.h>

void simulate_random_surfer(Graph *graph, int steps, int p) {
    

    // Initialize random starting node
    int current_node_index = randu(graph->num_nodes);
    Node *current_node = graph->nodes[current_node_index];

    // Array to count visits to each node
    int *visit_counts = calloc(graph->num_nodes, sizeof(int));

    // Simulate the random surfer for the specified number of steps
    for (int i = 0; i < steps; i++) {
        // With probability p%, jump to a random node
        if (randu(100) < p) {
            current_node_index = randu(graph->num_nodes);
            current_node = graph->nodes[current_node_index];
        } else if (current_node->out_degree > 0) {
            // With probability (100 - p)%, move to a randomly chosen adjacent node
            // This part assumes you have a way to iterate over the outgoing edges
            int random_outgoing_index = randu(current_node->out_degree);
            Node *next_node = NULL;
            int edge_count = 0;

            // Find the target node for the random outgoing edge
            for (int j = 0; j < graph->num_nodes; j++) {
                Node *potential_next_node = graph->nodes[j];
                // Assuming you have a way to check adjacency or out-degree nodes
                // Pseudo code: if (current_node is connected to potential_next_node)
                // Replace with your actual graph traversal logic
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
        printf("%s: %.2f%%\n", graph->nodes[i]->name, (double)visit_counts[i] / steps * 100);
    }

    free(visit_counts);
}
