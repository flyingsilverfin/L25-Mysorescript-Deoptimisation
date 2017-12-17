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

class Getter {
	/* this will get JIT'ed so it's called many times, and therefore implementing inline caching */
	func getter(a) {
		return a.get_val();
	}
}


var aclass = new A.init();
var get = new Getter;
var i = 0;
var s = 0;
while ( i < 1000000 ) {
	s = s + get.getter(aclass);
	i = i + 1;
}
s.dump();

