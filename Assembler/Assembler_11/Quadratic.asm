IN
IN
IN
POP CX
POP BX
POP AX
CALL Solve
END


:Solve

	PUSH AX
	PUSH 0
	JNE a_not_0

		PUSH BX
		PUSH 0
		JNE b_not_0

			PUSH CX
			PUSH 0
			JNE c_not_0
				PUSH -1
				OUT
				RETURN

			:c_not_0
			PUSH 0
			OUT
			RETURN

		:b_not_0
		PUSH CX
		PUSH 0
		JNE bc_not_0
			PUSH 1
			OUT
			PUSH 0
			OUT
			RETURN

		:bc_not_0
		PUSH 1
		OUT
		PUSH 0
		PUSH CX
		SUB
		PUSH BX
		DIV
		OUT
		RETURN

	:a_not_0
	PUSH BX
	PUSH BX
	MUL
	PUSH AX
	PUSH CX
	PUSH 4
	MUL
	MUL
	SUB
	POP DX

	PUSH DX
	PUSH 0
	JNE d_not_0
		PUSH 1
		OUT
		PUSH BX
		PUSH -1
		MUL
		PUSH AX
		PUSH 2
		MUL
		DIV
		OUT
		RETURN

	:d_not_0
	PUSH DX
	PUSH 0
	JB d_above_0
		PUSH 0
		OUT
		RETURN

	:d_above_0
	PUSH 2
	OUT

	PUSH BX
	PUSH -1
	MUL
	PUSH DX
	ROOT
	ADD
	PUSH AX
	PUSH 2
	MUL
	DIV
	OUT

	PUSH BX
	PUSH -1
	MUL
	PUSH DX
	ROOT
	SUB
	PUSH AX
	PUSH 2
	MUL
	DIV
	OUT

RETURN
END_