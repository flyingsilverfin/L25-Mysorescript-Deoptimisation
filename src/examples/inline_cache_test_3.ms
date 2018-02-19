
class A
{
	var a;
	func init() {
		a = 0;
		return self;
	}

	func get_val()
	{
		a = a + 1;
		return a;
	}
}

class B :  A 
{

	func get_val() {
		return 10;
	}
}

/* if allowed, this will get JIT'ed so it's called many times, and therefore implementing inline caching */
func getter(a) {
	var c = a.get_val();
	return c;
};

var aclass = new A.init();
var i = 0;
var s = 0;
var v = 0;
while ( i < 100 ) {
	v = getter(aclass); /* need SSA-like forms for now */
	v.dump();
	s = s + v;
	i.dump();
	i = i + 1;
}
s.dump();
