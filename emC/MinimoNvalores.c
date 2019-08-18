/*minimo entre n valores*/

void main()
{
	int n;
    int i;
    int minimo;
    int entrada;

    i = 1;
    n = input();
    output(n);
    minimo = input();
    output(minimo);

    while(i != n)
    {
        i = i + 1;
        entrada = input();
        output(entrada);
        if(entrada < minimo)
        {
            minimo = entrada;
        }
    }
    output(minimo);
}
