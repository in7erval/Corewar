#!/bin/bash
#
#	Change ASM and compare output
#
ASM=./vm_champs/asm
#ASM=./asm
$ASM champions/live.s
hexdump -vC champions/live.cor | sed -n '138p'
printf "No Argument Coding Byte for \"life\"\n\n"
$ASM champions/ld.s
hexdump -vC champions/ld.cor | sed -n '138p'
printf "          ^  ^  ^\n"
printf "op code___|  |  |\n"
printf "ACB__________|  |\n"
printf "args____________|\n\n"
$ASM champions/st.s
hexdump -vC champions/st.cor | sed -n '138p'
$ASM champions/add.s
hexdump -vC champions/add.cor | sed -n '138p'
$ASM champions/sub.s
hexdump -vC champions/sub.cor | sed -n '138p'
$ASM champions/and.s
hexdump -vC champions/and.cor | sed -n '138p'
$ASM champions/or.s
hexdump -vC champions/or.cor | sed -n '138p'
$ASM champions/xor.s
hexdump -vC champions/xor.cor | sed -n '138p'
$ASM champions/zjmp.s
hexdump -vC champions/zjmp.cor | sed -n '138p'
printf "No Argument Coding Byte for zjmp\n"
$ASM champions/ldi.s
hexdump -vC champions/ldi.cor | sed -n '138p'
$ASM champions/sti.s
hexdump -vC champions/sti.cor | sed -n '138p'
$ASM champions/fork.s
hexdump -vC champions/fork.cor | sed -n '138p'
printf "No Argument Coding Byte for fork\n"
$ASM champions/lld.s
hexdump -vC champions/lld.cor | sed -n '138p'
$ASM champions/lldi.s
hexdump -vC champions/lldi.cor | sed -n '138p'
$ASM champions/lfork.s
hexdump -vC champions/lfork.cor | sed -n '138p'
printf "No Argument Coding Byte for lfork\n"
$ASM champions/aff.s
hexdump -vC champions/aff.cor | sed -n '138p'
