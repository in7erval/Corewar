#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/zjmp_idx_mod.s
$ASM champions/zjmp_idx_mod.s
$COREWAR -a -dump 34 champions/zjmp_idx_mod.cor | head -5
