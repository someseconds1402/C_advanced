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
char *getVertex(Graph graph, int id);
int getKey(Graph g, char *name);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void display(Graph g, int (*func)(Graph, int, int *));
void dropGraph(Graph graph);
int DAG(Graph graph);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
void readFile(Graph g, IS is);
void printMatrix(Graph g);             //in ma tran lien ke
void printAdjacentCastle(Graph g);     //in ra nhung dinh lien ke
int to_Castel_On_foot(Graph g, int v); //tra ve 1 neu tat ca ca dinh di toi v > 50
int maxIndegree(Graph g, int *output); //tra ve so dinh co indegree cao nhat

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
            if (getEdgeValue(g, key, output[i]) == INFINITIVE_VALUE)
                printf("%10d", output[i]);
            else
                printf("%10d(%2.2f)", output[i], getEdgeValue(g, key, output[i]));
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

        n = outdegree(g, u, output);
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

void readFile(Graph g, IS is)
{
    while (get_line(is) >= 1)
    {
        if (is->line > 1)
        {
            addEdge(g, atoi(is->fields[0]),
                    atoi(is->fields[1]), atof(is->fields[2]));
        }
    }
}

void readFile_2(Graph g, IS is)
{
    while (get_line(is) >= 1)
    {
        if (is->line > 1)
        {
            if (atoi(is->fields[2]) >= 50)
            {
                addEdge(g, atoi(is->fields[0]),
                        atoi(is->fields[1]), atof(is->fields[2]));
                printf("%d\t%d\t%f\n", atoi(is->fields[0]),
                       atoi(is->fields[1]), atof(is->fields[2]));
            }
        }
    }
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

void printAdjacentCastle(Graph g)
{
    int i, j, n, arrIndegree[9], arrOutdegree[9];
    for (i = 1; i < 9; i++)
    {
        printf("Castle %d:", i);
        n = indegree(g, i, arrIndegree);
        for (j = 0; j < n; j++)
        {
            printf("%5d", arrIndegree[j]);
        }
        n = outdegree(g, i, arrOutdegree);
        for (j = 0; j < n; j++)
        {
            printf("%5d", arrOutdegree[j]);
        }
        printf("\n");
    }
}

int to_Castel_On_foot(Graph g, int v)
{
    int arr[9], i, n;
    n = indegree(g, v, arr);
    for (i = 0; i < n; i++)
    {
        if (getEdgeValue(g, arr[i], v) < 50)
            return 0;
    }
    return 1;
}

int indegree_2(Graph g, int v)
{
    int count = 0;
    JRB node, tree;
    jrb_traverse(node, g.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
            count++;
    }
    return count;
}

int maxIndegree(Graph g, int *output)
{
    int i, n, max = 0, count = 0;
    for (i = 1; i < 9; i++)
    {
        n = indegree_2(g, i);
        if (n > max)
        {
            max = n;
        }
    }
    for (i = 1; i < 9; i++)
    {
        if (indegree_2(g, i) == max)
        {
            output[count++] = i;
        }
    }
    return count;
}

/********************************************************/

int main()
{

    Graph g = createGraph();
    Graph g1 = createGraph();

    IS is;
    is = new_inputstruct("dothi.txt");
    readFile(g, is);
    readFile_2(g1, is);

    //     printf("INDEGREE:(no path)\n");
    //     display(g1, indegree);
    //     printf("\nOUTDEGREE:\n");
    //     display(g1, outdegree);

menu:
{
    printf("MENU\n");
    printf("1.Print Matrix\n2.Print Adjacent Castles\n3.???\n");
    printf("4.Search Shortest Route\n5.Search Shortest Route On Foot\n6.Quit\n");
    printf("Your selection : ");
    int c;
    scanf("%d", &c);
    while (getchar() != '\n')
        ;
    switch (c)
    {
    case 1:
    {
        printMatrix(g);
        goto menu;
    }
    case 2:
    {
        printAdjacentCastle(g);
        goto menu;
    }
    case 3:
    {
        printf("\na:");
        for (int i = 1; i < 9; i++)
        {
            if (to_Castel_On_foot(g, i))
                printf("%d\t", i);
        }
        goto menu;
    }
    case 4:
    {
        int i, v1, v2;
        printf("\nStart: ");
        scanf("%d", &v1);
        printf("Stop: ");
        scanf("%d", &v2);
        int length, path[100];
        double w = shortestPath(g, v1, v2, path, &length);
        if (w == INFINITIVE_VALUE)
        {
            printf("ROUTE NOT FOUND\n");
        }
        else
        {
            printf("Path from %d to %d (with total distance %3.2f)\n", v1, v2, w);
            printf(" %d", path[0]);
            for (i = 1; i < length; i++)
                printf(" -> %d", path[i]);
        }
        printf("\n");
        goto menu;
    }
    case 5:
    {
        int i, v1, v2;
        printf("\nStart: ");
        scanf("%d", &v1);
        printf("Stop: ");
        scanf("%d", &v2);
        int length, path[100];
        double w = shortestPath(g1, v1, v2, path, &length);
        if (w == INFINITIVE_VALUE)
        {
            printf("ROUTE NOT FOUND\n");
        }
        else
        {
            printf("Path from %d to %d (with total distance %3.2f)\n", v1, v2, w);
            printf(" %d", path[0]);
            for (i = 1; i < length; i++)
                printf(" -> %d", path[i]);
        }
        printf("\n");
        goto menu;
    }
    case 6:
    {
        return 0;
    }
    default:
    {
        printf("Error! Please try again\n");
        goto menu;
    }
    }
}

    dropGraph(g);
    dropGraph(g1);
    return 0;
}