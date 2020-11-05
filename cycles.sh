#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/cycles.s
$ASM champions/cycles.s
$COREWAR -verbose 11 champions/cycles.cor
