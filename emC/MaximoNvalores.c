/*maximo entre n valores*/

void main()
{
	int n;
    int i;
    int maximo;
    int entrada;

    i = 1;
    n = input();
    output(n);
    maximo = input();
    output(maximo);

    while(i != n)
    {
        i = i + 1;
        entrada = input();
        output(entrada);
        if(entrada > maximo)
        {
            maximo = entrada;
        }
    }
    output(maximo);
}
