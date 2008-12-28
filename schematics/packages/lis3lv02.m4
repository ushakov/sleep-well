include(common.m4)

define(`ml', `eval(`$1'*10000/254)')
# VPad(X Y1 Y2 N)
define(`VPad', `Pad[ `$1' `$2' `$1' `$3' ml(50) ml(30) ml(8) "$4" "$4" 0x00000100 ]')
# HPad(Y X1 X2 N)
define(`HPad', `Pad[ `$2' `$1' `$3' `$1' ml(50) ml(30) ml(8) "$4" "$4" 0x00000100 ]')

define(`PKG_LIS3LV02',
	`
#everything in 1/100mm -> 1/100mil (* 10000 / 254)
	define(`XO', `eval(ml(100))')
	define(`YO', `eval(ml(100))')
	define(`W', `eval(ml(750))')
	define(`H', `eval(ml(440))')
	define(`XC', `eval(XO+W/2)')
	define(`YC', `eval(YO+H/2)')
	define(`OFF', `eval(ml(65))')
	define(`YL', `eval(YO+OFF)')
	define(`YU', `eval(YO+H-OFF)')
	define(`XL', `eval(XO+OFF)')
	define(`XR', `eval(XO+W-OFF)')
	define(`E', `eval(ml(100))')
	define(`XF', `eval(XC-3*E+E/2)')
	define(`YF', `eval(YC-E/2)')
	define(`PL', `eval(ml(140))')
	define(`P', `1')
Element[0x00000000 "$1" "`$2'" "$3" XO YO XC YC 0 100 0x00000000]
(
        forloop(`i', 0, 5,
		`
	VPad(eval(XF+i*E),YU,eval(YU+PL),eval(i+1))
	VPad(eval(XF+i*E),YL,eval(YL-PL),eval(14-i))
		')
        forloop(`i', 0, 1,
		`
	HPad(eval(YF+i*E),XR,eval(XR+PL),eval(16-i))
	HPad(eval(YF+i*E),XL,eval(XL-PL),eval(7+i))
		')
	Pad[eval(XC-ml(200)) eval(YC+H/2-ml(170)) eval(XC-ml(200)) eval(YC+H/2-ml(170)) ml(30) 0 0 "20" "20" 0x00000100 ]
	ElementLine[XO YO XO eval(YO+H) 1000]
	ElementLine[XO YO eval(XO+W) YO 1000]
	ElementLine[eval(XO+W) eval(YO+H) eval(XO+W) YO 1000]
	ElementLine[eval(XO+W) eval(YO+H) XO eval(YO+H) 1000]
)')