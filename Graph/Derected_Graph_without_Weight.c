#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libfdr/fields.h"
#include "../libfdr/jrb.h"
#include "../libfdr/dllist.h"

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

int indegree(Graph g, int v, int *output)
{
    int count = 0;
    JRB node, tree;
    jrb_traverse(node, g.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
            output[count++] = jval_i(node->key);
    }
    return count;
}

int outdegree(Graph g, int v, int *output)
{
    int count = 0;
    JRB node, tree;
    node = jrb_find_int(g.edges, v);
    if (node)
    {
        tree = (JRB)jval_v(node->val);
        jrb_traverse(node, tree)
        {
            output[count++] = jval_i(node->key);
        }
    }
    return count;
}

void display(Graph g, int (*func)(Graph, int, int *))
{
    int key, n, i, output[100];
    JRB node, tree;
    jrb_traverse(node, g.edges)
    {
        key = jval_i(node->key);
        printf("Castle %d: ", key);
        n = func(g, key, output);
        for (i = 0; i < n; i++)
        {
                printf("%10d", output[i]);
        }
        printf("\n");
    }
}

int DAG(Graph graph)
{
    int visited[1000];
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, graph.vertices)
    {
        memset(visited, 0, sizeof(visited));

        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack, new_jval_i(start));

        while (!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);
            if (!visited[u])
            {
                visited[u] = 1;
                n = outdegree(graph, u, output);
                for (i = 0; i < n; i++)
                {
                    v = output[i];
                    if (v == start) // cycle detected
                        return 0;
                    if (!visited[v])
                        dll_append(stack, new_jval_i(v));
                }
            }
        }
    }
    return 1; // no cycle
}

void dropGraph(Graph g)
{
    JRB node, tree;
    jrb_traverse(node, g.edges)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(g.edges);
    jrb_free_tree(g.vertices);
}

void printMatrix(Graph g)
{
    int n, output[9];
    int arr[9][9] = {0}, key, i, j;
    JRB node;

    jrb_traverse(node, g.edges)
    {
        key = jval_i(node->key);
        n = outdegree(g, key, output);
        for (i = 0; i < n; i++)
        {
            arr[key][output[i]] = 1;
        }
        for (i = 0; i < 10; i++)
        {
            output[i] = 0;
        }
    }
    for (i = 1; i < 9; i++)
    {
        for (j = 1; j < 9; j++)
        {
            printf("%6d\t", arr[i][j]);
        }
        printf("\n\n\n");
    }
}

/********************************************************/

int main()
{

    Graph g = createGraph();
    Graph g1 = createGraph();

    IS is;
    is = new_inputstruct("dothi.txt");

    return 0;
}