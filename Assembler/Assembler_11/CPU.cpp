#include <math.h>
#include "Stack_Dynamic.h"
#include "Functions.cpp"

char *Check_Version (char *cur);
bool Run (char *cur, char *cur_start, stack_t *stk);


int main ()
{
	char file_name[MAX_NAME_LENGTH] = {};
	stack_t stk = {};
	int  NChars = 0;

	StackConstruct (&stk, 4);
	stk.name = "stk";
	//Getting input file...
	printf ("\nEnter input file name: ");
	Get_File_Name (file_name, "rb");
	char *cur = Get_Buffer (file_name, &NChars, "rb");
	char *cur_start = cur;
	cur = Check_Version (cur);
	//Running...
	MAX_ARGS_NUM = NChars / 5;
	if (Run (cur, cur_start, &stk) == false) printf("\nRunning error\n");
	printf("\nResult = %d\n", StackPop (&stk));

	StackDestruct (&stk);
	return 0;
}


bool Run (char *cur, char *cur_start, stack_t *stk)
{
	char *func_rtn[MAX_ARGS_NUM] = {};
	char param = -1;
	int i = 0;
	int RAM[256] = {};

	while (param != CMD_END)
	{
		param = *cur;
		cur++;
		#define DEF_CMD(name, num, Asm_code, CPU_code, Disasm_code)\
			case num: CPU_code break;
		#define DEF_JMP_CMD(name, num, operation)\
			case num: if (StackPop (stk) operation StackPop (stk)) cur = cur_start + *((int *) cur); else cur += sizeof (int); break;
		#define DEF_PP_CMD(name, num, CPU_code, Disasm_code)\
			case num: CPU_code break;

		switch (param)
		{
			#include "Commands.h"
		}
	}
	return false;
}
