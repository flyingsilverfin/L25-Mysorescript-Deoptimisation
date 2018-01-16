
class Base
{
	var a;
	func init() {
		a = 10;
		"Init Base\n".print();
	}

	func f() {
		/*a.dump();*/
		"called Base\n".print();
	}
}

class Sub : Base
{
	var d;
	func init() {
		d = 20;
		a = 9;
		"Init Sub\n".print();
	}
	
	func f() {
		/*d.dump();*/
		"Called Sub\n".print();
	}
}

func runner(obj) {
	obj.f();
};

var i = 0;
while ( i < 30 ) {
	var x;
	if ( i < 15) {
		x = new Base;
	}
	if (i >= 15){
		x = new Sub;
	}
	x.init();
	runner(x);
	i = i + 1;
}
