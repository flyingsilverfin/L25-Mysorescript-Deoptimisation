class Base
{
	var a;
	var b;
}

class Sub : Base
{
	var c;
	var d;
	func inherited()
	{
		a = "a";
		b = "b";
		c = "c";
		d = "d";
		a.dump();
	}
}

var s = new Sub;

s.inherited();


/*
		b.dump();
		c.dump();
		d.dump();
		"\n".dump();
*/
