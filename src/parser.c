#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


int parse_dot_file(const char* filename, Graph* graph) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    char line[512];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        // Extract the graph name from the first line containing "digraph"
        if (strstr(line, "digraph")) {
            sscanf(line, "digraph %s {", graph->name);
            // Remove any trailing '{' or whitespace from the name
            char* pos = strchr(graph->name, '{');
            if (pos) {
                *pos = '\0';
            }
            continue;
        }

        // Skip lines with '{' and '}'
        if (strstr(line, "}")) {
            continue;
        }

        char source[MAX_NAME_LENGTH], target[MAX_NAME_LENGTH];
        if (sscanf(line, " %[^-> ] -> %[^; ];", source, target) == 2) {
            if (strlen(source) > MAX_NAME_LENGTH || strlen(target) > MAX_NAME_LENGTH) {
                return 1; // Identifier too long
            }
            Node* src_node = find_or_create_node(graph, source);
            Node* tgt_node = find_or_create_node(graph, target);
            src_node->out_degree++;
            tgt_node->in_degree++;
            graph->num_edges++;
        } else {
            return 1; // Parsing error
        }
    }

    fclose(file);
    return 0; // Successful parsing
}
Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = 0;
    graph->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i] = NULL;
    }
    return graph;
}

Node* create_node(const char* name) {
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->name, name);
    node->in_degree = 0;
    node->out_degree = 0;
    node->next = NULL;
    return node;
}

Node* find_or_create_node(Graph* graph, const char* name) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (strcmp(graph->nodes[i]->name, name) == 0) {
            return graph->nodes[i];
        }
    }
    Node* node = create_node(name);
    graph->nodes[graph->num_nodes++] = node;
    return node;
}
void print_graph_stats(Graph* graph) {
    int min_in_degree = INT_MAX, max_in_degree = 0;
    int min_out_degree = INT_MAX, max_out_degree = 0;

    for (int i = 0; i < graph->num_nodes; i++) {
        Node* node = graph->nodes[i];
        if (node->in_degree < min_in_degree) min_in_degree = node->in_degree;
        if (node->in_degree > max_in_degree) max_in_degree = node->in_degree;
        if (node->out_degree < min_out_degree) min_out_degree = node->out_degree;
        if (node->out_degree > max_out_degree) max_out_degree = node->out_degree;
    }

    printf("%s:\n",graph->name);
    printf("- num nodes: %d\n", graph->num_nodes);
    printf("- num edges: %d\n", graph->num_edges);
    printf("- indegree: %d-%d\n", min_in_degree == INT_MAX ? 0 : min_in_degree, max_in_degree);
    printf("- outdegree: %d-%d\n", min_out_degree == INT_MAX ? 0 : min_out_degree, max_out_degree);
    
}
void free_graph(Graph *graph) {
    if (!graph) return;

    for (int i = 0; i < graph->num_nodes; i++) {
        free(graph->nodes[i]);
    }
    
    free(graph);
}