.define swap x(reg),y(reg)
push $x
mov $x,$y
pull $y
.enddef
.module
.break