# ChainSaw - A programming language

# Warning: this documentation is still in the making!

## TODO:

* Operators and operator overloading
* Structs
* Compile directives
* The `me` pointer
* ...

## Why

There is an unholy amount of programming languages out there, what raises the question: why? Why making my own
programming language? Because why not! It's not about some cool new features or something the world needed, but to just
make a programming language. It's more like a learning project where I develop and upgrade ChainSaw as I get better and
have more knowledge on the subject. E.g. at the beginning there only where the very abstract types 'void', 'num', 'chr'
and 'str' and structs where called 'things'. Now the type names are called 'void', 'int1', 'int8', ..., 'int128',
'flt16', ..., 'flt64', etc. Also, it now supports pointers so 'str' is now called 'int8*'. 'Things' are now structs,
defined by using the 'def' keyword. To keep it short, a lot changed from where I started off with this project a few
years ago, and I learned so much in the process to the point where I now use LLVM as a codegen backend (that means a lot
of headache I had and have to go through D: ). So to summarize, if you need a language that just works, use C, and if
you want to try something new, use ChainSaw. And, if you think you made something cool with my language, share the
project with me, I'd be happy to hear from someone actually working with this mess... But till then, enjoy this
'masterpiece' of a documentation!

And btw if you don't understand this documentation or simply don't want to read it, you can take a look at
the [examples](csaw).

## Usage

The commandline interface is very easy to use and can be integrated into build systems like gnu make:

`csaw [options] file...`

Use `csaw` without any arguments or `csaw -h` to display the help text.

So for example, if you have a project with a source and an include directory, you could do something like this:

```makefile
CSAW = <path to csaw>
GCC  = gcc

STDDIR = <path to the csaw std includes>
INCDIR = include
SRCDIR = src
OBJDIR = build

CSAW_FLAGS = -i $(INCDIR) -i $(STDDIR)
GCC_FLAGS  =

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

TARGET = $(OBJDIR)/<name of your target executable>
SRCS   = $(call rwildcard, $(SRCDIR), *.csaw)
OBJS   = $(patsubst $(SRCDIR)/%.csaw, $(OBJDIR)/%.o, $(SRCS))

all: $(TARGET)

clean:
	@if exist $(OBJDIR) rd /s /q $(OBJDIR)

run: $(TARGET)
	@$(TARGET)

$(TARGET): $(OBJS)
	$(GCC) $(GCC_FLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.csaw
	@if not exist $(@D) mkdir $(subst /,\,$(@D))
	$(CSAW) $(CSAW_FLAGS) -o $@ $<

.PHONY: all clean run
```

This is the template I personally use on windows. I guess there are better ways to manage a project, but for small ones
it's enough.

## The language specifications

### Functions

There are a few different ways to declare a function. The simplest one is just the function name and a return type:

`@foo::void`

This function takes no arguments, has no parent type and returns void.

To shorten this, here a table with all possible combinations:

| Signature               | Parent Type | Return Type | Takes Arguments | Call (x: bar, p: parent) |
|-------------------------|-------------|-------------|-----------------|--------------------------|
| `@foo::bar`             | none        | bar         | no              | x = foo()                |
| `@foo(...): bar`        | none        | bar         | yes             | x = foo(...)             |
| `@foo:parent:bar`       | parent      | bar         | no              | x = c.foo()              |
| `@foo:parent(...): bar` | parent      | bar         | yes             | x = p.foo(...)           |

Where `foo` is the function name, `bar` the return type and `parent` the type on which the function can be called
on.

To define a constructor for a type, simply don't specify a return type. It returns an instance of the type specified
with the constructor name. Because a constructor does not have a return or parent type, the only possible options
for a constructor are

* `@foo` and
* `@foo(...)`

Function arguments are comma separated and defined by the name, a colon and the type: `foo: bar, foo1: bar1, ...`. If
you don't use the argument or don't need a name for it yet (e.g. if you only want to pre-declare the function), you can
instead just use the type, e.g. `bar, bar1, ...`.

For variadic args simply put a `?` as the last argument. Example:

`@foo(foo1: bar1, foo2: bar2, ?): bar`

The function body can be either a scoped statement (`{ ... }`) or a simple expression or statement (`= ...;`):

`@foo { ... }`

or

`@foo = ...;`

If you just want to pre-declare a function, i.e. don't define a body for it yet, you can just end the signature with a
semicolon:

`@foo::bar;`

To add modifiers to a function, like the C++ `extern "C"`, put them into the brackets after the `@`:

`@[c]foo`

The `c` modifier leaves the function name untouched so the function can be recognized by linkers if you want to access
standard c functionality.

Here is a simple example for functions:

```
@fib(n: int32): int32 {
    if (n <= 0) ret 0;
    if (n == 1) ret 1;
    ret fib(n - 1) + fib(n - 2);
}

@[c]main::int32 = fib(10);
```

### Primitive Types

ChainSaw has several builtin primitive types:

Void: `void`

Integer:

* `int1`
* `int8`
* `int16`
* `int32`
* `int64`
* `int128`

Float:

* `flt16`
* `flt32`
* `flt64`

The number next to the type stands for the number of bits this type uses. This is platform and system independent.

You can also use pointers

`foo*`

as types. Or pointers to pointers:

`foo**`

Or even pointers to pointers to pointers:

`foo***`

Just like plain old C...

A very new addition to the collection are arrays (and yes, just like the commit says, it was a pain in the ass to
somehow get this to work):

`foo[10]`

This allocates an array of size 10 of type `foo` on the stack. To access the elements, use the `[]` operator:

`bar[index]`

You can assign into an element, as well as getting a reference to it. So to input an array as a pointer, you have to do
something like this:

`&bar[0]`

By taking the first element and using its reference, you get the array's pointer.

### Variables

Variables are defined similar to function args: `foo: bar`, where `foo` is the name and `bar` the type; But: you
can also initialize them: `foo: bar = foobar`.

Assigning to a variable is the same as in languages like C: `foo = bar` where `foo` is the variable (destination)
and `bar` the value (source).
