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

.define exp dest(reg),x(reg)
Fpow $dest,#2.71828,$x
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

.define max dest(reg),x(val),y(val)
cmp $x,$y
mov.GE $dest,$x
mov.LS $dest,$y
.enddef

.define min dest(reg),x(val),y(val)
cmp $x,$y
mov.GE $dest,$y
mov.LS $dest,$x
.enddef

.define dim dest(reg),x(val),y(val)
sub $dest,$x,$y
abs $dest,$dest
.enddef

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