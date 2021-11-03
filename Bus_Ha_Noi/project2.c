#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libfdr/fields.h"
#include "../libfdr/jrb.h"

#define INFINITIVE_VALUE 1000000

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void dropGraph(Graph graph);
double shortestPath(Graph graph, int s, int t, int *path, int *length);

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

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node, tree;
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
    {
        node = jrb_find_int(graph.edges, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
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

int indegree(Graph g, int v, int *output)
{
    JRB node, tree;
    int count = 0;
    jrb_traverse(node, g.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[count++] = jval_i(node->key);
        }
    }
    return count;
}

int outdegree(Graph g, int v, int *output)
{
    JRB node, tree;
    int count = 0;
    node = jbr_find_int(g.edges, v);
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

// double shortestPath(Graph graph, int s, int t, int *path, int *length);

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

int main()
{
int i, length, path[100], s, t;
   double w;
   Graph g = createGraph();
   addVertex(g, 0, "V0");
   addVertex(g, 1, "V1");
   addVertex(g, 2, "V2");
   addVertex(g, 3, "V3");
   addEdge(g, 0, 1, 1);
   addEdge(g, 1, 2, 3);
   addEdge(g, 2, 0, 3);
   addEdge(g, 1, 3, 1);
   addEdge(g, 3, 2, 1);
    char start[80], stop[80];
    char test1[80] = "DeVaiLon", test2[80] = "EasyToWin";

    // menu:
    // {
    //     printf("1.Tim kiem\n2.Thoat\n");
    //     printf("Your selection : ");
    //     int c, n, i;
    //     scanf("%d", &c);
    //     while (getchar() != '\n')
    //         ;
    //     switch (c)
    //     {
    //     case 1:
    //         i = 1;
    //         while (i)
    //         {
    //             printf("Tram dau tien:");
    //             gets(start);
    //             n = strcmp(start, test1);
    //             if (!n)
    //             {
    //                 i = 0;
    //                 break;
    //             }
    //             printf("Nhap sai, vui long nhap lai!\n");
    //         }
    //         i = 1;
    //         while (i)
    //         {
    //             printf("Tram cuoi cung:");
    //             gets(stop);
    //             n = strcmp(stop, test2);
    //             if (!n)
    //             {
    //                 i = 0;
    //                 break;
    //             }
    //             printf("Nhap sai, vui long nhap lai!\n");
    //         }
    //         goto menu;

    //     case 2:
    //     {
    //         return 0;
    //     }
    //     }
    // }
}
