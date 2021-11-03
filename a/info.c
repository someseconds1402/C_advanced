#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/jrb.h"
#include "libfdr/jval.h"
#include "libfdr/fields.h"

typedef struct
{
	char *name;
} Name;

typedef JRB Infomation;

/**************************************/

Infomation create_infomation()
{
	return make_jrb();
}

void Sign_in(Infomation infomation, char*email, char*password){

}

void Read_file(IS is, Infomation infomation)
{
	int n1;
	char name[30], email[30], pass[30];
	while (get_line(is) >= 0)
	{
		strcpy(pass, is->fields[is->NF - 1]);
		strcpy(email, is->fields[is->NF - 3]);
		strcpy(name, is->fields[0]);
		n1 = strlen(is->fields[0]);
		for (int i = 1; i < is->NF - 4; i++)
		{
			name[n1] = ' ';
			strcpy(name + n1 + 1, is->fields[i]);
			n1 += strlen(name + n1);
		}

		//printf("%s\n->%s\n-->%s\n", pass, email, name);
	}
}

int main()
{
	Infomation infomation = make_jrb();
	IS is;
	is = new_inputstruct("info.txt");

	Read_file(is, infomation);

	return 0;
}