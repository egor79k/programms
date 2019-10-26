#include "Functions.cpp"

#define GET_PARAMETR 													\
		int k = 0;															\
		memset (parametr, '\0', sizeof (char) * MAX_PARAMETR_LENGTH);	\
		while (*text != '\n' && *text != '\0' && *text != ' ')			\
		{																\
			parametr[k++] = *(text++);									\
		}																\
		text++;

#define FIND_LABEL																	\
		GET_PARAMETR																\
		int j = 0;																	\
		while (strcmp (parametr, labels[j].name) != 0 && j < MAX_ARGS_NUM) j++;		\
		if (j < MAX_ARGS_NUM)														\
		{																			\
			*((int *) bin) = labels[j].pointer;										\
		}																			\
		bin += sizeof (int);

const int MAX_PARAMETR_LENGTH = 20;
const int MAX_NAME_LENGTH = 100;
const int MAX_LABEL_NAME = 20;

struct label
{
	int pointer;
	char name[MAX_LABEL_NAME];
};

char *Write_File_Initial (char *bin);
char *Compile (char *text, char *bin, char *bin_start, label *labels);


int main ()
{
	char file_name[MAX_NAME_LENGTH] = {};
	int NChars = 0;
	//Processing input file...
	printf ("\nEnter input file name: ");
	Get_File_Name (file_name, "r");
	char *text = Get_Buffer (file_name, &NChars, "r");
	char *bin = (char *) calloc (NChars, sizeof (char));
	char *bin_start = bin;
	//Compiling...
	MAX_ARGS_NUM = NChars / 3;
	label labels[MAX_ARGS_NUM] = {};
	bin = Write_File_Initial (bin);
	Compile (text, bin, bin_start, labels);
	bin = Compile (text, bin, bin_start, labels);
	if (bin == nullptr)
	{
		printf("\nCompilation error\n");
		return 1;
	}
	else printf("\nSuccesful compilation\n");
	//Processing output file...
	printf ("\nEnter output file name: ");
	Get_File_Name (file_name, "wb");
	FILE *out = fopen (file_name, "wb");
	fwrite (bin_start, sizeof (char), bin - bin_start, out);
	return 0;
}

/**
*	Puts initial string in massive
*
*	@param[in] bin Massive pointer
*
*	return bin - massive pointer
*/
char *Write_File_Initial (char *bin)
{
	int j = 0;
	for (j = 0; j < 4; j++) *bin++ = initial[j];
	for (j = 0; j < sizeof (int); j++) *bin++ = *(((char *) &version) + j);
	return bin;
}

/**
*	Compile input text massive in machine code
*
*	@param[in] text Massive pointer input
*	@param[in] bin Massive pointer output
*	@param[in] bin_start Pointer to start of output massive
*	@param[in] labels Pointer to labels struct
*
*	return bin - Massive with compiled code; nullptr - Compile error;
*/
char *Compile (char *text, char *bin, char *bin_start, label *labels)
	{
		int label_counter = 0;
		char parametr[MAX_PARAMETR_LENGTH] = {};

		while (*text != '\0')
		{
			while (*text == ' ' || *text == '\n' || *text == '\t') text++;
			GET_PARAMETR
			if (parametr[0] == ':')
			{
				strcpy (labels[label_counter].name, &parametr[1]);
				labels[label_counter++].pointer = bin - bin_start;
			}
				else
			#define DEF_CMD(name, num, Asm_code, CPU_code, Disasm_code)\
				if (strcmp (parametr, #name) == 0) {*bin++ = num; Asm_code}\
				else
			#define DEF_JMP_CMD(name, num, operation)\
				if (strcmp (parametr, #name) == 0) {*bin++ = num; FIND_LABEL}\
				else
			#define DEF_PP_CMD(name, num, CPU_code, Disasm_code)

			#include "Commands.h"
			{
				printf("Syntax error\n"); 
				return nullptr;
			}
		}
		return bin;
	}