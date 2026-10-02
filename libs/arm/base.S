.define cbz label(lbl),val(val)
cmp $val,#0
call.EQ $lbl
.enddef
.define cbnz label(lbl),val(val)
cmp $val,#0
call.NE $label
.enddef

.define b label(lbl)
jmp $label
.enddef
.define bl label(lbl)
jmp $label
.enddef

.define and dest(reg),x(val),y(val)
logi $dest,$x,$y,AND
.enddef
.define orr dest(reg),x(val),y(val)
logi $dest,$x,$y,ORR
.enddef
.define mvn dest(reg),x(val)
logi $dest,$x,#0,NOT
.enddef
.define eor dest(reg),x(val),y(val)
logi $dest,$x,$y,XOR
.enddef
.define bic dest(reg),x(val),y(val)
push r0
logi r0,$y,#0,NOT
logi $dest,$x,r0,AND
pull r0
.enddef

.define lsl dest(reg),val(val),amount(val)
shif $dest,$val,$amount,LEFT
.enddef
.define lsr dest(reg),val(val),amount(val)
shif $dest,$val,$amount,RIGHT
.enddef
.define asl dest(reg),val(val),amount(val)
push r0
shif $dest,$val,$amount,LEFT
sub r0,$dest,#1
logi $dest,$dest,$val,ORR
pull r0
.enddef
.define asr dest(reg),val(val),amount(val)
push r0
shif $dest,$val,$amount,RIGHT
add r0,$dest,#1
logi $dest,$dest,$val,ORR
.enddef


.module
.break