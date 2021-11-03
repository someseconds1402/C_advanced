#include "fields.h"
#include <stdio.h>
#include <stdlib.h>

void something(IS is, int n, double *arr)
{
    int a;
    double sum;
    while (get_line(is) >= 0)
    {
        a = is->NF;
        sum = atof(is->fields[0]);

        if (is->NF < 2)
        {
            arr[n] = arr[n] + sum * 5 / 6;
            for (int i = 1; i < 7; i++)
            {
                if (i != n)
                {
                    arr[i] = arr[i] - sum / 6;
                }
            }
        }
        else
        {
            arr[n] = arr[n] + sum * (a - 1) / a;
            for (int i = 1; i < a; i++)
            {
                arr[atoi(is->fields[i])] = arr[atoi(is->fields[i])] - sum / a;
            }
        }
    }
}

int main()
{
    IS is1, is2, is3, is4, is5, is6;
    is1 = new_inputstruct("1.txt");
    is2 = new_inputstruct("2.txt");
    is3 = new_inputstruct("3.txt");
    is4 = new_inputstruct("4.txt");
    is5 = new_inputstruct("5.txt");
    is6 = new_inputstruct("6.txt");

    double arr[7] = {0};
    something(is1, 1, arr);
    something(is2, 2, arr);
    something(is3, 3, arr);
    something(is4, 4, arr);
    something(is5, 5, arr);
    something(is6, 6, arr);

    for (int i = 1; i < 7; i++)
    {
        printf("%d : %4.3f\n", i, arr[i]);
    }
}