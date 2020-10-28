.name "FarmVille"
.comment "All rights reserved"
				
				and r3, %8, r10
harvesting:		zjmp %:start
f_field:		ld %-403, r2
				ld %4, r3
				ld %108, r4
				st r1, 6
f_sowing:		live %0
				sti r11, %:f_field, r2
				add r2, r3, r2
				sti r12, %:f_field, r2
				add r2, r3, r2
				sti r13, %:f_field, r2
				add r2, r3, r2
				sti r14, %:f_field, r2
				add r2, r3, r2
				sti r15, %:f_field, r2
				add r2, r3, r2
				add r4, r16, r4
				and r4, %128, r10
				zjmp %:f_sowing
				and r3, %8, r10
				zjmp %:harvesting
start:			fork %:s_field
				ld %168427120, r16
				st r16, -127
				ld %1, r16
				st r1, 11
				st r1, 11
				live %0
				live %0
				ld -5, r11
				ld -11, r12
				ld -17, r13
				ld -23, r14
				ld -24, r15
				and r3, %8, r10
				zjmp %:continue
f_hirelings:	st r1, 6
				live %0
				fork %:s_hirelings
				st r1, 6
f_hire:			live %0
				fork %:harvesting
				ld %4, r3
				and r3, %8, r10
				zjmp %:f_hire
s_hirelings:	st r1, 6
s_hire:			live %0
				fork %:s_field
				ld %4, r3
				and r3, %8, r10
				zjmp %:s_hire
continue:		fork %:f_field
				ld %0, r2
				ld %4, r3
				ld %108, r4
				st r1, 6
s_sowing:		live %0
				sti r11, %:s_field, r2
				add r2, r3, r2
				sti r12, %:s_field, r2
				add r2, r3, r2
				sti r13, %:s_field, r2
				add r2, r3, r2
				sti r14, %:s_field, r2
				add r2, r3, r2
				sti r15, %:s_field, r2
				add r2, r3, r2
				add r4, r16, r4
				and r4, %128, r10
				zjmp %:s_sowing
				and r3, %8, r10
s_field:		ld %654960, r5
				sti r5, %:s_field, %399
				and r3, %8, r10
				zjmp %:f_hirelings
