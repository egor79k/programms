#include "Stack_Dynamic.h"

bool Test_Push_1 (stack_t *stk)
{
	for (int i = 0; i < 10; i++)
	{
		StackPush (stk, i);
		if (stk->data[i] != i) return false;
	}
	return true;
}


bool Test_Pop_1 (stack_t *stk)
{
	for (int i = 9; i >= 0; i--)
	{
		if (StackPop (stk) != i) return false;
	}
	return true;
}


bool Test_Pop_2 (stack_t *stk)
{
	StackPop (stk);
	if (stk->err != 2) return false;
	stk->err = 0;
	return true;
}


int main ()
{
	stack_t stk = {};
	StackConstruct (&stk, 2);
	stk.name = "stk";

	printf("\n|||||||Testing results|||||||\n");

	if (Test_Push_1 (&stk) == true) printf("\nPush test 1: OK\n");
	else printf("\nPush test 1: WRONG\n");

	if (Test_Pop_1  (&stk) == true) printf("\nPop test 1:  OK\n");
	else printf("\nPop test 1:  WRONG\n");

	if (Test_Pop_2  (&stk) == true) printf("\nPop test 2:  OK\n");
	else printf("\nPop test 2:  WRONG\n");

	StackDestruct (&stk);

	return 0;
}