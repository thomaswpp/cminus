int calculadora(int op, int a, int b){
	output(a);
	output(b);
	if(op == 1){
		return a + b;
	} else if(op == 2){
		return a - b;
	} else if(op == 3){
		return a * b;
	} else if(op == 4){
		return a / b;
	} else if(op == 5){
		return a - a / b * b;
	}
}

void main(void){
	int a; int b;
	int op;
	int x;
	int v[10];

	a = input();
    output(a);
	b = input();
    output(b);
	op = input();
    output(op);
    /*datatohd(10, 1, a);*/
   	hdtodata(10, v[2], a);
    nextprocess(10, a);
    while(i < 37){
		hdmi(1, 0, i);
		i = i + 1;
	}
    regtohd(10, 1);
    hdtoreg(10, 1);
    /*hdmi(10, 1, a);
    hdmi(10, 1, a);*/

	x = calculadora(op, a, b);

	output(x);
}
