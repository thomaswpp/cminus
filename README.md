# Cminus
This is a CMinus compiler made with the Yacc (parsing tool) and the Flex (tokenization). This compiler translate the Cminus language, which is a simpilfication of the C language, to the machine code of my architecture, the ENIL Architecture. I had developed this just using C, and the Yacc and Flex tools.

# How Compiler this project

#!/bin/bash

flex cminus.l

bison -d cminus.y

gcc -c *.c -g

gcc -o main *.o -ly -lfl -g

