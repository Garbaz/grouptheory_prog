# SYNOPSIS:
#    tpcalc.sh [FILE]
# DESCRIPTION:
#    Reads all lines from FILE (or stdin, if no file given) and waits for the first line not starting with a '#'.
#    The first interpreted line has to be of the form "p0 p1 p2 ..." (e.g. "1 3 2 5 4"). This line initializes the permutation of (1 2 3 4 ...) on which the transformations are applied.
#    All subsequent (interpreted) lines have to be of the form "i j" (e.g. "1 5"). Theses lines descript the transformations applied to the initial permutation.
#    
#    Outputs a visual representation of each step and on the last line the resulting permutation.



if [ -n "$1" ];then
exec 0< "$1"
fi

if [ -t 0 ];then
	inprefix=">"
else
	inprefix=""
fi

list=("")

printf -- "$inprefix"
while read -r tp;do
#	printf "tp=$tp;list=%s\n" "${list[*]}"
	if [ "${tp:0:1}" != "#" ];then
		if [[ "$tp" != "exit" && "$tp" != "quit" && "$tp" != "q"  ]];then
			if [ "${list[*]}" != "" ];then
				tp=($tp)
				i=$((${tp[0]} -1))
				j=$((${tp[1]} -1))
				if [ "$i" -gt "$j" ];then
					c=0
					while [ "$c" -lt "$j" ];do
						printf "  "
						c=$(($c+1))
					done
					printf "+-"
					c=0
					while [ "$c" -lt "$(($i - $j - 1))" ];do
						printf -- "--"
						c=$(($c+1))
					done
					printf "+"
				else
					c=0
					while [ "$c" -lt "$i" ];do
						printf "  "
						c=$(($c+1))
					done
					printf "+-"
					c=0
					while [ "$c" -lt "$(($j - $i - 1))" ];do
						printf -- "--"
						c=$(($c+1))
					done
					printf "+"
				fi
				printf "       (%s)\n" "$(($i+1)) $(($j+1))"
				tmp=${list[$i]}
				list[$i]=${list[$j]}
				list[$j]=$tmp
				printf "%s\n" "${list[*]}"
			else 
				list=($tp)
				printf "%s\n" "${list[*]}"
			fi
			printf "$inprefix"
		else
			exit 0
		fi
	fi
done
#printf "%s\n" "${list[*]}"
