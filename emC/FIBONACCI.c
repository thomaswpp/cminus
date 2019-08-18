int fib(int n)                             
{                                          
  int i;
  int fiba;
  int fibb;
  int soma;   

  i = 0;
  fiba = 0;
  fibb = 1;
  soma = 0; 

  while(i < n)        
  {                                        
    soma = fiba + fibb;                    
    fiba = fibb;                           
    fibb = soma; 
    i = i + 1;                          
  }                                        
  return fiba;                             
}                                          
 
void main(void)
{
  int n;
  n = input();
  output(n);

  output(fib(n));
}
