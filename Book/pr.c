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

int co_citation(Graph g, int i, int j, int *cc)
{
    JRB node, tree;
    int key, n, count = 0, total = 0, output[100] = {0};
    jrb_traverse(node, g.edges)
    {
        memset(output, 0, sizeof(output));
        key = jval_i(node->key);
        if (key != i && key != j)
        {
            n = indegree(g, key, output);
            for (int k = 0; k < n; k++)
            {
                if (output[k] == i || output[k] == j)
                    count++;
            }
            if (count == 2)
                cc[total++] = key;
        }
    }
    return total;
}

/********************************************************/

int main(int argc, char *argv[])
{

    if (argc < 2 || argc > 5)
    {
        printf("FBI Warning!!!!!!\n");
        return 1;
    }

    else if (!strcmp(argv[1], "help"))
    {
        printf("Supported command: help, i2n, n2i, cc, cr, odg, rel\n");
        return 0;
    }

    else if (!strcmp(argv[1], "about"))
    {
        printf("C advanced 20142 final exam.\n");
        return 0;
    }

    else if (argc > 2)
    {

        /*******************************************************/

        int a, b, c;

        Graph g = createGraph();
        IS is;
        is = new_inputstruct(argv[2]);

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

        /*******************************************************/

        if (!strcmp(argv[1], "i2n"))
        {
            if (argc != 4)
            {
                printf("FBI Warning!!!\n");
                printf("Exactly Syntax: ./pr i2n file_name id\n");
                return 1;
            }

            int id = atoi(argv[3]);
            if (getVertex(g, id) == NULL)
                printf("Error!\n");
            printf("%s\n", getVertex(g, id));

            return 0;
        }

        else if (!strcmp(argv[1], "n2i"))
        {
            if (argc != 4)
            {
                printf("FBI Warning!!!\n");
                printf("Exactly Syntax: ./pr n2i file_name item_name\n");
                return 1;
            }

            char name[100];
            strcpy(name, argv[3]);
            if (getKey(g, name) == -1)
                printf("Error!\n");
            printf("%d\n", getKey(g, name));

            return 0;
        }
        else if (!strcmp(argv[1], "cc"))
        {
            if (argc != 5)
            {
                printf("FBI Warning!!!\n");
                printf("Exactly Syntax: ./pr cc file_name i j\n");
                return 1;
            }
            int i, j, cc[100];
            i = atoi(argv[3]);
            j = atoi(argv[4]);
            int n = co_citation(g, i, j, cc);
            printf("%d\n", n);
            return 0;
        }

        else if (!strcmp(argv[1], "cr"))
        {

            return 0;
        }

        else if (!strcmp(argv[1], "odr"))
        {

            return 0;
        }

        else if (!strcmp(argv[1], "rel"))
        {

            return 0;
        }
    }

    else
        printf("Lenh khong phu hop!\n");
}