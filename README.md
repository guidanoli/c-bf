# C Brainfuck Compiler

Having developed an interpreter in Lua, I thought I would give building a compiler a try.

## Running

Simply run `make`, which will generate the bf compiler + some programs, and then:

``` bash
$ ./bf [input] [output_c]
$ gcc [output_c] -o [output]
$ ./[output]
```

If you desire to write the code in the terminal itself (stdin), then do the following:

``` bash
$ ./bf > [output_c]
(write code and then escape input*)
$ gcc [output_c] -o [output]
$ ./[output]
*Ctrl+D on Unix and Ctrl+Z on Windows
```

## Scripts

All the bf scripts are of property of [daniel b cristofani](http://www.hevanet.com/cristofd/brainfuck/).
