# STDARG #

def va_list;

@[c]va_start(ap: va_list, param: void*): void;
@[c]va_arg<type>(ap: va_list): type;
@[c]va_end(ap: va_list): void;
@[c]va_copy(dest: va_list, src: va_list): void;
