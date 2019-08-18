

int fatorial(int a)
{
	int i;
	i = a - 1;

	while(i > 0)
	{
		a = a * i;
		i = i - 1;
	}

	return a;
}

void main()
{

	int a;
	a = input();	
	output(fatorial(a));

}