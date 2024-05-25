# SIGNAL #

def sig_atomic_t = int32;

SIGABRT: int32 = 1;
SIGFPE: int32 = 2;
SIGILL: int32 = 3;
SIGINT: int32 = 4;
SIGSEGV: int32 = 5;
SIGTERM: int32 = 6;

SIG_DFL: int32 = 0;
SIG_IGN: int32 = 1;
SIG_ERR: int32 = 2;

@[c]signal(sig: int32, func: (int32)(void)*): (int32)(void)*;
@[c]raise(sig: int32): int32;
