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
while ( i < 10000 ) {
	if ( i < 5000) {
		x = b;
	}
	if (i >= 5000){
		x = s;
	}
	runner(x);
	i = i + 1;
}
i.dump();
