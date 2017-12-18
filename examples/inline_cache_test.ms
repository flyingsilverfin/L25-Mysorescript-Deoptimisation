class A
{
	var a;
	func init() {
		a = 0;
		return self;
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



	func get_val()
	{
		a = a + 1;
		return a;
	}
}

/* if allowed, this will get JIT'ed so it's called many times, and therefore implementing inline caching */
func getter(a) {
	return a.get_val();
};

var aclass = new A.init();
var i = 0;
var s = 0;
while ( i < 1000000 ) {
	s = s + getter(aclass);
	/*s.dump();*/
	i = i + 1;
}
s.dump();
