# SETJMP #

def jmp_buf;

@[c]longjmp(env: jmp_buf, val: int32): void;
@[c]setjmp(env: jmp_buf): int32;
