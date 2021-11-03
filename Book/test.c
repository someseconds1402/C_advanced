#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libfdr/fields.h"
#include "../libfdr/jrb.h"
//#include "../libfdr/dllist.h"

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
int getKey(Graph g, char *name);
void addEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void display(Graph g, int (*func)(Graph, int, int *));
void dropGraph(Graph graph);
int DAG(Graph graph);

/********************************************************/

Graph createGraph()
{
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}

void addVertex(Graph g, int id, char *name)
{
    JRB node = jrb_find_int(g.vertices, id);
    if (node == NULL)
        jrb_insert_int(g.vertices, id, new_jval_s(name));
}

char *getVertex(Graph g, int id)
{
    JRB node = jrb_find_int(g.vertices, id);
    if (node == NULL)
        return NULL;
    return jval_s(node->val);
}

int getKey(Graph g, char *name)
{
    JRB node, tree;
    jrb_traverse(node, g.vertices)
    {
        if (strcmp(name, jval_s(node->val)) == 0)
            return jval_i(node->key);
    }
    return -1;
}

void addEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
    {
        node = jrb_insert_int(graph.edges, v1, new_jval_v((void *)make_jrb()));
    }
    jrb_insert_int((JRB)jval_v(node->val), v2, new_jval_i(1));
}

/*****************************************************************************/

int main(int argc, char *argv[])
{
    Graph g = createGraph();
    IS is;
    is = new_inputstruct(argv[1]);
    int a, b, count = 0;

    addVertex(g, 12, "asdasd");

    get_line(is);
    a = atoi(is->fields[0]);
    b = atoi(is->fields[1]);

    for (int i = 0; i < a; i++)
        {
            get_line(is);
            addVertex(g, atoi(is->fields[1]), strdup(is->fields[0]));
        }
        for (int i = 0; i < b; i++)
        {
            get_line(is);
            addEdge(g, atoi(is->fields[0]), atoi(is->fields[1]));
        }
        int id=30;
        printf("%s\n", getVertex(g, id));

    JRB node, tree;
    jrb_traverse(node, g.vertices)
    {
        printf("%d  %s\n", jval_i(node->key), jval_s(node->val));
    }

    return 0;
}