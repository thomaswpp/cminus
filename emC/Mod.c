/*a mod b*/

void main()
{
    int a;
    int b;

    a = input();
    output(a);
    b = input();
    output(b);
    
    while( a >= b)
    { 
        a = a - b;
    }
    output(a);
}
