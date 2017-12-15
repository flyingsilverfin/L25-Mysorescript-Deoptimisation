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



func runner() {
	var aclass = new A.init();
	var i = 0;
	var s = 0;
	while ( i < 10000 ) {
		s = s + aclass.get_val();
		/*s.dump();*/
		i = i + 1;
	}
	return s;
};

runner().dump();
