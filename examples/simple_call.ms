class c 
{

	func init() {
		return self;
	}

	func call_me(x) {
		var y = x + 2;
		return y;
	}
}

var instance = new C.init();
var i = 0;
while ( i < 10 ) {
	var x = instance.call_me(i);
	x.dump();
	i = i + 1;
}


