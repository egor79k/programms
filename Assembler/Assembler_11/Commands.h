/**
* Version 11
*
* Command structure:
* (<name>, <num>, <Asm_code>, <CPU_code>, <Disasm_code>)
*/

#define CPU_REGISTERS 	\
param = *cur++;			\
switch (param)			\
{						\
CPU_REG(AX, 1)			\
CPU_REG(BX, 2)			\
CPU_REG(CX, 3)			\
CPU_REG(DX, 4)			\
}


#define ASM_REGISTERS(num)			\
		switch (parametr[ num ])	\
		{							\
			case 'A':				\
				*bin = AX_NUM;		\
				break;				\
			case 'B':				\
				*bin = BX_NUM;		\
				break;				\
			case 'C':				\
				*bin = CX_NUM;		\
				break;				\
			case 'D':				\
				*bin = DX_NUM;		\
				break;				\
		}\
		bin++;


#define DISASM_REGISTERS				\
		param = *cur++;					\
		switch (param)					\
		{								\
			case AX_NUM:				\
				fprintf (output, "AX");	\
				break;					\
			case BX_NUM:				\
				fprintf (output, "BX");	\
				break;					\
			case CX_NUM:				\
				fprintf (output, "CX");	\
				break;					\
			case DX_NUM:				\
				fprintf (output, "DX");	\
				break;					\
		}


DEF_CMD(PUSH, 17,
	GET_PARAMETR
	if (parametr[1] == 'X')
	{
		*(bin - 1) = 18;
		ASM_REGISTERS(0)
	}
	else
	if (parametr[0] == '[')
	{
		if (parametr[2] == 'X')
		{
			*(bin - 1) = 22;
			ASM_REGISTERS(1)
		}
		else
		{
			*(bin - 1) = 21;
			int number = atoi (&parametr[1]);
			*((int *) bin) = number;
			bin += sizeof (int);
		}
	}
	else
	{
	int number = atoi (parametr);
	*((int *) bin) = number;
	bin += sizeof (int);
	}
	,
	{
		StackPush (stk, *((int *) cur));
		cur += sizeof (int);
	}
	,
	{
		fprintf (output, " %d", *((int *) cur));
		cur += sizeof (int);
	})


DEF_CMD(POP, 34,
	{
		GET_PARAMETR
		if (parametr[0] == '[')
		{
			if (parametr[2] == 'X')
			{
				*(bin - 1) = 38;
				ASM_REGISTERS(1)
			}
			else
			{
				*(bin - 1) = 37;
				int number = atoi (&parametr[1]);
				*((int *) bin) = number;
				bin += sizeof (int);
			}
		}
		else
		{
			ASM_REGISTERS(0)
		}
	}
	,
	{
		#undef CPU_REG
		#define CPU_REG(name, num) case num: name = StackPop (stk); break;
		CPU_REGISTERS
	}
	,
	{
		fprintf (output, " ");
		DISASM_REGISTERS
	})


DEF_CMD(ADD, 2, ,
	{
		StackPush (stk, StackPop (stk) + StackPop (stk));
	}, )


DEF_CMD(SUB, 3, ,
	{
		int subtrahend = StackPop (stk);
		int minued = StackPop (stk);
		StackPush (stk, minued - subtrahend);
	}, )


DEF_CMD(MUL, 4, ,
	{
		StackPush (stk, StackPop (stk) * StackPop (stk));
	}, )


DEF_CMD(DIV, 5, ,
	{
		int divider = StackPop (stk);
		int dividend = StackPop (stk);
		if (divider != 0) StackPush (stk, dividend / divider);
		else
		{
			printf ("Error: %d/%d< You can't divide by zero!", dividend, divider);
			return false;
		}
	}, )


DEF_CMD (IN, 6, ,
	{
		int number = 0;
		printf ("\nEnter number: ");
		scanf ("%d", &number);
		StackPush (stk, number);
	}, )


DEF_CMD(OUT, 7, ,
	{
		printf ("\n%d\n", StackPop (stk));
	}, )


DEF_CMD(DUMP, 8, ,
	{
		Dump (stk, "Just looking", DEFAULT);
	}, )


DEF_CMD(ROOT, 9, ,
	{
		StackPush (stk, sqrt (StackPop (stk)));
	}, )


DEF_CMD(JMP, 10,
	{
		FIND_LABEL
	}
	,
	{
		cur = cur_start + *((int *) cur);
	}
	,
	{
		fprintf (output, " label_%d", *((int *) cur));
		if (compil_num == 1) label->pointer[label->num++] = *((int *) cur);
		cur += sizeof (int);
	})


DEF_JMP_CMD(JA, 11, >)

DEF_JMP_CMD(JAE, 12, >=)

DEF_JMP_CMD(JB, 13, <)

DEF_JMP_CMD(JBE, 14, <=)

DEF_JMP_CMD(JE, 15, ==)

DEF_JMP_CMD(JNE, 16, !=)

//Only for CPU
DEF_PP_CMD(PUSH, 18,
	{
		#undef CPU_REG
		#define CPU_REG(name, num) case num: StackPush (stk, name); break;
		CPU_REGISTERS
	}
	,
	{
		fprintf (output, " ");
		DISASM_REGISTERS
	})


DEF_PP_CMD(PUSH, 21,
	{
		StackPush (stk, RAM[*((int *) cur)]);
		cur += sizeof (int);
	}
	,
	{
		fprintf (output, " [%d]", *((int *) cur));
		cur += sizeof (int);
	})


DEF_PP_CMD(PUSH, 22,
	{
		#undef CPU_REG
		#define CPU_REG(name, num) case num: StackPush (stk, RAM[ name ]); break;
		CPU_REGISTERS
	}
	,
	{
		fprintf (output, " [");
		DISASM_REGISTERS
		fprintf (output, "]");
	})


DEF_CMD(CALL, 35,
	{
		FIND_LABEL
	}
	,
	{
		func_rtn[i++] = cur + sizeof (int);
		cur = cur_start + *((int *) cur);
	}
	,
	{
		fprintf (output, " label_%d", *((int *) cur));
		if (compil_num == 1) label->pointer[label->num++] = *((int *) cur);
		cur += sizeof (int);
	})


DEF_CMD(RETURN, 36, ,
	{
		i--;
		cur = func_rtn[i];
		func_rtn[i] = 0;
	}, )


DEF_PP_CMD(POP, 37,
	{
		RAM[*((int *) cur)] = StackPop (stk);
		cur += sizeof (int);
	}
	,
	{
		fprintf (output, " [%d]", *((int *) cur));
		cur += sizeof (int);
	})


DEF_PP_CMD(POP, 38,
	{
		#undef CPU_REG
		#define CPU_REG(name, num) case num: RAM[ name ] = StackPop (stk); break;
		CPU_REGISTERS
	}
	,
	{
		fprintf (output, " [");
		DISASM_REGISTERS
		fprintf (output, "]");
	})


DEF_CMD(END, 0, ,
	{
		return true;
	}, )


DEF_CMD(END_, 100, , , )
