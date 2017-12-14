class C
{

	func init() {
		return self;
	}

	func call_me(x) {
		var y = x + 2;
		return y;
	}
}

func caller() {
	var instance = new C;
	var i = 0;
	while ( i < 10 ) {
		var x = instance.call_me(i);
		i = i + 1;
	}
	return i;
};

caller().dump();


