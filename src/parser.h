#ifndef PARSER_H
#define PARSER_H
#define MAX_NODES 256
#define MAX_NAME_LENGTH 256

typedef struct Node {
    char name[MAX_NAME_LENGTH];
    int in_degree;
    int out_degree;
    struct Node* next;
} Node;

typedef struct Graph {
    char name[MAX_NAME_LENGTH];
    Node* nodes[MAX_NODES];
    int num_nodes;
    int num_edges;
} Graph;
;

Graph* create_graph();
Node* create_node(const char* name);
void print_graph_stats(Graph *graph);
void free_graph(Graph *graph);
void parse_dot_file(const char* filename, Graph* graph);
Node* find_or_create_node(Graph* graph, const char* name);
#endif