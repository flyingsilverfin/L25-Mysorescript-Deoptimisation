/*
*
*func test_add() {
*	var x = 1;
*	var y = 2;
*	var z = 3;
*
*	var k = x + y;
*	var l = k+1;
*	var m = l+k;	
*
*	return k+l+m;
*};
*
*
*	NO CONSTANTS, NO DEAD CODE!
*/
func test_add_2() {
	var i = 1;
	var sum = 1;
	while ( i < 10000000000 ) {
		var x = (sum) / (i);
		sum = sum + x;
		i = i + i;
	}
	return sum;
};
	
		


/*test_add().dump();*/
test_add_2().dump();
