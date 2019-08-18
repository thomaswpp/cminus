int median( int a[], int low, int high)
{	int i; 
	int k; 
	int t; 
	int i2;
	int x2; int k2; int low2;
  	int aux;

	i = low;
	while (i < high-1){
		low2 = i;

		k2 = low2;
		x2 = a[low2];
		i2 = low2 + 1;

		while(i2 < high){
			aux = a[i2];
			if(aux < x2){
				x2 = a[i2];
				k2 = i2;
			}
			i2 = i2 + 1;
		}
		k = k2;
		t = a[k];
		aux = a[i];
		a[k] = aux;
		a[i] = t;
		i = i + 1;
	}
	
	aux = a[high/2];
	return aux;
}

void main(void)
{
	int vet[ 5 ];
	int i;
	int res;

	i = 0;
	
	while (i < 5){
		vet[i] = input();
		output(vet[i]);
		i = i + 1;
	}
	
	res = median(vet,0,5);
	
	output(res);
}
