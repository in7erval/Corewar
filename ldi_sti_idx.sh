#!/bin/bash
ASM=./asm
COREWAR=./corewar
cat champions/ldi_idx_mod.s
$ASM champions/ldi_idx_mod.s
$COREWAR -dump 49 champions/ldi_idx_mod.cor |  sed -n '3p'
$COREWAR -dump 50 champions/ldi_idx_mod.cor |  sed -n '3p'
