#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/fork_idx_mod.s
$ASM champions/fork_idx_mod.s
$COREWAR -a -dump 804 champions/fork_idx_mod.cor | head -5
