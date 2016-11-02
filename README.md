# The Falk Programming Language


## Goals

#### Fast
The Virtual Machine in Falk is a [Threaded Interpreter](https://en.wikipedia.org/wiki/Threaded_code).
This is probably the fastest Virtual Machine interpreter. The only downside I can think of is that is't harder to implement, compared to other solutions like [Decode and Dispatch](http://stackoverflow.com/questions/3848343/decode-and-dispatch-interpretation-vs-threaded-interpretation).

#### Simple
Target syntax style is aimed to be like C.
Falk is going to be a dynamically typed language. When defining a variable, type is not required.

Example:
``` ruby
# define function
test = func(from, to) {
    if (from > to) {
        return;
    }
    count = from;
    while (count < to) {
        print(count);
        count += 1;
    }
}

# call function
test(4, 18);

```

#### Cross-platform
Falk is compiled in C99 and should compile on most platforms.

#### Extensible
If you want to do something, nothing should stop you from doing so. My goal is to make it possible to compile a library and then, link it to the Falk interpreter.



## Accomplishments

* VM **√**
   * Variables (modify, create)
   * Strings, numbers (``` double ```)
   * C function calls
   * Deserializer (execute a compiled file)
   * Push, pop, goto, jump and if instructions
   * Assignment & Comparison operators
   * Simple optimizations (lookup variable and store pointer instead of key)
* Lexer **√**
   * Keywords
   * Identifiers
   * Numbers
   * Operators (length of 1 (+, -, /))
   * Simple block mismatch check
   * Line counter
* Parser **x**
   * Infix to postfix **√**
   * Grammar checking (syntax validation) **√**
   * AST **x**
   * AST -> byte code **x**
