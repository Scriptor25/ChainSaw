## STDLIB ##

## Types
def div_t;
def ldiv_t;
def lldiv_t;
def size_t = int64;
def wchar_t = int32;

## Constants
EXIT_FAILURE: int32;
EXIT_SUCCESS: int32;
MB_CUR_MAX:   int32;
NULL:         [const] void* = 0;
RAND_MAX:     int32;

## String conversions
@[c]atof(str: int8*): flt64;
@[c]atoi(str: int8*): int32;
@[c]atol(str: int8*): int64;
@[c]atoll(str: int8*): int128;
@[c]strtod(str: int8*, endptr: int8**): flt64;
@[c]strtof(str: int8*, endptr: int8**): flt32;
@[c]strtol(str: int8*, endptr: int8**, base: int32): int64;
@[c]strtold(str: int8*, endptr: int8**): flt64;
@[c]strtoll(str: int8*, endptr: int8**, base: int32): int128;
@[c]strtoul(str: int8*, endptr: int8**, base: int32): int64;
@[c]strtoull(str: int8*, endptr: int8**, base: int32): int128;

## Pseudo-random sequence generation
@[c]rand::int32;
@[c]srand(seed: int32): void;

## Dynamic memory management
@[c]calloc(num: size_t, size: size_t): void*;
@[c]free(ptr: void*): void;
@[c]malloc(size: size_t): void*;
@[c]realloc(ptr: void*, size: size_t): void*;

## Environment
@[c]abort::void;
@[c]atexit(func: ()(void)*): int32;
@[c]at_quick_exit(func: ()(void)*): int32;
@[c]exit(status: int32): void;
@[c]getenv(name: int8*): int8*;
@[c]quick_exit(status: int32): void;
@[c]system(command: int8*): int32;
@[c]_Exit(status: int32): void;

## Searching and sorting
@[c]bsearch(key: void*, base: void*, num: size_t, compar: (void*, void*)(int32)*): void*;
@[c]qsort(base: void*, num: size_t, size: size_t, compar: (void*, void*)(int32)*): void;

## Integer arithmetics
@[c]abs(n: int32): int32;
@[c]div(numer: int32, denom: int32): div_t;
@[c]labs(n: int64): int64;
@[c]ldiv(numer: int64, denom: int64): ldiv_t;
@[c]llabs(n: int128): int128;
@[c]lldiv(numer: int128, denom: int128): lldiv_t;

## Multibyte characters
@[c]mblen(pmb: int8*, max: size_t): int32;
@[c]mbtowc(pwc: wchar_t*, pmb: int8*, max: size_t): int32;
@[c]wctomb(pmb: int8*, wc: wchar_t): int32;

## Multibyte strings
@[c]mbstowcs(dest: wchar_t*, src: int8*, max: size_t): size_t;
@[c]wcstombs(dest: int8*, src: wchar_t*, max: size_t): size_t;
