.define cbz label(lbl),val(val)
cmp $val,#0
call.EQ $lbl
.enddef

.define cbnz label(lbl),val(val)
cmp $val,#0
call.NE $label
.enddef

.define clz dest(reg),ref(val),x(val)
push r0
push r1
push r2
push r3
push r4
mov r0,#1
mov r1,#0
mov r3,$ref
mov r4,#0
call _std_clz_loop
mov $dest,r3
pull r4
pull r3
pull r2
pull r1
pull r0
.enddef

.module
.break

_std_clz_loop:
    logi r2,r3,r0,AND
    cmp r2,#0 
    incr.GE r3
    shif r0,#1,LEFT
    cmp r4,#64
    jmp.LT _std_clz_loop