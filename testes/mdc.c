
int gcd(int a, int b)
{

	while(a != b)
	{
		if(a > b)
			a = a - b;
		else
			b = b - a;
	}

	return a;

}
void main(void)
{
	int a;
	int b;

	a = input();
	output(a);
	b = input();
	output(b);
	output(gcd(a,b));

}
