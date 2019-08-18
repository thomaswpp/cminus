/* Um programa para calcular o mdc
   segundo o algoritmo de Euclides. */

int gcd (int u, int v)
{     

	while(u != v){
		if(u > v)	u = u -v;
		else	v = v -u;
	}

	return u;

	/*  
	if (v == 0) return u ;
	else return gcd(v,u-u/v*v);
	 u-u/v*v == u mod v */
}

void main(void)
{	int x; int y;
	x = input(); 
	output(x);
	y = input();
	output(y);
	output(gcd(x,y));
}
