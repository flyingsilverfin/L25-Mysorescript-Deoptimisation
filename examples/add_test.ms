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
*/

func test_add_2() {
	var i = foo();
	var sum = 1;
	while ( i < 10000000 ) {
		var tmp = foo();
		var x = (sum) / (i);
		sum = sum + x;
		i = i + tmp;
	}
	return sum;
};

func foo() {
	return 5;
};
	
		


/*test_add().dump();*/
test_add_2().dump();
