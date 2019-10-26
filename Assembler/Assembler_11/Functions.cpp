#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Command_nums.h"

const char *initial = {"ELKI"};
const int version = 11;
int MAX_ARGS_NUM = 0;


void Get_File_Name (char *file_name, const char mode[3])
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


char *Get_Buffer (const char *file_name, int *NChars, const char mode[3])
{
	assert (file_name);

	FILE *in = fopen (file_name, mode);
	fseek (in, 0, SEEK_END);
	*NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (*NChars, sizeof (char));
	fread (text, sizeof (char), *NChars, in);
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