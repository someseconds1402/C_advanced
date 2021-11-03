#include <stdio.h>

void change_color(int i)
{
    switch (i)
    {
    case 0:
        printf("\x1B[0m");
        break;

    case 1:
        printf("\x1B[31m");
        break;

    case 2:
        printf("\x1B[32m");
        break;
    case 3:
        printf("\x1B[33m");
        break;
    case 4:
        printf("\x1B[34m");
        break;
    case 5:
        printf("\x1B[35m");
        break;
    case 6:
        printf("\x1B[36m");
        break;
    case 7:
        printf("\x1B[37m");
        break;
    default:
        break;
    }
}

int main()
{
    change_color(1);
    printf("Day la mau do\n");
    change_color(2);
    printf("Day la mau xanh\n");
    change_color(0);
    printf("ajshdgajshd\n");
    change_color(3);
    printf("ajshdgajshd\n");
    change_color(4);
    printf("ajshdgajshd\n");
    change_color(5);
    printf("ajshdgajshd\n");
    change_color(6);
    printf("ajshdgajshd\n");
    change_color(7);
    printf("ajshdgajshd\n");
}