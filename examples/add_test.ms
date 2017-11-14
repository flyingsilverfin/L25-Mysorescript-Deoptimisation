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
*/
func test_add_2() {
	var i = 0;
	var sum = 0;
	while ( i < 3 ) {
		var x = (i) / (2);
		var y = x + 10;
		sum = sum + y;
		i = i + 1;
	}
	return sum;
};
	
		


/*test_add().dump();*/
test_add_2().dump();
