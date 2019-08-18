/* programa para ordena��o por sele��o de 
   uma matriz com dez elementos. */

int vet[ 10 ];

void sort( int a[], int low, int high)
{	
	int i; int k;

	int j;
	int l;
	int y;


	i = low;
	while (i < high-1)
    {
		int t;
		/*k = minloc(a,i,high);
		k = minloc(a,low,high);
		INICIO MINLOC */
		l = i;
		y = a[i];
		j = i + 1;
		while(j < high)
		{
			if(a[j] < y)
			{
				y = a[j];
				l = j;
			}
			j = j + 1;
		}
		k = l;

		t = a[k];
		a[k] = a[i];
		a[i] = t;
		i = i + 1;
	}
}

void main(void)
{
	int i;
	i = 0;

	while (i < 10){
		vet[i] = input();
		output(vet[i]);
		i = i + 1;
	}


	sort(vet,0,10);
	i = 0;
	while (i < 10)
    {
		output(vet[i]);
		output(vet[i]);
		output(vet[i]);
		output(vet[i]);
		output(vet[i]);
		i = i + 1;
	}
}

			
