# STRING #

def size_t = int64;

NULL: [internal] void* = 0;

@[c]memcpy(destination: void*, source: void*, num: size_t): void*;
@[c]memmove(destination: void*, source: void*, num: size_t): void*;
@[c]strcpy(destination: int8*, source: int8*): int8*;
@[c]strncpy(destination: int8*, source: int8*, num: size_t): int8*;

@[c]strcat(destination: int8*, source: int8*): int8*;
@[c]strncat(destination: int8*, source: int8*, num: size_t): int8*;

@[c]memcmp(ptr1: void*, ptr2: void*, num: size_t): int32;
@[c]strcmp(str1: int8*, str2: int8*): int32;
@[c]strcoll(str1: int8*, str2: int8*): int32;
@[c]strncomp(str1: int8*, str2: int8*, num: size_t): int32;
@[c]strxfrm(destination: int8*, source: int8*, num: size_t): size_t;

@[c]memchr(ptr: void*, value: int32, num: size_t): void*;
@[c]strchr(str: int8*, character: int32): int8*;
@[c]strcspn(str1: int8*, str2: int8*): size_t;
@[c]strpbrk(str1: int8*, str2: int8*): int8*;
@[c]strrchr(str: int8*, character: int32): int8*;
@[c]strspn(str1: int8*, str2: int8*): size_t;
@[c]strstr(str1: int8*, str2: int8*): int8*;
@[c]strtok(str: int8*, delimiters: int8*): int8*;

@[c]memset(ptr: void*, value: int32, num: size_t): void*;
@[c]strerror(errnum: int32): int8*;
@[c]strlen(str: int8*): size_t;
