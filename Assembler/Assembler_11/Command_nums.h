/**
* Version 11
*/

//Registers:
#define REGISTER(name, num) int name = 0; const int name##_NUM = num;
#include "Registers.h"
#undef REGISTER
//Commands:
#define DEF_CMD(name, num, Asm_code, CPU_code, Disasm_code) CMD_##name = num,
#define DEF_JMP_CMD(name, num, operation) CMD_##name = num,
#define DEF_PP_CMD(name, num, CPU_code, Disasm_code) CMD_##name_##num,
enum Commands
{
	#include "Commands.h"
	CMD_MAX
};
#undef DEF_CMD
#undef DEF_JMP_CMD
#undef DEF_PP_CMD
#undef REGISTER