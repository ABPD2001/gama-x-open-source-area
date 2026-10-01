.define swap x(reg),y(reg)
push $x
mov $x,$y
pull $y
.enddef

.define fill dest(reg)
mov $dest,#0
logi $dest,$dest,#0,NOT
.enddef

.define clear dest(reg)
mov $dest,#0
.enddef

.module
.break