
void main() 
{
	int qtd;
	int prog;
	int listaProg[11];
	int i;
	int executar;

	while( 1 == 1)  /*bugou*/
	{		
		executar = 0;
		prog = 0;
		qtd = 0;
		i = 0;
		while(i < 11)
		{
			listaProg[i] = 0;
			i = i + 1;
		}

		/*Quantos programas quer executar?*/
		qtd = input();

		i = 0;
		while(i < qtd) /*bugou !! 31 a 46 em assembly */
		{
			/*Escolha um programa*/
			prog = input();

			listaProg[prog] = 1;
			i = i + 1;		
		}


		prog = 0;
		
		if(executar == 1) /*nao vai executar na primeira vez*/
		{		 
		 	/*salvar Regs  //voltar aqui para salvar os registradores*/
		 	output(prog);
		 	/*regtomem(prog);*/
			prog = prog + 1;
		}

		executar = 1;

		while(qtd > 0)
		{
			if(listaProg[prog] == 1) 
			{
				/* recupera registradores da memoria
				// troca de contexto
				Executa prog %d\n", prog
				memtoreg(prog);*/
				i = 0;
				while(i < 48)
				{
					hdmi(prog,prog,i);
					i = i + 1;
				}
				cupr(prog);	
				/*Terminou de executar prog*/
				output(prog);
				qtd = qtd - 1; /*volta pra ca se terminar de executar*/
				listaProg[prog] = 0;
			}
			prog = prog + 1;
			if(prog >= 11)
			{
				prog = 0;
			}
		}
	}
}  


