# SIGNAL #

def sig_atomic_t = int32;

SIGABRT: [const] int32 = 1;
SIGFPE:  [const] int32 = 2;
SIGILL:  [const] int32 = 3;
SIGINT:  [const] int32 = 4;
SIGSEGV: [const] int32 = 5;
SIGTERM: [const] int32 = 6;

SIG_DFL: [const] int32 = 0;
SIG_IGN: [const] int32 = 1;
SIG_ERR: [const] int32 = 2;

@[c]signal(sig: int32, func: (int32)(void)*): (int32)(void)*;
@[c]raise(sig: int32): int32;
