#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libfdr/fields.h"
#include "../libfdr/jrb.h"
#include "../libfdr/dllist.h"

#define INFINITIVE_VALUE 1000000

typedef JRB Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id); //Tra ve value cua id
int getKey(Graph g, char *name);      //Tra ve key cua name
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph g, int v, int *output);
void dropGraph(Graph graph);
int DAG(Graph graph);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
void readFile(Graph g, IS is);

/********************************************************/

Graph createGraph()
{
    Graph g;
    g = make_jrb();
    return g;
}

void addVertex(Graph g1, int id, char *name)
{
    JRB node = jrb_find_int(g1, id);
    if (node == NULL)
        jrb_insert_int(g1, id, new_jval_s(name));
}

char *getVertex(Graph g1, int id)
{
    JRB node = jrb_find_int(g1, id);
    if (node == NULL)
        return NULL;
    return jval_s(node->val);
}

int getKey(Graph g1, char *name)
{
    JRB node, tree;
    jrb_traverse(node, g1)
    {
        if (strcmp(name, jval_s(node->val)) == 0)
            return jval_i(node->key);
    }
    return -1;
}

void addEdge(Graph g2, int v1, int v2, double weight)
{
    JRB node1, node2, tree1, tree2;
    //v1
    node1 = jrb_find_int(g2, v1);
    if (node1 == NULL)
    {
        node1 = jrb_insert_int(g2, v1, new_jval_v((void *)make_jrb()));
    }
    jrb_insert_int((JRB)jval_v(node1->val), v2, new_jval_d(weight));

    //v2
    node2 = jrb_find_int(g2, v2);
    if (node2 == NULL)
    {
        node2 = jrb_insert_int(g2, v2, new_jval_v((void *)make_jrb()));
    }
    jrb_insert_int((JRB)jval_v(node2->val), v1, new_jval_d(weight));
}

double getEdgeValue(Graph g2, int v1, int v2)
{
    JRB node = jrb_find_int(g2, v1);
    if (node == NULL)
        return INFINITIVE_VALUE;
    JRB tree = (JRB)jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node == NULL)
        return INFINITIVE_VALUE;
    else
        return jval_d(node->val);
}

int getAdjacentVertices(Graph g2, int v, int *output)
{
    int count = 0;
    JRB node, tree;
    node = jrb_find_int(g2, v);
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

int DAG(Graph g2)
{
    int visited[1000];
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, g2)
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
                n = getAdjacentVertices(g2, u, output);
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

double shortestPath(Graph g2, int s, int t, int *path, int *length)
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

        n = getAdjacentVertices(g2, u, output);
        for (i = 0; i < n; i++)
        {
            v = output[i];
            w = getEdgeValue(g2, u, v);
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
    jrb_traverse(node, g)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(g);
    jrb_free_tree(g);
}

void add_kho_hang(Graph kho_hang, int id_kho_hang, int id_san_pham, int so_luong)
{
    JRB node;
    node = jrb_find_int(kho_hang, id_kho_hang);
    if (node == NULL)
    {
        node = jrb_insert_int(kho_hang, id_kho_hang, new_jval_v((void *)make_jrb()));
    }
    jrb_insert_int((JRB)jval_v(node->val), id_san_pham, new_jval_i(so_luong));
}

void read_san_pham(Graph san_pham, IS is)
{
    int a, x, i;
    get_line(is);
    a = atoi(is->fields[0]);
    for (i = 0; i < a; i++)
    {
        get_line(is);
        x = atoi(is->fields[1]);
        addVertex(san_pham, x, strdup(is->fields[0]));
    }
}

void read_kho_hang(Graph kho_hang, Graph ten_kho_hang, Graph lien_ket, IS is)
{
    int a, b, x, y, i, j;
    double weight;
    get_line(is);
    a = atoi(is->fields[0]);
    for (i = 0; i < a; i++)
    {
        get_line(is);
        b = atoi(is->fields[1]);
        addVertex(ten_kho_hang, b, strdup(is->fields[0]));

        for (j = 0; j < 3; j++)
        {
            get_line(is);
            x = atoi(is->fields[0]);
            y = atoi(is->fields[1]);
            add_kho_hang(kho_hang, b, x, y);
        }
    }
    get_line(is);
    a = atoi(is->fields[0]);
    for (i = 0; i < a; i++)
    {
        get_line(is);
        x = atoi(is->fields[0]);
        y = atoi(is->fields[1]);
        weight = atof(is->fields[2]);
        addEdge(lien_ket, x, y, weight);
    }
}

void do_something(Graph kho_hang, Graph ten_kho_hang, Graph san_pham, int id_san_pham, int id_kho_hang)
{
    printf("%s\n", getVertex(ten_kho_hang, id_kho_hang));
    JRB node, tree;
    node = jrb_find_int(kho_hang, id_kho_hang);
    tree = (JRB)jval_v(node->val);
    node = jrb_find_int(tree, id_san_pham);
    printf("%s %d\n", getVertex(san_pham, id_san_pham), jval_i(node->val));
}

/********************************************************/

int main()
{
    Graph g = createGraph();
    addEdge(g, 0, 1, 1);
    addEdge(g, 2, 0, 2);
    addEdge(g, 3, 0, 3);
    addEdge(g, 2, 1, 2);
    int out[10], n = getAdjacentVertices(g, 0, out);
    printf("%d\n", n);
    return 0;
}