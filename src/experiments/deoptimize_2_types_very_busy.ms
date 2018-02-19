
/*
	Demonstrate no inline caching versus inline caching in interpreter 
	versus immutable JIT versus Deoptimization

	This example uses 2 types (inheritance based) in 50/50 blocks, which is good for deoptimization

*/

class Base
{
	var a;
	func init() {
		a = 10;
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
    func get_val10()
    {
        a = a + 1;
        return a;
    }
    func get_val11()
    {
        a = a + 1;
        return a;
    }
    func get_val12()
    {
        a = a + 1;
        return a;
    }
    func get_val13()
    {
        a = a + 1;
        return a;
    }
    func get_val14()
    {
        a = a + 1;
        return a;
    }
    func get_val15()
    {
        a = a + 1;
        return a;
    }
    func get_val16()
    {
        a = a + 1;
        return a;
    }
    func get_val17()
    {
        a = a + 1;
        return a;
    }
    func get_val18()
    {
        a = a + 1;
        return a;
    }
    func get_val19()
    {
        a = a + 1;
        return a;
    }
    func get_val20()
    {
        a = a + 1;
        return a;
    }
    func get_val21()
    {
        a = a + 1;
        return a;
    }
    func get_val22()
    {
        a = a + 1;
        return a;
    }
    func get_val23()
    {
        a = a + 1;
        return a;
    }
    func get_val24()
    {
        a = a + 1;
        return a;
    }
    func get_val25()
    {
        a = a + 1;
        return a;
    }
    func get_val26()
    {
        a = a + 1;
        return a;
    }
    func get_val27()
    {
        a = a + 1;
        return a;
    }
    func get_val28()
    {
        a = a + 1;
        return a;
    }
    func get_val29()
    {
        a = a + 1;
        return a;
    }
    func get_val30()
    {
        a = a + 1;
        return a;
    }
    func get_val31()
    {
        a = a + 1;
        return a;
    }
    func get_val32()
    {
        a = a + 1;
        return a;
    }
    func get_val33()
    {
        a = a + 1;
        return a;
    }
    func get_val34()
    {
        a = a + 1;
        return a;
    }
    func get_val35()
    {
        a = a + 1;
        return a;
    }
    func get_val36()
    {
        a = a + 1;
        return a;
    }
    func get_val37()
    {
        a = a + 1;
        return a;
    }
    func get_val38()
    {
        a = a + 1;
        return a;
    }
    func get_val39()
    {
        a = a + 1;
        return a;
    }
    func get_val40()
    {
        a = a + 1;
        return a;
    }
    func get_val41()
    {
        a = a + 1;
        return a;
    }
    func get_val42()
    {
        a = a + 1;
        return a;
    }
    func get_val43()
    {
        a = a + 1;
        return a;
    }
    func get_val44()
    {
        a = a + 1;
        return a;
    }
    func get_val45()
    {
        a = a + 1;
        return a;
    }
    func get_val46()
    {
        a = a + 1;
        return a;
    }
    func get_val47()
    {
        a = a + 1;
        return a;
    }
    func get_val48()
    {
        a = a + 1;
        return a;
    }
    func get_val49()
    {
        a = a + 1;
        return a;
    }
    func get_val50()
    {
        a = a + 1;
        return a;
    }
    func get_val51()
    {
        a = a + 1;
        return a;
    }
    func get_val52()
    {
        a = a + 1;
        return a;
    }
    func get_val53()
    {
        a = a + 1;
        return a;
    }
    func get_val54()
    {
        a = a + 1;
        return a;
    }
    func get_val55()
    {
        a = a + 1;
        return a;
    }
    func get_val56()
    {
        a = a + 1;
        return a;
    }
    func get_val57()
    {
        a = a + 1;
        return a;
    }
    func get_val58()
    {
        a = a + 1;
        return a;
    }
    func get_val59()
    {
        a = a + 1;
        return a;
    }
    func get_val60()
    {
        a = a + 1;
        return a;
    }
    func get_val61()
    {
        a = a + 1;
        return a;
    }
    func get_val62()
    {
        a = a + 1;
        return a;
    }
    func get_val63()
    {
        a = a + 1;
        return a;
    }
    func get_val64()
    {
        a = a + 1;
        return a;
    }
    func get_val65()
    {
        a = a + 1;
        return a;
    }
    func get_val66()
    {
        a = a + 1;
        return a;
    }
    func get_val67()
    {
        a = a + 1;
        return a;
    }
    func get_val68()
    {
        a = a + 1;
        return a;
    }
    func get_val69()
    {
        a = a + 1;
        return a;
    }
    func get_val70()
    {
        a = a + 1;
        return a;
    }
    func get_val71()
    {
        a = a + 1;
        return a;
    }
    func get_val72()
    {
        a = a + 1;
        return a;
    }
    func get_val73()
    {
        a = a + 1;
        return a;
    }
    func get_val74()
    {
        a = a + 1;
        return a;
    }
    func get_val75()
    {
        a = a + 1;
        return a;
    }
    func get_val76()
    {
        a = a + 1;
        return a;
    }
    func get_val77()
    {
        a = a + 1;
        return a;
    }
    func get_val78()
    {
        a = a + 1;
        return a;
    }
    func get_val79()
    {
        a = a + 1;
        return a;
    }
    func get_val80()
    {
        a = a + 1;
        return a;
    }
    func get_val81()
    {
        a = a + 1;
        return a;
    }
    func get_val82()
    {
        a = a + 1;
        return a;
    }
    func get_val83()
    {
        a = a + 1;
        return a;
    }
    func get_val84()
    {
        a = a + 1;
        return a;
    }
    func get_val85()
    {
        a = a + 1;
        return a;
    }
    func get_val86()
    {
        a = a + 1;
        return a;
    }
    func get_val87()
    {
        a = a + 1;
        return a;
    }
    func get_val88()
    {
        a = a + 1;
        return a;
    }
    func get_val89()
    {
        a = a + 1;
        return a;
    }
    func get_val90()
    {
        a = a + 1;
        return a;
    }
    func get_val91()
    {
        a = a + 1;
        return a;
    }
    func get_val92()
    {
        a = a + 1;
        return a;
    }
    func get_val93()
    {
        a = a + 1;
        return a;
    }
    func get_val94()
    {
        a = a + 1;
        return a;
    }
    func get_val95()
    {
        a = a + 1;
        return a;
    }
    func get_val96()
    {
        a = a + 1;
        return a;
    }
    func get_val97()
    {
        a = a + 1;
        return a;
    }
    func get_val98()
    {
        a = a + 1;
        return a;
    }
    func get_val99()
    {
        a = a + 1;
        return a;
    }
    func get_val100()
    {
        a = a + 1;
        return a;
    }
    func get_val101()
    {
        a = a + 1;
        return a;
    }
    func get_val102()
    {
        a = a + 1;
        return a;
    }
    func get_val103()
    {
        a = a + 1;
        return a;
    }
    func get_val104()
    {
        a = a + 1;
        return a;
    }
    func get_val105()
    {
        a = a + 1;
        return a;
    }
    func get_val106()
    {
        a = a + 1;
        return a;
    }
    func get_val107()
    {
        a = a + 1;
        return a;
    }
    func get_val108()
    {
        a = a + 1;
        return a;
    }
    func get_val109()
    {
        a = a + 1;
        return a;
    }
    func get_val110()
    {
        a = a + 1;
        return a;
    }
    func get_val111()
    {
        a = a + 1;
        return a;
    }
    func get_val112()
    {
        a = a + 1;
        return a;
    }
    func get_val113()
    {
        a = a + 1;
        return a;
    }
    func get_val114()
    {
        a = a + 1;
        return a;
    }
    func get_val115()
    {
        a = a + 1;
        return a;
    }
    func get_val116()
    {
        a = a + 1;
        return a;
    }
    func get_val117()
    {
        a = a + 1;
        return a;
    }
    func get_val118()
    {
        a = a + 1;
        return a;
    }
    func get_val119()
    {
        a = a + 1;
        return a;
    }
    func get_val120()
    {
        a = a + 1;
        return a;
    }
    func get_val121()
    {
        a = a + 1;
        return a;
    }
    func get_val122()
    {
        a = a + 1;
        return a;
    }
    func get_val123()
    {
        a = a + 1;
        return a;
    }
    func get_val124()
    {
        a = a + 1;
        return a;
    }
    func get_val125()
    {
        a = a + 1;
        return a;
    }
    func get_val126()
    {
        a = a + 1;
        return a;
    }
    func get_val127()
    {
        a = a + 1;
        return a;
    }
    func get_val128()
    {
        a = a + 1;
        return a;
    }
    func get_val129()
    {
        a = a + 1;
        return a;
    }
    func get_val130()
    {
        a = a + 1;
        return a;
    }
    func get_val131()
    {
        a = a + 1;
        return a;
    }
    func get_val132()
    {
        a = a + 1;
        return a;
    }
    func get_val133()
    {
        a = a + 1;
        return a;
    }
    func get_val134()
    {
        a = a + 1;
        return a;
    }
    func get_val135()
    {
        a = a + 1;
        return a;
    }
    func get_val136()
    {
        a = a + 1;
        return a;
    }
    func get_val137()
    {
        a = a + 1;
        return a;
    }
    func get_val138()
    {
        a = a + 1;
        return a;
    }
    func get_val139()
    {
        a = a + 1;
        return a;
    }
    func get_val140()
    {
        a = a + 1;
        return a;
    }
    func get_val141()
    {
        a = a + 1;
        return a;
    }
    func get_val142()
    {
        a = a + 1;
        return a;
    }
    func get_val143()
    {
        a = a + 1;
        return a;
    }
    func get_val144()
    {
        a = a + 1;
        return a;
    }
    func get_val145()
    {
        a = a + 1;
        return a;
    }
    func get_val146()
    {
        a = a + 1;
        return a;
    }
    func get_val147()
    {
        a = a + 1;
        return a;
    }
    func get_val148()
    {
        a = a + 1;
        return a;
    }
    func get_val149()
    {
        a = a + 1;
        return a;
    }
    func get_val150()
    {
        a = a + 1;
        return a;
    }
    func get_val151()
    {
        a = a + 1;
        return a;
    }
    func get_val152()
    {
        a = a + 1;
        return a;
    }
    func get_val153()
    {
        a = a + 1;
        return a;
    }
    func get_val154()
    {
        a = a + 1;
        return a;
    }
    func get_val155()
    {
        a = a + 1;
        return a;
    }
    func get_val156()
    {
        a = a + 1;
        return a;
    }
    func get_val157()
    {
        a = a + 1;
        return a;
    }
    func get_val158()
    {
        a = a + 1;
        return a;
    }
    func get_val159()
    {
        a = a + 1;
        return a;
    }
    func get_val160()
    {
        a = a + 1;
        return a;
    }
    func get_val161()
    {
        a = a + 1;
        return a;
    }
    func get_val162()
    {
        a = a + 1;
        return a;
    }
    func get_val163()
    {
        a = a + 1;
        return a;
    }
    func get_val164()
    {
        a = a + 1;
        return a;
    }
    func get_val165()
    {
        a = a + 1;
        return a;
    }
    func get_val166()
    {
        a = a + 1;
        return a;
    }
    func get_val167()
    {
        a = a + 1;
        return a;
    }
    func get_val168()
    {
        a = a + 1;
        return a;
    }
    func get_val169()
    {
        a = a + 1;
        return a;
    }
    func get_val170()
    {
        a = a + 1;
        return a;
    }
    func get_val171()
    {
        a = a + 1;
        return a;
    }
    func get_val172()
    {
        a = a + 1;
        return a;
    }
    func get_val173()
    {
        a = a + 1;
        return a;
    }
    func get_val174()
    {
        a = a + 1;
        return a;
    }
    func get_val175()
    {
        a = a + 1;
        return a;
    }
    func get_val176()
    {
        a = a + 1;
        return a;
    }
    func get_val177()
    {
        a = a + 1;
        return a;
    }
    func get_val178()
    {
        a = a + 1;
        return a;
    }
    func get_val179()
    {
        a = a + 1;
        return a;
    }
    func get_val180()
    {
        a = a + 1;
        return a;
    }
    func get_val181()
    {
        a = a + 1;
        return a;
    }
    func get_val182()
    {
        a = a + 1;
        return a;
    }
    func get_val183()
    {
        a = a + 1;
        return a;
    }
    func get_val184()
    {
        a = a + 1;
        return a;
    }
    func get_val185()
    {
        a = a + 1;
        return a;
    }
    func get_val186()
    {
        a = a + 1;
        return a;
    }
    func get_val187()
    {
        a = a + 1;
        return a;
    }
    func get_val188()
    {
        a = a + 1;
        return a;
    }
    func get_val189()
    {
        a = a + 1;
        return a;
    }
    func get_val190()
    {
        a = a + 1;
        return a;
    }
    func get_val191()
    {
        a = a + 1;
        return a;
    }
    func get_val192()
    {
        a = a + 1;
        return a;
    }
    func get_val193()
    {
        a = a + 1;
        return a;
    }
    func get_val194()
    {
        a = a + 1;
        return a;
    }
    func get_val195()
    {
        a = a + 1;
        return a;
    }
    func get_val196()
    {
        a = a + 1;
        return a;
    }
    func get_val197()
    {
        a = a + 1;
        return a;
    }
    func get_val198()
    {
        a = a + 1;
        return a;
    }
    func get_val199()
    {
        a = a + 1;
        return a;
    }


	func f() {
		return a;
	}
}

class Sub : Base
{
	var d;
	func init() {
		d = 20;
		a = 9;
	}
	
	func f() {
		return a;
	}
}

func runner(obj) {
	var t = obj.f();
	return t;
};

var i = 0;
var b = new Base;
b.init();
var s = new Sub;
s.init();
var x;
while ( i < 100000 ) {
	if ( i < 50000) {
		x = b;
	}
	if (i >= 50000){
		x = s;
	}
	runner(x);
	i = i + 1;
}
i.dump();
