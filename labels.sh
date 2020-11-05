#!/bin/bash
#ASM=./vm_champs/asm
ASM=./asm
cat champions/label.s
$ASM champions/label.s
hexdump -vC champions/label.cor | sed -n '138p'
printf "                ^           ^         ^\n"
printf "direct label____|           |         |\n"
printf "indirect label______________|         |\n"
printf "label_________________________________|\n"
