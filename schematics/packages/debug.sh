echo "$1" | m4 $2 - | awk '/^[ \t]*$/ {next} {print}' | less -X
