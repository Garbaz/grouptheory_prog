/*
Returns the inversion number of the given permutation (See https://en.wikipedia.org/wiki/Inversion_%28discrete_mathematics%29)

perm = Array of numbers representing the permutation. (e.g. [2,1], which maps 1->2 and 2->1)

return: Returns the inversion number

*/

var transCount = 1;
function invnum(perm)
{
	var result = 0;
	for(i = 0; i < perm.length; i++) {
		for(j = i+1; j < perm.length; j++) {
			if(perm[i] != "" && perm[j] != "" && !isNaN(perm[i]) && !isNaN(perm[j]) && Number(perm[j]) < Number(perm[i])) {
				result++;
			}
		}
	}
	return result;
}

function calc() {
	document.getElementById("result").value = invnum(document.getElementById("perm").value.split(' '));
}

function evalTrans() {
	var modPerm = document.getElementById("perm").value.split(' ');
	for(i = 0; i < transCount; i++) {
		var tmpTransIn = document.getElementById("transInput" + i);
		if(tmpTransIn != null && tmpTransIn.value != "") {
			var tmpTrans = tmpTransIn.value.split(' ');
			if(tmpTrans[0] != "" && tmpTrans[1] != "" && !isNaN(tmpTrans[0]) && !isNaN(tmpTrans[1])) {
				var a = parseInt(tmpTrans[0],10)-1;
				var b = parseInt(tmpTrans[1],10)-1;
				var tmpVal = modPerm[a];
				modPerm[a] = modPerm[b];
				modPerm[b] = tmpVal;
				document.getElementById("transOutput" + i).value = modPerm.toString().replace(/,/g," ");
			}
		}
		
	}
}
function keyHandlePerm(e) {
	if(e.which == 13 || e.keyCode == 13) {
	setTimeout(calc());
	}
//	else {
//		console.log(e.keyCode);
//	}
}

function keyHandleTrans(e,id) {
	if((e.which == 13 || e.keyCode == 13)) {
		if(document.activeElement.id == "transInput" + (transCount-1)) {
			var currTrans = document.createElement("div");
			currTrans.innerHTML = '<input id="transInput' + transCount + '" type="text" size="10" onkeypress="keyHandleTrans(event)"><input id="transOutput' + transCount + '" type="text" size="100" readonly><br>'
			currTrans.setAttribute("id","trans" + transCount);
			document.body.appendChild(currTrans);
			document.getElementById("transInput" + transCount).focus();
			transCount++;
		}
		else {
			document.getElementById("transInput" + (parseInt(document.activeElement.id.replace(/transInput/,''))+1)).focus();
		}
	}
	evalTrans();
}

function clearTrans() {
	for(i = 1; i < transCount; i++) {
		document.body.removeChild(document.getElementById("trans" + i));
	}
	transCount = 1;
}
