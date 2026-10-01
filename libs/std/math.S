@ Math instructions of Gama-X Open-Source Standard Library.
@ Abolfazl Pouretemadi [10-1-2026]
@ R0-R4 Registers are required as ABI.

.replace PI #3.141592653
.replace TAU #6.283185307
.replace E #2.718281828

.define logb dest(reg),x(val),base(value)
push r0
push r1
ln r0,$x
ln r1,$base
div $dest,r0,r1
pull r1
pull r0
.enddef


.define log10 dest(reg),x(val)
push r0
push r1
ln r0,$x
ln r1,#10
div $dest,r0,r1
pull r1
pull r0
.enddef

.define log2 dest(reg),x(val)
push r0
push r1
ln r0,$x
ln r1,#2
div $dest,r0,r1
pull r1
pull r0
.enddef

.define exp dest(reg),x(reg)
Fpow $dest,E,$x
.enddef

.define hypot dest(reg),x(val),y(val)
push r0
push r1
pow r0,$x,#2
pow r1,$y,#2
add r0,r0,r1
sqrt $dest,r0
pull r1
pull r0
.enddef

.define factorial dest(reg), n(val)
    push r0
    push r1
    mov r0,$n
    mov r1,#1
    call _std_factorial_loop
    mov $dest,r1
    pull r1
    pull r0
.enddef

.define deg2rad dest(reg),x(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fmul.EQ $dest,$dest,PI
Fdiv.EQ $dest,$dest,#180
.enddef

.define rad2deg dest(reg),x(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fmul $dest,$dest,#180
Fdiv $dest,$dest,PI
.enddef

@ Comparison instructions.

.define max dest(reg),x(val),y(val)
cmp $x,$y
mov.GE $dest,$x
mov.LS $dest,$y
.enddef
.define fmax dest(reg),x(val),y(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fcmp.EQ $x,$y
Fmov.FGE $dest,$x
Fmov.FLS $dest,$y
.enddef

.define min dest(reg),x(val),y(val)
cmp $x,$y
mov.GE $dest,$y
mov.LS $dest,$x
.enddef
.define fmin dest(reg),x(val),y(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fcmp.EQ $x,$y
Fmov.FGE $dest,$y
Fmov.FLS $dest,$x
.enddef

.define dim dest(reg),x(val),y(val)
cmp $_float_mode_,#1
sub.NE $dest,$x,$y
abs.NE $dest,$dest
Fsub.EQ $dest,$x,$y
Fabs.EQ $dest,$dest
.enddef

.define clamp dest(reg),x(val),min(val),max(val)
cmp $x,$min
mov.LS $dest,$min
cmp $x,$max
mov.GE $dest,$max
.enddef
.define fclamp dest(reg),x(val),min(val),max(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fcmp.EQ $x,$min
Fmov.FLS $dest,$min
Fcmp $x,$max
Fmov.FGE $dest,$max
.enddef

@ Bits operation instructions.

.define sbit dest(reg),value(val),nth(val)
push r0
mov r0,#1
shif r0,r0,$nth,LEFT
log $dest,$val,r0,ORR
pull r0
.enddef

.define cbit dest(reg),value(val),nth(val)
push r0
mov r0,#1
shif r0,r0,$nth,LEFT
log r0,r0,#0,NOT
log $dest,$val,r0,AND
pull r0
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

@ a(sin,cos,tan) instructions.

.define asin dest(reg),x(val)
sin $dest,$x
cmp $_float_mode_,#1
Fpow.EQ $dest,#-1
.enddef

.define atan dest(reg),x(val)
tan $dest,$x
cmp $_float_mode_,#1
Fpow.EQ $dest,#-1
.enddef

.define acos dest(reg),x(val)
cos $dest,$x
cmp $_float_mode_,#1
Fpow.EQ $dest,#-1
.enddef

.module
.break

@ Label Bodies.

_std_factorial_loop:
    mul r1,r1,r0
    decr r0
    cmp r0,#0
    jmp.EQ _std_factorial_loop

_std_clz_loop:
    logi r2,r3,r0,AND
    cmp r2,#0 
    incr.GE r3
    shif r0,#1,LEFT
    cmp r4,#64
    jmp.LT _std_clz_loop