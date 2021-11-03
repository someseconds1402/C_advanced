#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libfdr/fields.h"
#include "../libfdr/jrb.h"
#include "../libfdr/dllist.h"

#define INFINITIVE_VALUE 1000000

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id); //Tra ve value cua id
int getKey(Graph g, char *name);      //Tra ve key cua name
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph g, int v, int *output);
double shortestPath(Graph g, int s, int t, int *path, int *length);
void dropGraph(Graph graph);
int DAG(Graph graph);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
void readFile(Graph g, IS is);

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

void addEdge(Graph g, int v1, int v2, double weight)
{
    JRB node1, node2, tree1, tree2;
    //v1
    node1 = jrb_find_int(g.edges, v1);
    if (node1 == NULL)
    {
        node1 = jrb_insert_int(g.edges, v1, new_jval_v((void *)make_jrb()));
    }
    jrb_insert_int((JRB)jval_v(node1->val), v2, new_jval_d(weight));

    //v2
    node2 = jrb_find_int(g.edges, v2);
    if (node2 == NULL)
    {
        node2 = jrb_insert_int(g.edges, v2, new_jval_v((void *)make_jrb()));
    }
    jrb_insert_int((JRB)jval_v(node2->val), v1, new_jval_d(weight));
}

double getEdgeValue(Graph g, int v1, int v2)
{
    JRB node = jrb_find_int(g.edges, v1);
    if (node == NULL)
        return INFINITIVE_VALUE;
    JRB tree = (JRB)jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node == NULL)
        return INFINITIVE_VALUE;
    else
        return jval_d(node->val);
}

int getAdjacentVertices(Graph g, int v, int *output)
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

double shortestPath(Graph g, int s, int t, int *path, int *length)
{
    double distance[1000], min, w, total;
    int previous[1000], tmp[1000];
    int n, output[100], i, u, v, start;
    Dllist ptr, queue, node;

    for (i = 0; i < 1000; i++)
        distance[i] = INFINITIVE_VALUE;
    distance[s] = 0;
    previous[s] = s;

    queue = new_dllist();
    dll_append(queue, new_jval_i(s));

    while (!dll_empty(queue))
    {
        // get u from the priority queue
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue)
        {
            u = jval_i(ptr->val);
            if (min > distance[u])
            {
                min = distance[u];
                node = ptr;
            }
        }
        dll_delete_node(node);

        if (u == t)
            break; // stop at t

        n = getAdjacentVertices(g, u, output);
        for (i = 0; i < n; i++)
        {
            v = output[i];
            w = getEdgeValue(g, u, v);
            if (distance[v] > distance[u] + w)
            {
                distance[v] = distance[u] + w;
                previous[v] = u;
            }
            dll_append(queue, new_jval_i(v));
        }
    }
    total = distance[t];
    if (total != INFINITIVE_VALUE)
    {
        tmp[0] = t;
        n = 1;
        while (t != s)
        {
            t = previous[t];
            tmp[n++] = t;
        }
        for (i = n - 1; i >= 0; i--)
            path[n - i - 1] = tmp[i];
        *length = n;
    }
    return total;
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

/********************************************************/

int main(int argc, char *argv[])
{

    if (argc < 2 || argc > 5)
    {
        printf("FBI Warning!!!\n");
        return 1;
    }

    else if (!strcmp(argv[1], "h"))
    {
        printf("C-Advanced, HK20152\n");
        return 0;
    }

    else if (argc > 2)
    {
        Graph g = createGraph();
        IS is;
        is = new_inputstruct(argv[2]);
        int a, b, i, x, y, z;

        get_line(is);
        a = atoi(is->fields[0]);

        for (i = 0; i < a; i++)
        {
            get_line(is);
            addVertex(g, atoi(is->fields[0]), strdup(is->fields[1]));
        }

        get_line(is);
        b = atoi(is->fields[0]);

        for (i = 0; i < b; i++)
        {
            get_line(is);
            x = atoi(is->fields[0]);
            y = atoi(is->fields[1]);
            z = atof(is->fields[2]);
            addEdge(g, x, y, z);
        }

        if (!strcmp(argv[1], "-v"))
        {
            JRB node;
            jrb_traverse(node, g.vertices)
            {
                printf("%d %s\n", jval_i(node->key), jval_s(node->val));
            }
            return 0;
        }

        else if (!strcmp(argv[1], "-w"))
        {
            a = atoi(argv[3]);
            b = atoi(argv[4]);
            double d = getEdgeValue(g, a, b);
            if (d == INFINITIVE_VALUE)
                printf("-1\n");
            else
                printf("%5.2f\n", d);

            return 0;
        }

        else if (!strcmp(argv[1], "-p"))
        {

            return 0;
        }
    }

    printf("\n");

    return 0;
}