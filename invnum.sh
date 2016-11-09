#
# SYNOPSIS: 
#    invnum.sh [FILE]
#
# DESCRIPTION:
#    Reads from FILE (or stdin, if no file given) and waits for the first line not starting with a '#'.
#    This line has to be of the form "p0 p1 p2 ..." (e.g. "1 3 2 5 4").
#
#    Outputs the permuation in the form "p = (p0 p1 p2 ...)" and (on a new line) the resulting inversion number ("inv(p) = F(p) = INV_NUM").
#
#    The inversion number of a permutation p (Written "inv(p)" or "F(p)") is defined as follows:
#      F(p) := "Number of cases in which i < k, but p(i) > p(k)"
#    To calculate the number for a given permutation, one counts the numbers left to each p(i) which are smaller than p(i).
#    e.g.:
#      p=(1 2 5 4 3)
#      F(p)=3, since "4 left to 5" & "3 left to 5" & "3 left to 4"

if [ -n "$1" ];then
	exec 0< "$1"
fi

if [ -t 0 ];then
	inprefix=">"
else
	inprefix=""
fi

printf -- "$inprefix"
while read -r line;do
	if [ "${line:0:1}" != "#" ];then
		break
	fi
	printf -- "$inprefix"
done
list=($line)
invnumber=0
i=0
while [ "$i" -lt "${#list[@]}" ];do
	j=$(($i + 1))
	while [ "$j" -lt "${#list[@]}" ];do
		if [ "${list[$j]}" -lt "${list[$i]}" ];then
			invnumber=$(($invnumber + 1))
		fi
#		printf " j = $j\n"
		j=$(($j + 1))
	done
#	printf "i = $i\n"
	i=$(($i + 1))
done
printf "p = ($line)\n"
printf "inv(p) = F(p) = $invnumber\n"
