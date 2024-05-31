# SIGNAL #

def sig_atomic_t = int32;

SIGABRT: int32;
SIGFPE:  int32;
SIGILL:  int32;
SIGINT:  int32;
SIGSEGV: int32;
SIGTERM: int32;

SIG_DFL: int32;
SIG_IGN: int32;
SIG_ERR: int32;

@[c]signal(sig: int32, func: (int32)(void)*): (int32)(void)*;
@[c]raise(sig: int32): int32;
