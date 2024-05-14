# ChainSaw - A programming language

1. [Why](#why)
2. [Usage](#usage)
3. [The language specifications](#the-language-specifications)
    1. [Functions](#functions)
        1. [Signature](#signature)
        2. [Arguments](#arguments)
        3. [Example](#example)
    2. [Primitive Types](#primitive-types)
    3. [Variables](#variables)

## Why

There is an unholy amount of programming languages out there: so why? Why making my own programming language? Because
why not! It's not about some cool new features or something the world needed, but to just make a programming language.
It's more like a learning project where I develop and upgrade ChainSaw as I get better and have more knowledge on the
subject. E.g. at the beginning there only where the very abstract types 'void', 'num', 'chr' and 'str' and structs where
called 'things'. Now the type names are called 'void', 'int1', 'int8', ..., 'int128', 'flt16', ..., 'flt64', etc. Also,
it now supports pointers so 'str' is now called 'int8*'. 'Things' are now structs, defined by using the 'def'
keyword. To keep it short, a lot changed from where I started off with this project a few years ago, and I learned so
much in the process to the point where I now use LLVM as a codegen backend (that means a lot of headache I had and have
to go through D: ). So to summarize, if you need a language that just works, use C, and if you want to try something
new, use ChainSaw. And, if you think you made something cool with my language, share the project with me, I'd be happy
to hear from someone actually working with this mess... But till then, enjoy this 'masterpiece'!

And btw if you don't understand this documentation or simply don't want to read it, you can take a look at
the [example directory](csaw). It contains 3 examples and the standard library for ChainSaw:

* [fibonacci](csaw/fib)
* [ghost game](csaw/ghost)
* [mandelbrot set](csaw/mandel)
* [standard library](csaw/stdlib)

## Usage

The ChainSaw commandline interface (CSawCLI) is very easy to use:

``CSawCLI [<flag|option>...] <filename>``

Flags are defined as ``--foo``, options additionally have a value: ``--foo=bar``

* ``flag``:
    * ``--help``: show the help text
* ``option``:
    * ``--include``: a comma separated list of additional include directories
* ``filename``: the file to operate on

To compile the [fibonacci](csaw/fib) example:

``CSawCLI <path-to-csaw-examples>/fib/main.csaw``

Or the [mandelbrot set](csaw/mandel):

``CSawCLI --include=<path-to-csaw-examples> <path-to-csaw-examples>/mandel/main.csaw``

## The language specifications

### General syntax rules

1. You need a semicolon after every statement
2. Every function needs at least a declared signature before being used, so you can't do this:
    ```
    @foo = bar();
    
    @bar { ... }
    ```
   You have to do this:
    ```
    @bar;
    
    @foo = bar();
    
    @bar { ... }
    ```
   i.e. you have to define something first to use it.
3. It's illegal to use any statements except defines for structs, functions, constructors or variables outside a
   function.
4. A function has to return a result of its return type on every code path, except for void.
5. There can be two functions with the same name, but they must have different numbers of arguments or a different
   callee.

### Functions

#### Signature

There are a few different ways to define/declare a function. The most simple one is just the function name:

``@foo``

This function takes no arguments, is not callable on any object and returns void.

To shorten this, here a matrix of all possible ways:

| Signature                 | Callee Type | Return Type | Takes Arguments | Called (x: bar, c: callee) |
|---------------------------|-------------|-------------|-----------------|----------------------------|
| ``@foo``                  | none        | void        | no              | foo()                      |
| ``@foo::bar``             | none        | bar         | no              | x = foo()                  |
| ``@foo(...)``             | none        | void        | yes             | foo(...)                   |
| ``@foo(...): bar``        | none        | bar         | yes             | x = foo(...)               |
| ``@foo:callee``           | callee      | void        | no              | c.foo()                    |
| ``@foo:callee:bar``       | callee      | bar         | no              | x = c.foo()                |
| ``@foo:callee(...)``      | callee      | void        | yes             | c.foo(...)                 |
| ``@foo:callee(...): bar`` | callee      | bar         | yes             | x = c.foo(...)             |

Where ``foo`` is the function name, ``bar`` the return type and ``callee`` the type on which the function can be called
on.

To define a constructor for a type, simply swap the ``@`` for a ``$``. It returns an instance of the type specified with
the constructor name. Because a constructor does not have a return type or a callee type, the only possible options for
a constructor are

* ``$foo`` and
* ``$foo(...)``

#### Arguments

are comma separated and defined by the name, a colon and the type: ``foo: bar, foo1: bar1, ...``

For variadic args simply put a ``?`` after the last argument name instead of ``: type``. Example:

``@foo(foo1: bar1, foo2: bar2, foo3?)``

#### Function body

The function body can be either a scoped statement (``{ ... }``) or a simple expression (``= ...;``):

``@foo { ... }``

or

```@foo = ...;```

If you just want to predefine a function, i.e. don't define a body for it yet, you can just put a semicolon on after the
signature:

``@foo::bar;``

#### Example

Here is a simple example of functions:

```
@fib(n: int32): int32 {
    if (n <= 0) ret 0;
    if (n == 1) ret 1;
    ret fib(n - 1) + fib(n - 2);
}

@main::int32 = fib(10);
```

### Primitive Types

ChainSaw has several builtin primitive types:

Void: ``void``

Integer:

* ``int1``
* ``int8``
* ``int16``
* ``int32``
* ``int64``
* ``int128``

Float:

* ``flt16``
* ``flt32``
* ``flt64``

The number next to the type stands for the number of bits this type uses.

You can also use pointers

``foo*``

as types. Or pointers to pointers:

``foo**``

Or even pointers to pointers to pointers:

``foo***``

Just like plain old C...

### Variables

Variables are defined similar to function args: ``foo: bar``, where ``foo`` is the name and ``bar`` the type; But: you
can also initialize them: ``foo: bar = foobar``.

Assigning to a variable is the same as in languages like C: ``foo = bar`` where ``foo`` is the variable (destination)
and ``bar`` the value (source).
