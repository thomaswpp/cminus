/*media entre n valores*/

void main()
{
	int n;
    int i;
    int soma;
    int media;
    int entrada;

    i = 0;
    soma = 0;
    n = input();
    output(n);
    
    while(i != n)
    {
        i = i + 1;
        entrada = input();
        output(entrada);
        soma = soma + entrada;
    }
    media = soma/n;
    output(media);
}
