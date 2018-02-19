/*

	Demonstrate no inline caching versus inline caching in the interpreter
	versus inline caching normal implementation, versus inline optimisation with type assumptions

*/


class A
{
	var a;
	func init() {
		a = 0;
		return self;
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
	func get_va5l()
	{
		a = a + 1;
		return a;
	}
	func get_val()
	{
		a = a + 1;
		return a;
	}
}

/* if allowed, this will get JIT'ed so it's called many times, and therefore implementing inline caching */
func getter(a) {
	var b = 10;
	var c = a.get_val();
	return c + b;
};

var aclass = new A.init();
var i = 0;
var s = 0;
var v = 0;
while ( i < 10000 ) {
	v = getter(aclass); /* need SSA-like forms for now */
	s = s + v;
	i = i + 1;
}
s.dump();
