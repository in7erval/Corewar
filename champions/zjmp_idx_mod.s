.name "zjmp_idx_mod"
.comment "zjmp_idx_mod"
#
# cycles to exec
# aff		2
# add		10
# zjump		20
# second run aff at 34 cycle
#
add r2, r3, r4			# set carry
aff r1				# 1 + 1 + 1 = 3 bytes
zjmp %-515			# -515 mod 512 = -3

