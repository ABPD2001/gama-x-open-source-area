.define swap x(reg),y(reg)
cmp $_float_mode_,#1
push.NE $x
mov.NE $x,$y
pull.NE $y
Fpush.EQ $x
Fmov.EQ $x,$y
Fpull.EQ $y
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