/* Um programa para calcular o mdc
   segundo o algoritmo de Euclides. */

int gcd (int u, int v)
{       
	
	while(u != v)
	{
		if (u > v)
			u = u - v;
		else
			v = v - u;
	}
	return u;
}

void main(void)
{	

}
