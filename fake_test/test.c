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

int hasEdge(Graph g, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(g.edges, v1);
    if (node == NULL)
        return 0;
    tree = jrb_find_int((JRB)jval_v(node->val), v2);
    if (tree == NULL)
        return 0;
    return 1;
}

void addEdge(Graph g, int v1, int v2, double weight)
{
    JRB node1, node2, tree1, tree2;
    if (hasEdge(g, v1, v2) == 0)
    {
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
                n = getAdjacentVertices(graph, u, output);
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

void enQueue(Dllist q, int n)
{
    dll_append(q, new_jval_i(n));
}

int deQueue(Dllist q)
{
    Dllist node = dll_first(q);
    int v = jval_i(node->val);
    dll_delete_node(node);
    return v;
}

int BFS(Graph g, int start, int stop)
{
    int visited[100] = {0};
    Dllist queue = new_dllist();
    enQueue(queue, start);
    while (!dll_empty(queue))
    {
        int v = deQueue(queue);
        if (visited[v] == 0)
        {
            visited[v] = 1;
            int arr_vertex[100] = {0};
            int n = getAdjacentVertices(g, v, arr_vertex);
            for (int i = 0; i < n; i++)
            {
                if (visited[arr_vertex[i]] == 0)
                    enQueue(queue, arr_vertex[i]);
            }
            if (v == stop)
                return 1;
        }
    }
    return 0;
}

void DFS(Graph g, int start, int stop, int visited[])
{
    if (stop == -1)
    {

        visited[start] = 1;
        int arr_vertex[100] = {0};
        int n = getAdjacentVertices(g, start, arr_vertex);
        for (int i = 0; i < n; i++)
        {
            if (visited[arr_vertex[i]] == 0)
            {
                DFS(g, arr_vertex[i], stop, visited);
            }
        }
    }
    else
    {
        visited[start] = 1;
        if (start == stop)
        {
            return;
        }
        int arr_vertex[100] = {0};
        int n = getAdjacentVertices(g, start, arr_vertex);
        for (int i = 0; i < n; i++)
        {
            if (visited[arr_vertex[i]] == 0)
            {
                DFS(g, arr_vertex[i], stop, visited);
            }
        }
    }
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

void readFile(Graph g, IS is)
{
    int a, b, i;
    double weight;
    while (get_line(is) >= 0)
    {
        a = is->line;
        for (i = 0; i < is->NF; i++)
        {
            weight = atof(is->fields[i]);
            if (weight != 0)
            {
                addEdge(g, a, i + 1, weight);
            }
        }
    }
}

void printMatrix(IS is)
{
    int i;
    while (get_line(is) >= 0)
    {
        for (i = 0; i < is->NF; i++)
        {
            printf("%s\t", is->fields[i]);
        }
        printf("\n\n");
    }
}

void duyetDoThi(Graph g)
{
    JRB node, tree, ptr;
    int arr[100] = {0}, output[100];
    int i, count_vertieces = 0, count_edge = 0, n;
    jrb_traverse(node, g.edges)
    {
        n = getAdjacentVertices(g, jval_i(node->key), output);
        count_edge += n;
        count_vertieces++;
    }
    printf("Tong so canh: %d\n", count_edge / 2);
    printf("Tong so dinh: %d\n", count_vertieces);
}

/********************************************************/

int main()
{

    Graph g = createGraph();
    IS is = new_inputstruct("file.txt");
    IS is_2 = new_inputstruct("file.txt");
    JRB node, tree;
    int out[100];
    readFile(g, is);
menu:
{
    printf("\n\x1B[32mMENU\n");
    printf("\x1B[33m1.Doc va luu\n2.Duyet do thi\n3.Liet ke cac phong lien ke\n4.Ton tai ket noi\n5.Kiem tra tinh lien thong\n7.Thoat\n\x1B[0m");
    printf("Lua chon : ");
    int c;
    scanf("%d", &c);
    while (getchar() != '\n')
        ;
    switch (c)
    {
    case 1:
    {
        printMatrix(is_2);
        goto menu;
    }
    case 2:
    {
        duyetDoThi(g);
        goto menu;
    }
    case 3:
    {
        int n, id, output[10] = {0};
        printf("Nhap so phong: ");
        scanf("%d", &id);
        n = getAdjacentVertices(g, id, output);
        for (int i = 0; i < n; i++)
        {
            printf("%d\t", output[i]);
        }
        goto menu;
    }
    case 4:
    {
        int n, a, b;
        printf("Nhap so phong 1:");
        scanf("%d", &a);
        printf("Nhap so phong 2:");
        scanf("%d", &b);
        n = BFS(g, a, b);
        if (n == 0)
            printf("Khong co day dien giua phong %d va %d\n", a, b);
        else
            printf("Co day dien giua phong %d va %d\n", a, b);
        goto menu;
    }
    case 5:
    {
        int arr[10], n, i;
        DFS(g, 1, -1, arr);
        for (i = 0; i < 5; i++)
        {
            printf("%d\n", arr[i]);
        }
        goto menu;
    }
    case 6:
    {
        goto menu;
    }
    case 7:
        return 0;
    default:
    {
        printf("Error! Please try again\n");
        goto menu;
    }
    }
}

    dropGraph(g);
    return 0;
}