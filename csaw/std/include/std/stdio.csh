## STDIO ##

## Types
def FILE;
def fpos_t = int64;
def size_t = int64;
def va_list;

EOF:  [const] int32 = -1;
NULL: [const] void* =  0;

## Operations of files
@[c]remove(filename: int8*): int32;
@[c]rename(oldname: int8*, newname: int8*): int32;
@[c]tmpfile::FILE*;
@[c]tmpnam(str: int8*): int8*;

## File access
@[c]fclose(stream: FILE*): int32;
@[c]fflush(stream: FILE*): int32;
@[c]fopen(filename: int8*, mode: int8*): FILE*;
@[c]freopen(filename: int8*, mode: int8*, stream: FILE*): FILE*;
@[c]setbuf(stream: FILE*, buffer: int8*): void;
@[c]setvbuf(stream: FILE*, buffer: int8*, size: size_t): int32;

## Formatted input/output
@[c]fprintf(stream: FILE*, format: int8*, ?): int32;
@[c]fscanf(stream: FILE*, format: int8*, ?): int32;
@[c]printf(format: int8*, ?): int32;
@[c]scanf(format: int8*, ?): int32;
@[c]snprintf(s: int8*, n: size_t, format: int8*, ?): int32;
@[c]sprintf(s: int8*, format: int8*, ?): int32;
@[c]sscanf(s: int8*, format: int8*, ?): int32;
@[c]vfprintf(stream: FILE*, format: int8*, arg: va_list): int32;
@[c]vfscanf(stream: FILE*, format: int8*, arg: va_list): int32;
@[c]vprintf(format: int8*, arg: va_list): int32;
@[c]vscanf(format: int8*, arg: va_list): int32;
@[c]vsnprintf(s: int8*, n: size_t, format: int8*, arg: va_list): int32;
@[c]vsprintf(s: int8*, format: int8*, arg: va_list): int32;
@[c]vsscanf(s: int8*, format: int8*, arg: va_list): int32;

## Character input/output
@[c]fgetc(stream: FILE*): int32;
@[c]fgets(str: int8*, num: int32, stream: FILE*): int8*;
@[c]fputc(character: int32, stream: FILE*): int32;
@[c]fputs(str: int8*, stream: FILE*): int32;
@[c]getc(stream: FILE*): int32;
@[c]getchar::int32;
@[c]gets(str: int8*): int8*;
@[c]putc(character: int32, stream: FILE*): int32;
@[c]putchar(character: int32): int32;
@[c]puts(str: int8*): int32;
@[c]ungetc(character: int32, stream: FILE*): int32;

## Direct input/output
@[c]fread(ptr: void*, size: size_t, count: size_t, stream: FILE*): size_t;
@[c]fwrite(ptr: void*, size: size_t, count: size_t, stream: FILE*): size_t;

## File positioning
@[c]fgetpos(stream: FILE*, pos: fpos_t*): int32;
@[c]fseek(stream: FILE*, offset: int64, origin: int32): int32;
@[c]fsetpos(stream: FILE*, pos: fpos_t*): int32;
@[c]ftell(stream: FILE*): int64;
@[c]rewind(stream: FILE*): void;

## Error-handling
@[c]clearerr(stream: FILE*): void;
@[c]feof(stream: FILE*): int32;
@[c]ferror(stream: FILE*): int32;
@[c]perror(str: int8*): void;
