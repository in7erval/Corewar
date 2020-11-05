#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/A.s
cat champions/B.s
$ASM champions/A.s
$ASM champions/B.s
$COREWAR -a champions/A.cor champions/B.cor
