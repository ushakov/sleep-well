include(common.m4)

define(`ml', `eval(`$1'*10000/254)')

# VPad(X Y L W N)
define(`VPad',
	`
	define(`X', `ml($1)')
	define(`Y', `ml($2)')
	define(`L', `ml($3)')
	define(`HW', `eval(ml($4)/2)')
	Pad[ eval(X) eval(Y+HW) eval(X) eval(Y+L-HW) eval(2*HW) ml(30) ml(8) "$5" "$5" 0x00000100 ]')
# HPad(X Y L W N)
define(`HPad',
	`
	define(`X', `ml($1)')
	define(`Y', `ml($2)')
	define(`L', `ml($3)')
	define(`HW', `eval(ml($4)/2)')
	Pad[ eval(X+HW) eval(Y) eval(X+L-HW) eval(Y) eval(2*HW) ml(30) ml(8) "$5" "$5" 0x00000100 ]')

define(`PKG_MICROSD',
	`
Element[0x00000000 "$1" "`$2'" "$3" 0 0 0 0 0 100 0x00000000]
(
	ElementLine[ml(1640) ml(1430) ml(1640) ml(5) 1000]
	ElementLine[ml(1640) ml(5) ml(560) ml(5) 1000]
	ElementLine[ml(560) ml(5) ml(560) ml(70) 1000]
	ElementLine[ml(560) ml(70) ml(10) ml(70) 1000]
	ElementLine[ml(10) ml(70) ml(10) ml(1430) 1000]
	ElementLine[ml(10) ml(1430) ml(1640) ml(1430) 1000]
	define(`YO', `750')
	define(`XO', `1545')
	VPad(XO,eval(YO+240),180,120,11)
	VPad(XO,eval(YO-315-180),180,120,12)
	VPad(eval(XO-1355),eval(YO+235),180,140,13)
	VPad(eval(XO-1355),eval(YO-235-180),180,140,14)
	define(`XF', `eval(XO-1065-180)')
	define(`YF', `eval(YO+385)')
	forloop(`i', 0, 7,
		`HPad(XF, eval(YF - i*110), 180, 80, eval(i+1))')
)')