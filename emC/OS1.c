
void main(void)
{

	int i;
	int prog;
	int progOld;
	prog = 0;

	regtomem(prog);
	output(prog);

	progOld = prog;
	prog = input();
	output(prog);

	if(prog < 4)
	{

		memtoreg(prog);
		i = 0;
		if(progOld != prog)
		{
			while(i < 48)
			{
				hdmi(prog,prog,i);
				i = i + 1;
			}
		}
		output(79);
		cupr(prog);
	}
	else
	{
		output(99);		
	}
}

