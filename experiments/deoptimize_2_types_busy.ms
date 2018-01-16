
/*
	Demonstrate no inline caching versus inline caching in interpreter 
	versus immutable JIT versus Deoptimization

	This example uses 2 types (inheritance based) in 50/50 blocks, which is good for deoptimization

*/

class Base
{
	var a;
	func init() {
		a = 10;
	}



	func get_val10()
	{
		a = a + 1;
		return a;
	}

	func get_val20()
	{
		a = a + 1;
		return a;
	}
	
	func get_val30()
	{
		a = a + 1;
		return a;
	}

	func get_val40()
	{
		a = a + 1;
		return a;
	}

	func get_val50()
	{
		a = a + 1;
		return a;
	}


	func get_val60()
	{
		a = a + 1;
		return a;
	}

	func get_val70()
	{
		a = a + 1;
		return a;
	}
	
	func get_val80()
	{
		a = a + 1;
		return a;
	}

	func get_val90()
	{
		a = a + 1;
		return a;
	}

	func get_val0()
	{
		a = a + 1;
		return a;
	}




	func get_val1()
	{
		a = a + 1;
		return a;
	}

	func get_val2()
	{
		a = a + 1;
		return a;
	}
	
	func get_val3()
	{
		a = a + 1;
		return a;
	}

	func get_val4()
	{
		a = a + 1;
		return a;
	}

	func get_val5()
	{
		a = a + 1;
		return a;
	}


	func get_val6()
	{
		a = a + 1;
		return a;
	}

	func get_val7()
	{
		a = a + 1;
		return a;
	}
	
	func get_val8()
	{
		a = a + 1;
		return a;
	}

	func get_val9()
	{
		a = a + 1;
		return a;
	}

	func get_val0()
	{
		a = a + 1;
		return a;
	}
	func f() {
		return a;
	}
}

class Sub : Base
{
	var d;
	func init() {
		d = 20;
		a = 9;
	}
	
	func f() {
		return a;
	}
}

func runner(obj) {
	var t = obj.f();
	return t;
};

var i = 0;
var b = new Base;
b.init();
var s = new Sub;
s.init();
var x;
while ( i < 100000 ) {
	if ( i < 50000) {
		x = b;
	}
	if (i >= 50000){
		x = s;
	}
	runner(x);
	i = i + 1;
}
i.dump();
