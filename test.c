#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(abs(1-j)>i)printf(" " );
			else printf("*");
		}
		printf("\n");
	}
	return 0;
}