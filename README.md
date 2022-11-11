# Tiny+

A simple compiler for language Tiny+

### Language Definition

see `Tiny+EBNF.md`

### Build

Firstly install Flex and Bison, then run `make`.

### Usage

```
Usage: ./tiny <file> [options]
Options:
  -d            Print syntax tree
  -h            Print help
  -o <file>     Place the output into <file>
```

### Tutorials (in Chinese)

phase1: syntax analyze & draw syntax tree, using flex and bison

http://kqp.world/Tiny+syntaxtree/

phase2: semantic analyze & 3-address code

http://kqp.world/Tiny+3addrcode/
