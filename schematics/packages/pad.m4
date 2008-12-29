define(`ml', `eval(`$1'*10000/254)')

# VPad(X Y L W N)
define(`VPad',
	`
	define(`X', `ml($1)')
	define(`Y', `ml($2)')
	define(`L', `ml($3)')
	define(`HW', `eval(ml($4)/2)')
	Pad[ eval(X) eval(Y+HW) eval(X) eval(Y+L-HW) eval(2*HW) ml(30) ml(8) "$5" "$5" 0x00000100 ]')

define(`PKG_SOLDERPAD',
	`
Element[0x00000000 "$1" "`$2'" "$3" 0 0 0 0 0 100 0x00000000]
(
	VPad(0, 0, eval(500), 100, 1)
)')