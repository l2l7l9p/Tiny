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

http://kqp.world/%E3%80%90%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86%E5%A4%A7%E4%BD%9C%E4%B8%9A%E3%80%91Tiny+%E7%9A%84%E8%AF%AD%E6%B3%95%E6%A0%91/

phase2: semantic analyze & 3-address code

http://kqp.world/%E3%80%90%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86%E5%A4%A7%E4%BD%9C%E4%B8%9A%E3%80%91Tiny+%E7%9A%84%E4%B8%89%E5%9C%B0%E5%9D%80%E7%A0%81/
