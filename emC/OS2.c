
void main()
{
	int qtd;
	int prog;
	int listaProg[11];
	int i;
	int executar;

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

	while( 1 == 1 )
	{
		prog = 0;
		if(executar == 1) /*nao vai executar na primeira vez*/
		{
			/*não terminou de executar*/
		 	/*salvar Regs  //voltar aqui para salvar os registradores*/
		 	regtohd(prog,0);
			regtohd(prog,1);
			regtohd(prog,2);
			regtohd(prog,3);
			regtohd(prog,4);
			regtohd(prog,5);
			regtohd(prog,6);
			regtohd(prog,7);
			regtohd(prog,8);
			regtohd(prog,9);
			regtohd(prog,10);
			regtohd(prog,11);
			regtohd(prog,12);
			regtohd(prog,13);
			regtohd(prog,14);
			regtohd(prog,15);
			regtohd(prog,16);
			regtohd(prog,17);
			regtohd(prog,18);
			regtohd(prog,19);
			regtohd(prog,20);
			regtohd(prog,21);
			regtohd(prog,22);
			regtohd(prog,23);
			regtohd(prog,24);
			regtohd(prog,25);
			regtohd(prog,26);
			regtohd(prog,27);
			regtohd(prog,28);
			regtohd(prog,29);
			regtohd(prog,30);
			regtohd(prog,31);
			prog = prog + 1;
		}

		executar = 1;

		while(qtd > 0)
		{
			if(listaProg[prog] == 1)
			{
				/* recupera registradores da memoria
				// troca de contexto
				Executa prog %d\n", prog*/
				hdtoreg(prog,0);
				hdtoreg(prog,1);
				hdtoreg(prog,2);
				hdtoreg(prog,3);
				hdtoreg(prog,4);
				hdtoreg(prog,5);
				hdtoreg(prog,6);
				hdtoreg(prog,7);
				hdtoreg(prog,8);
				hdtoreg(prog,9);
				hdtoreg(prog,10);
				hdtoreg(prog,11);
				hdtoreg(prog,12);
				hdtoreg(prog,13);
				hdtoreg(prog,14);
				hdtoreg(prog,15);
				hdtoreg(prog,16);
				hdtoreg(prog,17);
				hdtoreg(prog,18);
				hdtoreg(prog,19);
				hdtoreg(prog,20);
				hdtoreg(prog,21);
				hdtoreg(prog,22);
				hdtoreg(prog,23);
				hdtoreg(prog,24);
				hdtoreg(prog,25);
				hdtoreg(prog,26);
				hdtoreg(prog,27);
				hdtoreg(prog,28);
				hdtoreg(prog,29);
				hdtoreg(prog,30);
				hdtoreg(prog,31);
				i = 0;
				while(i < 140)
				{
					hdmi(prog,prog,i);
					i = i + 1;
				}
				cupr(prog);
				/*Terminou de executar prog, salto para cá*/
				qtd = qtd - 1; /*volta pra ca se terminar de executar*/
				listaProg[prog] = 0;
			}

			prog = prog + 1;
			output(prog);
			if(prog > 10)
			{
				prog = 0;
			}
		}
	}
}
