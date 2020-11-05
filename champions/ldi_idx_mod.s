.name "ldi_idx_mod"
.comment "ldi_idx_mod"
#
# cycles to exec
# ldi		25
# sti		25
# sti run at 50 cycle
#
ldi %515, %516, r4	# 1 + 1 + 2 + 2 + 1 = 7
sti r4, %515, %516	# 1 + 1 + 1 + 2 + 2 = 7

