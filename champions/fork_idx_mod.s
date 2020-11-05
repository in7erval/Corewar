.name "fork_idx_mod"
.comment "fork_idx_mod"
#
# cycles to exec
# aff		2
# fork 		800
# second run aff at 804 cycle
#
aff r1				# 1 + 1 + 1 = 3 bytes
fork %-515			# -515 mod 512 = -3

