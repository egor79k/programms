IN
POP AX
CALL fibonacci
END


:fibonacci
	PUSH AX
	PUSH 1
	JNE test_1
		PUSH 1
		RETURN
	:test_1

	PUSH AX
	PUSH 2
	JNE test_2
		PUSH 1
		RETURN
	:test_2

	PUSH AX
	PUSH 1
	SUB
	POP AX
	CALL fibonacci

	PUSH AX
	PUSH 1
	SUB
	POP AX
	CALL fibonacci

	ADD
	POP DX
	PUSH DX

	PUSH AX
	PUSH 2
	ADD
	POP AX
RETURN
END_