#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "Stack_Dynamic.h"


const char *initial = {"ELKI"};
const int version = 4;

void Get_File_Name (char *file_name, const char *mode);
char *Get_Buffer (const char *file_name);
char *Check_Version (char *cur);
bool Run (char *cur, stack_t *stk);


int main ()
{
	char file_name[MAX_NAME_LENGTH] = {};
	stack_t stk = {};

	StackConstruct (&stk, 4);
	stk.name = "stk";
	//Getting input file...
	printf ("\nEnter input file name: ");
	Get_File_Name (file_name, "rb");
	char *cur = Get_Buffer (file_name);
	cur = Check_Version (cur);
	//Running...
	if (Run (cur, &stk) == false) printf("\nRunning error\n");
	printf("\nResult = %d\n", StackPop (&stk));

	StackDestruct (&stk);
	return 0;
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
		printf("\nWrong initial of file!\n|File initial: %s\n|CPU initial: %s\n", file_initial, initial);
		exit (1);
	}
	if (file_version != version)
	{
		printf("\nWrong version of file!\n|File version: %d\n|CPU version: %d\n", file_version, version);
		exit (1);
	}
	return cur;
}


bool Run (char *cur, stack_t *stk)
{
	char param = -1;

	while (param != end_num)
	{
		param = *cur;
		cur++;
		switch (param)
		{
			case push_num:
				StackPush (stk, *((int *) cur));
				cur += sizeof (int);
				break;

			case add_num:
				StackPush (stk, StackPop (stk) + StackPop (stk));
				break;

			case sub_num:
				{
					int subtrahend = StackPop (stk);
					int minued = StackPop (stk);
					StackPush (stk, minued - subtrahend);
				}
				break;

			case mul_num:
				StackPush (stk, StackPop (stk) * StackPop (stk));
				break;

			case din_num:
				{
					int divider = StackPop (stk);
					int dividend = StackPop (stk);
					if (divider != 0) StackPush (stk, dividend / divider);
					else
					{
						printf ("Error: %d/%d< You can't divide by zero!", dividend, divider);
						return false;
					}
				}
				break;

			case end_num:
				return true;
				break;
		}
	}
	return false;
}