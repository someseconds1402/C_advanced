#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libfdr/fields.h"
#include "../libfdr/jrb.h"
//#include "../libfdr/dllist.h"

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

void xu_li_chuoi(char *str)
{
    int i, mark, n = strlen(str);
    char s1[10], s2[10];
    for (i = 0; i < n; i++)
    {
        if (str[i] == '-')
            mark = i;
    }
    for (i = 0; i < mark; i++)
    {
        s1[i] = str[i];
    }
    for (i = mark + 1; i < n; i++)
    {
        s2[i - mark - 1] = str[i];
    }
    printf("%s\n%s\n", s1, s2);
}

/********************************************************/

int main()
{

    Graph g = createGraph();

    IS is;
    is = new_inputstruct("file2.txt");

    char s[20]="putin-1977";
    xu_li_chuoi(s);

    return 0;
}