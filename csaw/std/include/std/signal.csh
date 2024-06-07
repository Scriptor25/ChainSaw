# SIGNAL #

def sig_atomic_t = int32;

SIGABRT: [internal] int32 = 1;
SIGFPE:  [internal] int32 = 2;
SIGILL:  [internal] int32 = 3;
SIGINT:  [internal] int32 = 4;
SIGSEGV: [internal] int32 = 5;
SIGTERM: [internal] int32 = 6;

SIG_DFL: [internal] int32 = 0;
SIG_IGN: [internal] int32 = 1;
SIG_ERR: [internal] int32 = 2;

@[c]signal(sig: int32, func: (int32)(void)*): (int32)(void)*;
@[c]raise(sig: int32): int32;
