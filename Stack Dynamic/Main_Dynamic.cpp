#include "Stack_Dynamic.h"

int main ()
{
	stack_t stk1 = {};

	StackConstruct (&stk1, 4);
	stk1.name = "stk1";

	Dump (&stk1, "Just looking", DEFAULT);

	StackPush (&stk1, 100);
	Dump (&stk1, "Push", DEFAULT);
	StackPush (&stk1, 200);
	Dump (&stk1, "Push", DEFAULT);
	StackPush (&stk1, 300);
	Dump (&stk1, "Push", DEFAULT);
	StackPush (&stk1, 400);
	Dump (&stk1, "Push", DEFAULT);
	StackPush (&stk1, 500);
	Dump (&stk1, "Push", DEFAULT);

	StackPop (&stk1);
	Dump (&stk1, "Pop", DEFAULT);
	StackPop (&stk1);
	Dump (&stk1, "Pop", DEFAULT);
	StackPop (&stk1);
	Dump (&stk1, "Pop", DEFAULT);
	StackPop (&stk1);
	Dump (&stk1, "Pop", DEFAULT);
	StackPop (&stk1);
	Dump (&stk1, "Pop", DEFAULT);

	StackDestruct (&stk1);

	return 0;
}