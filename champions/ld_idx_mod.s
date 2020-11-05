.name "ld_idx_mod"
.comment "ld_idx_mod"

#
# cycles to exec
# ld		5
# st 		5
# st run at 10 cycle
#

ld 517, r1		# 1 + 1 + 2 + 1 = 5 bytes
st r1, 517		# 517 mod 512 = 5 bytes

