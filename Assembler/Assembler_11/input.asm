PUSH 0
POP AX

:not_0
	IN
	POP [AX]

	PUSH [AX]
	PUSH 0

	PUSH AX
	PUSH 1
	ADD
	POP AX
JNE not_0

PUSH 0
POP BX

:not_eqal
	PUSH [BX]
	OUT

	PUSH BX
	PUSH 1
	ADD
	POP BX

	PUSH AX
	PUSH BX
JNE not_eqal
END
END_