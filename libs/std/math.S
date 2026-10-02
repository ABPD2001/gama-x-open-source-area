@ Math instructions of Gama-X Open-Source Standard Library.
@ Abolfazl Pouretemadi [10-1-2026]
@ R0-R4 General-Registers and f0-f2 Float-Registers are required as ABI.

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
Fmul.EQ $dest,$dest,#180
Fdiv.EQ $dest,$dest,PI
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
mov.EQ $dest,$x
.enddef
.define fclamp dest(reg),x(val),min(val),max(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fcmp.EQ $x,$min
Fmov.FLS $dest,$min
Fcmp $x,$max
Fmov.FGE $dest,$max
Fmov.FEQ $dest,$x
.enddef

.define lerp dest(reg),x(val),y(val),t(val)
push r0
add $dest,$x,$t
sub r0,$y,$x
mul $dest,$dest,r0
pull r0
.enddef

.define flerp dest(reg),x(val),y(val),t(val)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fpush.EQ f0
Fadd.EQ $dest,$x,$t
Fsub.EQ f0,$y,$x
Fmul.EQ $dest,$dest,f0
Fpull.EQ f0
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

.module
.break

@ Label Bodies.

_std_factorial_loop:
    mul r1,r1,r0
    decr r0
    cmp r0,#1
    jmp.GT _std_factorial_loop

    
_std_maprange: @ result would be saved in r0, R0: value, R1: min1, R2: max1, R3: min2, R4: max2
    Fpush f0
    Fpush f1

    sub r0,r0,r1
    sub r1,r2,r1
    mvrf r0,f0
    mvrf r1,f1
    Fdiv f0,f0,f1
    mvrf r3,f1
    Fsub f0,f0,f1
    Fmul f0,f0,#100
    sub r0,r4,r3
    mvrf r0,f1
    Fdiv f0,f0,f1
    Fflr f0,f0
    mvfr f0,r0

    Fpull f1
    Fpull f0