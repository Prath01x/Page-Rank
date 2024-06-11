#ifndef PARSER_H
#define PARSER_H
#define MAX_NODES 200000
#define MAX_NAME_LENGTH 257

typedef struct Node Node;
typedef struct Edge Edge;

 struct Node {
    char name[MAX_NAME_LENGTH];
    int in_degree;
    int out_degree;
    struct Node* next;
    struct Edge* edges;
};
 struct Edge{
   Node* target;
   Edge* next;
};


typedef struct Graph {
    char name[MAX_NAME_LENGTH];
    Node* nodes[MAX_NODES];
    int num_nodes;
    int num_edges;
} Graph;


Graph* create_graph();
Node* create_node(const char* name);
void print_graph_stats(Graph *graph);
void free_graph(Graph *graph);
int parse_dot_file(const char* filename, Graph* graph);
Node* find_or_create_node(Graph* graph, const char* name);
#endif