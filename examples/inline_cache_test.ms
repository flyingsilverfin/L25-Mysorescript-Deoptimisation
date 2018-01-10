class A
{
	var a;
	func init() {
		a = 0;
		return self;
	}
	func get_val1()
	{
		a = a + 12;
		return a;
	}

	func get_val2()
	{
		a = a + 13;
		return a;
	}
	
	func get_val3()
	{
		a = a + 15;
		return a;
	}

	func get_val4()
	{
		a = a + 14;
		return a;
	}

	func get_val5()
	{
		a = a + 21;
		return a;
	}


	func get_val10()
	{
		a = a + 61;
		return a;
	}

	func get_val20()
	{
		a = a + 51;
		return a;
	}
	
	func get_val30()
	{
		a = a + 15;
		return a;
	}

	func get_val40()
	{
		a = a + 14;
		return a;
	}

	func get_val50()
	{
		a = a +2231;
		return a;
	}

	func get_val6()
	{
		a = a + 41;
		return a;
	}

	func get_val7()
	{
		a = a + 11;
		return a;
	}
	
	func get_val8()
	{
		a = a + 21;
		return a;
	}

	func get_val49()
	{
		a = a + 1;
		return a;
	}

	func get_val51()
	{
		a = a + 1;
		return a;
	}


	func get_val101()
	{
		a = a + 1;
		return a;
	}

	func get_val201()
	{
		a = a + 1;
		return a;
	}
	
	func get_val301()
	{
		a = a + 1;
		return a;
	}

	func get_val401()
	{
		a = a + 1;
		return a;
	}

	func get_val501()
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
