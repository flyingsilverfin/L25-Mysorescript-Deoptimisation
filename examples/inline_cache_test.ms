class A
{
	var a;
	func init() {
		a = 0;
		return self;
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



	func get_val()
	{
		a = a + 1;
		a.dump();
		return a;
	}
}

/* if allowed, this will get JIT'ed so it's called many times, and therefore implementing inline caching */
func getter(a) {
	var b = 10;
	var c = a.get_val();
	return b+c;
};

var aclass = new A.init();
var i = 0;
var s = 0;
while ( i < 100 ) {
	s = s + getter(aclass);
	/*s.dump();*/
	i = i + 1;
}
s.dump();
