#include "Functions.cpp"

const int MAX_NAME_LENGTH = 100;

struct labels
{
	int *pointer;
	int num;
};

char *Check_Version (char *cur);
bool Compile (char *cur, char *cur_start, labels *label, FILE *output);


int main ()
{
	char file_name[MAX_NAME_LENGTH] = {};
	int NChars = 0;
	//Getting input file...
	printf ("\nEnter input file name: ");
	Get_File_Name (file_name, "rb");
	char *cur = Get_Buffer (file_name, &NChars, "rb");
	char *cur_start = cur;
	cur = Check_Version (cur);
	//Getting output file...
	printf ("\nEnter output file name: ");
	Get_File_Name (file_name, "w");
	FILE *output = fopen (file_name, "w");
	//Compiling...
	MAX_ARGS_NUM = NChars / 5;
	labels label = {};
	int pointers[MAX_ARGS_NUM] = {};
	label.pointer = pointers;
	Compile (cur, cur_start, &label, output);
	fseek (output, 0 , SEEK_SET);
	if (Compile (cur, cur_start, &label, output) == true) printf("\nSuccesful compilation\n");
	else printf("\nCompilation error\n");

}


bool Compile (char *cur, char *cur_start, labels *label, FILE *output)
{
	int compil_num = 1;
	if (label->num != 0) compil_num = 2;
	char param = -1;
	while (param != CMD_END_)
	{
		param = *cur;
		cur++;
		for (int j = 0; j < label->num; j++)
			{
			if (cur - cur_start - 1 == label->pointer[j] && compil_num == 2)
			{
				fprintf (output, ":label_%d\n", cur - cur_start - 1);
			}
			}
		#define DEF_CMD(name, num, Asm_code, CPU_code, Disasm_code)\
			case num: fprintf (output, "%s", #name); Disasm_code fprintf(output, "\n"); break;

		#define DEF_JMP_CMD(name, number, operation)\
			case number: fprintf (output, "%s", #name);\
			fprintf (output, " label_%d", *((int *) cur));\
			if (compil_num == 1) label->pointer[label->num++] = *((int *) cur);\
			cur += sizeof (int);\
			fprintf(output, "\n");\
			break;

		#define DEF_PP_CMD(name, num, CPU_code, Disasm_code)\
			case num: fprintf (output, "%s", #name); Disasm_code fprintf(output, "\n"); break;

		switch (param)
		{
			#include "Commands.h"
		}
	}
	return true;
}