.name "FarmVille"
.comment "All rights reserved"

				fork %:field
				ld %1, r16
				st r1, 26
				st r1, 26
				st r1, 26
				st r1, 26
				st r1, 26
				live %0
				live %0
				live %0
				live %0
				live %0
				ld -25, r11
				ld -26, r12
				ld -27, r13
				ld -28, r14
				ld -29, r15
				ld %0, r2
				ld %4, r3
				ld %108, r4
				st r1, 6
sowing:			live %0
				sti r11, %:field, r2
				add r2, r3, r2
				sti r12, %:field, r2
				add r2, r3, r2
				sti r13, %:field, r2
				add r2, r3, r2
				sti r14, %:field, r2
				add r2, r3, r2
				sti r15, %:field, r2
				add r2, r3, r2
				add r4, r16, r4
				and r4, %128, r10
				zjmp %:sowing
				and r3, %8, r10
field:			ld %654960, r5
				sti r5, %:field, %399
				and r3, %8, r10
				zjmp %:field
