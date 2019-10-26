IN
POP AX

PUSH AX
PUSH 13
JA less_13
	PUSH 0 
	OUT
	PUSH 1
	END

:less_13

PUSH AX
PUSH 0
JBE above_0
	PUSH -1
	OUT
	PUSH 1
	END

:above_0

PUSH AX
PUSH 0
JNE not_0
	PUSH 1
	OUT
	PUSH 0
	END

:not_0
PUSH 1
POP BX
PUSH 1
CALL factorial
OUT
PUSH 0
END


:factorial
	PUSH BX
	MUL

	PUSH BX
	PUSH AX
	JE stop
		PUSH BX
		PUSH 1
		ADD
		POP BX
		CALL factorial

	:stop
RETURN
END_
