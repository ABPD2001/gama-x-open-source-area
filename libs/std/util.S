.define swap x(reg),y(reg)
cmp $_float_mode_,#1
push.NE $x
mov.NE $x,$y
pull.NE $y
Fpush.EQ $x
Fmov.EQ $x,$y
Fpull.EQ $y
.enddef

.define smothSwap x(reg),val(reg)
cmp $_float_mode_,#0
cmp.EQ $x,#0
Fcmp.NE $x,#0
decr.GT $x
incr.GT $y
Fdecr.FGT $x
Fincr.FGT $y
.enddef

.define signRevert x(reg)
abs.LS $x,$x
mul.GT $x,$x,#-1
.enddef
.define fsignRevert x(reg)
Fabs.LS $x,$x
Fmul.GT $x,$x,#-1
.enddef

.define fill dest(reg)
cmp $_float_mode_,#1
mov.NE $dest,#0
logi.NE $dest,$dest,#0,NOT
Fmov.EQ $dest,#0
Flogi.EQ $dest,$dest,#0,NOT
.enddef

.define clear dest(reg)
cmp $_float_mode_,#1
mov.NE $dest,#0
Fmov.EQ $dest,#0
.enddef

.module
.break

_smooth_act_:
    decr r0
    incr r1