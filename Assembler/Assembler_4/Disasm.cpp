#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"


const int MAX_NAME_LENGTH = 100;
const char *initial = {"ELKI"};
const int version = 4;

void Get_File_Name (char *file_name, const char *mode);
char *Get_Buffer (const char *file_name);
char *Check_Version (char *cur);
bool Compile (char *cur, FILE *output);


int main ()
{
	char file_name[MAX_NAME_LENGTH] = {};
	//Getting input file...
	printf ("\nEnter input file name: ");
	Get_File_Name (file_name, "rb");
	char *cur = Get_Buffer (file_name);
	//FILE *input = fopen (file_name, "rb");
	cur = Check_Version (cur);
	//Getting output file...
	printf ("\nEnter output file name: ");
	Get_File_Name (file_name, "w");
	FILE *output = fopen (file_name, "w");
	//Compiling...
	if (Compile (cur, output) == true) printf("\nSuccesful compilation\n");
	else printf("\nCompilation error\n");

}


void Get_File_Name (char *file_name, const char *mode)
{
	bool check_result = false;
	while (check_result == false)
	{
		scanf ("%s", file_name);
		FILE *file = fopen (file_name, mode);
		if (file == 0) check_result = false;
		else check_result = true;
		if (check_result == false) printf ("\nWrong name of file. Please enter correct: ");
	}
	return;
}


char *Get_Buffer (const char *file_name)
{
	assert (file_name);

	int NChars = 0;
	FILE *in = fopen (file_name, "rb");
	fseek (in, 0, SEEK_END);
	NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (NChars, sizeof (char));
	fread (text, sizeof (char), NChars, in);
	fclose (in);

	return text;
}


char *Check_Version (char *cur)
{
	char file_initial[5] = {};

	for (int i = 0; i < 4; i++) file_initial[i] = *cur++;
	int file_version = *((int *) cur);
	cur += sizeof (int);
	
	if (strcmp(file_initial, initial) != 0)
	{
		printf("\nWrong initial of file!\n|File initial: %s\n|Disasm initial: %s\n", file_initial, initial);
		exit (1);
	}
	if (file_version != version)
	{
		printf("\nWrong version of file!\n|File version: %d\n|Disasm version: %d\n", file_version, version);
		exit (1);
	}
	return cur;
}


bool Compile (char *cur, FILE *output)
{
	char param = -1;
	while (param != end_num)
	{
		param = *cur;
		cur++;
		switch (param)
		{
			case push_num:
				fprintf (output, "%s %d\n", push, *((int *) cur));
				cur += sizeof (int);
				break;
			case add_num:
				fprintf (output, "%s\n", add);
				break;
			case sub_num:
				fprintf (output, "%s\n", sub);
				break;
			case mul_num:
				fprintf (output, "%s\n", mul);
				break;
			case din_num:
				fprintf (output, "%s\n", din);
				break;
			case end_num:
				fprintf (output, "%s\n", end);
				break;
		}
	}
	return true;
}