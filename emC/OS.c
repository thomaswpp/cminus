
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
		 	regtomem(prog,0);
			regtomem(prog,1);
			regtomem(prog,2);
			regtomem(prog,3);
			regtomem(prog,4);
			regtomem(prog,5);
			regtomem(prog,6);
			regtomem(prog,7);
			regtomem(prog,8);
			regtomem(prog,9);
			regtomem(prog,10);
			regtomem(prog,11);
			regtomem(prog,12);
			regtomem(prog,13);
			regtomem(prog,14);
			regtomem(prog,15);
			regtomem(prog,16);
			regtomem(prog,17);
			regtomem(prog,18);
			regtomem(prog,19);
			regtomem(prog,20);
			regtomem(prog,21);
			regtomem(prog,22);
			regtomem(prog,23);
			regtomem(prog,24);
			regtomem(prog,25);
			regtomem(prog,26);
			regtomem(prog,27);
			regtomem(prog,28);
			regtomem(prog,29);
			regtomem(prog,30);
			regtomem(prog,31);
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
				memtoreg(prog,0);
				memtoreg(prog,1);
				memtoreg(prog,2);
				memtoreg(prog,3);
				memtoreg(prog,4);
				memtoreg(prog,5);
				memtoreg(prog,6);
				memtoreg(prog,7);
				memtoreg(prog,8);
				memtoreg(prog,9);
				memtoreg(prog,10);
				memtoreg(prog,11);
				memtoreg(prog,12);
				memtoreg(prog,13);
				memtoreg(prog,14);
				memtoreg(prog,15);
				memtoreg(prog,16);
				memtoreg(prog,17);
				memtoreg(prog,18);
				memtoreg(prog,19);
				memtoreg(prog,20);
				memtoreg(prog,21);
				memtoreg(prog,22);
				memtoreg(prog,23);
				memtoreg(prog,24);
				memtoreg(prog,25);
				memtoreg(prog,26);
				memtoreg(prog,27);
				memtoreg(prog,28);
				memtoreg(prog,29);
				memtoreg(prog,30);
				memtoreg(prog,31);
				i = 0;
				while(i < 150)
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
