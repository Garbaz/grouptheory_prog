/*
Returns the inversion number of the given permutation (See https://en.wikipedia.org/wiki/Inversion_%28discrete_mathematics%29)

perm = Array of numbers representing the permutation. (e.g. [2,1], which maps 1->2 and 2->1)

return: Returns the inversion number

*/
function invnumber(perm)
{
	var result = 0;
	for(i = 0; i < perm.length; i++) {
		for(m in perm.slice(i+1,perm.length) {
			if(m > perm[i]) {
				result++;
			}
		}
	}
	return result;
}
