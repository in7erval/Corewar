#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/cycles.s
$ASM champions/cycles.s
$COREWAR -v champions/cycles.cor
