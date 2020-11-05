#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/ld_idx_mod.s
$ASM champions/ld_idx_mod.s
$COREWAR -dump 9 champions/ld_idx_mod.cor |  sed -n '3p'
$COREWAR -dump 10 champions/ld_idx_mod.cor |  sed -n '3p'
