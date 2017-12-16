class A
{
	var a;
	func init() {
		a = 0;
		return self;
	}
	/* increase cost of findinga method in the class  since it's a linear scan */
	func f1() 
	{
		return 2;
	}

	func f2()
	{
		return 3;
	}
	
	func get_val()
	{
		a = a + 1;
		return a;
	}
}

/* this will get JIT'ed so it's called many times, and therefore implementing inline caching */
func getter(a) {
	return a.get_val();
};

/* runner isn't actually compiled since it's only run once */
func runner() {
	var aclass = new A.init();
	var i = 0;
	var s = 0;
	while ( i < 10000000 ) {
		s = s + getter(aclass);
		/*s.dump();*/
		i = i + 1;
	}
	return s;
};

runner().dump();
