
#include "target.h"

#define NR 64

AssemblyCode CodeAssembly;
int addr_reg[NR];
static BucketList functionActual = NULL;
static FuncMem * funcmemory;
Labels * labels;

//vetor armazena os registradores das funções do OS
Operating paramos[3];
int cont_param;

void initialize_register()
{
	int i;
	for(i=0; i < NR; i++)
		addr_reg[i] = 0;
}

static int get_address_register()
{
	int i;

	for(i=0; i < NR; i++)
		if(!addr_reg[i])
			break;

	addr_reg[i] = 1;
	return i + registerSBR;
	// return i;
}

void address_register_discart(Operating op)
{
	if(op == NULL) return;

    if(op->type == regAddress)
	   addr_reg[op->op - registerSBR] = 0;
}

void insert_function_location(const char * name_func)
{
	FuncMem *ant, *aux = funcmemory;
	FuncMem *new_element = (FuncMem*) malloc(sizeof(FuncMem));

	if(new_element == NULL)
		return;


	new_element->name_func = name_func;
	new_element->memloc = CodeAssembly->memlocation;
	new_element->args = NULL;
	new_element->next = NULL;

	if(funcmemory == NULL)
	{
		funcmemory = (FuncMem*) malloc(sizeof(FuncMem));
		funcmemory = new_element;
	} else
	{

		while(aux != NULL)
		{
			ant = aux;
			aux = aux->next;
		}
		ant->next = new_element;
	}
}

static FuncMem * search_function_location( const char * name_func)
{
	FuncMem *aux = funcmemory;

	while(aux != NULL)
	{
		fprintf(stderr, "ddd%s\n", name_func);
		if( !strcmp(name_func, aux->name_func)){
			fprintf(stderr, "fdsdg%s\n", name_func);
			return aux;
		}


		aux = aux->next;
	}

	return NULL;
}

void insert_params_function(const char * name_func, BucketList l)
{
	Arguments * new_arg;
	FuncMem *aux = search_function_location(name_func);

	if(aux == NULL) return;

	new_arg = (Arguments*) malloc(sizeof(Arguments));
	new_arg->name = l->name;
	new_arg->memloc = l->memloc;
	new_arg->next = NULL;

	Arguments *ant, *args = aux->args;
	if(args == NULL)
		aux->args = new_arg;
	else
	{
		while(args != NULL)
		{
			ant = args;
			args = args->next;
		}
		ant->next = new_arg;
	}
}

void insert_labels(int label, Operating op)
{
	Labels * ant, * aux = labels;
	Labels * new_label = (Labels*) malloc(sizeof(Labels));

	if(new_label == NULL)
	{
			Error = TRUE;
			fprintf(stderr, "Erro ao inserir labels\n");
			return;
	}

	new_label->label = label;
	new_label->op = op;
	new_label->next = NULL;

	if(labels == NULL)
	{
		labels = (Labels*) malloc(sizeof(Labels));
		labels = new_label;
	} else
	{
		while( aux != NULL )
		{
			ant = aux;
			aux = aux->next;
		}
		ant->next = new_label;
	}
}

int update_jump_label(int label, int memloc)
{
	Labels * aux = labels;
	while(aux != NULL)
	{
		if(aux->label == label)
		{
			aux->op->op = memloc;
			return 1;
		}
		aux = aux->next;
	}


	//não achou, possivelmente while
	if(aux == NULL)
	{
		Operating op = create_element_Operating(address, memloc);
		insert_labels(label, op);
	}
	return 0;
}

int label_exist(int label)
{
	Labels * aux = labels;
	while(aux != NULL)
	{
		if(aux->label == label)
		{
			return aux->op->op;
		}
		aux = aux->next;
	}
	return 0;
}

void create_assembly_list()
{
    CodeAssembly = (AssemblyCode) malloc(sizeof(struct AssemblyCode));
    if(CodeAssembly != NULL)
    {
        CodeAssembly->first = CodeAssembly->last = NULL;
        CodeAssembly->memlocation = 1;
    }
}


Operating create_element_Operating(OperatingType type, int op)
{
    Operating new_element = (Operating) malloc(sizeof(struct OperatingT));
    new_element->type = type;
    new_element->op = op;
    return new_element;
}

AssemblyList create_element_objeto(AssemblyOperation ao, Operating result, Operating op1, Operating op2)
{
    AssemblyList new_element = (AssemblyList) malloc(sizeof(struct AssemblyList));
    new_element->ao = ao;
    new_element->memlocation = CodeAssembly->memlocation;
    new_element->result = result;
    new_element->op1 = op1;
    new_element->op2 = op2;
    new_element->next = NULL;
    return new_element;
}

int insert_instruction_objeto(AssemblyOperation ao, Operating result, Operating op1, Operating op2, insert_mode mode)
{
    AssemblyList new_element = create_element_objeto(ao, result, op1, op2);
    if(new_element == NULL || CodeAssembly == NULL)
    {
    	Error = TRUE;
    	fprintf(stderr, "Erro ao inserir instrução\n");
    	return 1; // ERROR
    }

    if(CodeAssembly->first == NULL)
	{
		if(mode == LAST)
			CodeAssembly->memlocation++;
		else
			new_element->memlocation = 0;

        CodeAssembly->first = CodeAssembly->last = new_element;
	}
    else if(mode == LAST)
    {
        CodeAssembly->last->next = new_element;
        CodeAssembly->last = new_element;
		CodeAssembly->memlocation++;
    }
    else if(mode == FIRST)
    {
		new_element->memlocation = 0;
        new_element->next = CodeAssembly->first;
        CodeAssembly->first = new_element;
    }

    return 0;
}


Operating create_instruction_three(AssemblyOperation ao, OperatingType type1, int value1, OperatingType type2, int value2, OperatingType type3, int value3){
    Operating op1, op2, op3;
    op1 = create_element_Operating(type1, value1);
    op2 = create_element_Operating(type2, value2);
    op2 = create_element_Operating(type3, value3);
    insert_instruction_objeto( ao, op1, op2, op3, LAST );
    return op1;
}

Operating create_instruction_two(AssemblyOperation ao, OperatingType type1, int value1, OperatingType type2, int value2, Operating op3)
{
    Operating op1, op2;
    op1 = create_element_Operating(type1, value1);
    op2 = create_element_Operating(type2, value2);
    insert_instruction_objeto( ao, op1, op2, op3, LAST );
    return op1;
}

Operating create_instruction_one(AssemblyOperation ao, OperatingType type, int value, Operating op2, Operating op3, insert_mode mode)
{
    Operating op1;
    op1 = create_element_Operating(type, value);
    insert_instruction_objeto( ao, op1, op2, op3, mode );
    return op1;
}

Operating BEQ(AssemblyOperation ao, Operating op, OperatingType type, int value, OperatingType type2, int value2)
{
	Operating op1, op2;
	op1 = create_element_Operating(type, value);
	op2 = create_element_Operating(type2, value2);
	insert_instruction_objeto( ao, op, op1, op2, LAST );
    return op2;
}

Operating load(AssemblyOperation ao, OperatingType type, int value)
{
        return create_instruction_two( ao, regAddress, get_address_register(), type, value, NULL);
}

void store(AssemblyOperation ao, Operating op1, Operating op2, OperatingType type, int value)
{
        if (op2 == NULL)
            op2 = create_element_Operating(type, value);

        insert_instruction_objeto( ao, op1, op2, NULL, LAST );

        address_register_discart(op1);
}

Operating mult_or_div(AssemblyOperation ao, Operating result, Operating op1, Operating op2)
{
        Operating zero, opAux, regAux, OpReturn;

        /*cria registrador auxiliar*/
        regAux = load( _LI, immediate, 0 );

		//BEQ( _BEQ, op1, regZero, registerZero, address, CodeAssembly->memlocation+4);

        //reg = 1
        opAux = create_element_Operating(immediate, 1);
        if(ao == _MULT)
        {

			BEQ( _BEQ, op1, regZero, registerZero, address, CodeAssembly->memlocation+4);

            insert_instruction_objeto( _ADD, regAux, regAux, op2, LAST);

            insert_instruction_objeto( _SUBI, op1, op1, opAux, LAST);

        } else if(ao == _DIV)
        {

			BEQ( _BLT, op1, regZero, registerZero, address, CodeAssembly->memlocation+4);

            insert_instruction_objeto( _SUB, op1, op1, op2, LAST);

            insert_instruction_objeto( _ADDI, regAux, regAux, opAux, LAST);
        }

        create_instruction_one( _JUMP, address, CodeAssembly->memlocation-3, NULL, NULL, LAST);

        OpReturn = create_instruction_one( _MOVE, regAddress, get_address_register(), regAux, NULL, LAST);

        address_register_discart(regAux);

        return OpReturn;

}


static Operating instrucoes_aritimeticas(Operand *op)
{

    OperatingType opt;
    Operating op1;
    int reg;

    switch(op->kind)
    {
        case String:

            // opt = address;
            op1 = load( _LW, address, op->contents.variable.bucket->memloc );

            break;
        case Temp:

            opt = regTemp;
            reg = op->contents.val;
			// reg = get_address_register();
            op1 = create_element_Operating(opt, reg);

            break;
        case IntConst:

            // opt = immediate;
            op1 = load( _LI, immediate, op->contents.val );

            break;
    }

    return op1;
}

Operating geraCodigoInstrucaoTipoR(Quadruple qd, AssemblyOperation Ao)
{
    Operating op1 = instrucoes_aritimeticas(qd->op1);
    Operating op2 = instrucoes_aritimeticas(qd->op2);
    Operating result = instrucoes_aritimeticas(qd->op3);
    Operating Opmult_div;

    switch(Ao)
    {
        case _MULT:

			Opmult_div = mult_or_div(Ao, result, op1, op2);

            break;

        case _DIV:

            Opmult_div = mult_or_div(Ao, result, op1, op2);

            break;

        default: //_ADD or _SUB etc

            insert_instruction_objeto(Ao, result, op1, op2, LAST);

            break;
    }

    address_register_discart(op1);
    address_register_discart(op2);
	return result;
}

void assign(Operand *op1, Operand *result)
{
    Operating regAux;

    switch(op1->kind)
    {
        case Temp:

            regAux = CodeAssembly->last->result; //register temp

            store( _SW, regAux, NULL, address, result->contents.variable.bucket->memloc);

            break;

        case String:

            regAux = load( _LW, address, op1->contents.variable.bucket->memloc);

            store( _SW, regAux, NULL, address, result->contents.variable.bucket->memloc);

            break;

		case IntConst:

			regAux = load( _LI, immediate, op1->contents.val);

			store( _SW, regAux, NULL, address, result->contents.variable.bucket->memloc);

			break;

        default: break;
    }

	address_register_discart(regAux);
}

void assign_vector(Operand *opv, Operand *op2, Operand *op, int position, int vector_reference)
{
    Operating regV, regAux = NULL;


    if(vector_reference == 1)
    	regV = load( _LI, address, opv->contents.variable.bucket->memloc);
    else // vector_reference == 2
    	regV = load( _LW, address, opv->contents.variable.bucket->memloc);

    //create add
    switch(op2->kind)
    {
        case IntConst:

            insert_instruction_objeto( _ADDI, regV, regV, create_element_Operating(immediate, op2->contents.val), LAST);

            break;

        case String:

            regAux = load( _LW, address, op2->contents.variable.bucket->memloc);
            insert_instruction_objeto( _ADD, regV, regV, regAux, LAST);

            break;

		case Temp:

			insert_instruction_objeto( _ADD, regV, regV, create_element_Operating(regTemp, op2->contents.val), LAST);

			break;

        default: break;
    }


    switch (op->kind)
    {

        case IntConst:

            regAux = load( _LI, immediate, op->contents.val);

			store( _SWR, regAux, regV, 0, 0);

            break;

        case String:

			if(position) //left side vector
			{
				regAux = create_instruction_one( _LWR, regAddress, get_address_register(), regV, NULL, LAST);
				store( _SW, regAux, NULL, address, op->contents.variable.bucket->memloc);
			}
			else
			{
				regAux = load( _LW, address, op->contents.variable.bucket->memloc);
				store( _SWR, regAux, regV, 0, 0);
			}

            break;

		case Temp:

			regAux = create_element_Operating(regTemp, op->contents.val);
			if(position) //left side vector
				insert_instruction_objeto( _LWR, regAux, regV, NULL, LAST);
			else
				store( _SWR, regAux, regV, 0, 0);

			break;

        default: break;
    }

	address_register_discart(regV);
	address_register_discart(regAux);
}


void geraCodigoInstrucaoAtribuicao(Quadruple qd, AssemblyOperation Ao)
{
	Operand *opv, *op;
	BucketList l;
	/*0 - right side vector, 1 - left side vector*/
	int position, vector_reference = 0;

	if(qd->op2 != NULL) //is vector
	{
		/*Vector left side*/
		if(qd->op1->kind == String)
		{
			l = qd->op1->contents.variable.bucket;

			if(l->is_vector)
			{
				opv = qd->op1;
				op = qd->op3;
				position = 1;
				vector_reference = l->is_vector;
			}
		}
		/*Vector right side*/
		if(qd->op3->kind == String)
		{
			l = qd->op3->contents.variable.bucket;
			if(l->is_vector)
			{
				opv = qd->op3;
				op = qd->op1;
				position = 0;
				vector_reference = l->is_vector;
			}
		}

		assign_vector(opv, qd->op2, op, position, vector_reference);

	} else
		assign(qd->op1, qd->op3);
}


void geraCodigoParametrosFuncao(Arguments *args, int order_params, Quadruple qd)
{
	Operating regAux, regAux2;
	Operand *op1, *op2;
	BucketList l;
    int i;

	if (args == NULL) return;

	op1 = qd->op1; op2 = qd->op2;
	switch (op1->kind)
	{
		case IntConst:
			regAux = load( _LI, immediate, op1->contents.val);
			break;

		case String:
			l = op1->contents.variable.bucket;

			if(l->is_vector)
			{
				if(l->is_vector == 1)
					regAux = load( _LI, address, l->memloc);
				else //l->is_vector == 2
					regAux = load( _LW, address, l->memloc);


				if(op2 != NULL)
				{
					switch (op2->kind)
					{

						case IntConst:
							insert_instruction_objeto( _ADDI, regAux, regAux, create_element_Operating(immediate, op2->contents.val), LAST);
							insert_instruction_objeto( _LWR, regAux, regAux, NULL, LAST);

							break;
						case String:
							regAux2 = load( _LW, address, op2->contents.variable.bucket->memloc);
							insert_instruction_objeto( _ADD, regAux, regAux, regAux2, LAST);
							insert_instruction_objeto( _LWR, regAux, regAux, NULL, LAST);

							break;
						default: break;
					}
				}
			} else
				regAux = load( _LW, address, l->memloc);

			break;
		case Temp:
			regAux = load( _LW, regTemp, op1->contents.val);
			break;
	}

	for(i = 0; i < order_params; i++)
		args = args->next;

	store( _SW, regAux, NULL, address, args->memloc);

	address_register_discart(regAux);

}

void geraCodigoChamadaFuncao(Quadruple qd, AssemblyOperation Ao)
{
	FuncMem * funcmem;
	Operand *op1;
	Operating regAux;
	op1 = qd->op1;

	funcmem = search_function_location(op1->contents.variable.name);

	if(funcmem != NULL)
	{
		/*salva posíção de retorno */
		regAux = load( _LI, address, CodeAssembly->memlocation+3);
		store( _SW, regAux, NULL, address, op1->contents.variable.bucket->memloc);
		/*cria o jump para inicio da função chamada*/
		create_instruction_one( _JUMP, address, funcmem->memloc, NULL, NULL, LAST);

		address_register_discart(regAux);
	}
}

Operating geraCodigoIf(Quadruple qd, AssemblyOperation Ao, Operating op)
{
	// Operating op;
	// op = create_element_Operating(regTemp, qd->op1->contents.val);
	/*Salto será atualizado depois*/
	return BEQ( Ao, op, regZero, registerZero, address, 0);
}

void geraCodigoReturn(Quadruple qd, AssemblyOperation Ao)
{
    Operating regAux;
    Operand *op = qd->op3;

	if(op != NULL)
	{
	    switch (op->kind)
	    {
	        case IntConst:

	            regAux = create_instruction_two( _LI, regAddress, registerRV, immediate, op->contents.val, NULL);

	            break;

	        case Temp:

	            regAux = CodeAssembly->last->result;
	            create_instruction_one( _MOVE, regAddress, registerRV, regAux, NULL, LAST);

	            break;

	        case String:

	            create_instruction_two( _LW, regAddress, registerRV, address, op->contents.variable.bucket->memloc, NULL);

	            break;

	        default: break;
	    }
	}

    regAux = create_instruction_two( _LW, regAddress, registerAR, address, functionActual->memloc, NULL);
    insert_instruction_objeto( _JR, regAux, NULL, NULL, LAST);
	return;
}

void geraCodigoParametrosSaida(Quadruple qd)
{
	Operating pos, regSaida;
    Operand * op1, * op2;
    BucketList l;

    op1 = qd->op1; op2 = qd->op2;

    if(op1 == NULL) return; //ERROR

    l = op1->contents.variable.bucket;

    if(l != NULL && l->is_vector)
    {

    	if(l->is_vector == 1)
        	regSaida = create_instruction_two( _LI, regAddress, registerINOUT, address, l->memloc, NULL);
    	else //l->is_vector == 2
        	regSaida = create_instruction_two( _LW, regAddress, registerINOUT, address, l->memloc, NULL);

        /*se a passagem de parametro de saída for um vetor sem a sua posição,
        imprima o valor da primeira posição*/
        if(op2 == NULL)
            pos =  create_element_Operating(immediate, 0);
        else
		{
			switch (op2->kind)
			{
				case IntConst:
					pos =  create_element_Operating(immediate, op2->contents.val);
					insert_instruction_objeto( _ADDI, regSaida, regSaida, pos, LAST);
					break;
				case String:
					pos = load( _LW, address, op2->contents.variable.bucket->memloc);
					insert_instruction_objeto( _ADD, regSaida, regSaida, pos, LAST);
					break;
				default: break;
			}
		}

        insert_instruction_objeto( _LWR, regSaida, regSaida, NULL, LAST);
    }
    else
    {
		switch(op1->kind)
		{
			case IntConst:
				create_instruction_two( _LI, regAddress, registerINOUT, immediate, op1->contents.val, NULL);
				break;
			case String:
				create_instruction_two( _LW, regAddress, registerINOUT, address, op1->contents.variable.bucket->memloc, NULL);
				break;
			case Temp:
				create_instruction_two( _MOVE, regAddress, registerINOUT, regTemp, op1->contents.val, NULL);
				break;
			default: break;
		}
    }
}


void geraCodigoParametrosFuncaoOS(Quadruple qd)
{
	Operand * op1, * op2;
	Operating pos, regSaida;
	BucketList l;

	op1 = qd->op1; op2 = qd->op2;

    if(op1 == NULL) return; //ERROR

    l = op1->contents.variable.bucket;

    if(l != NULL && l->is_vector)
    {

    	if(l->is_vector == 1)
        	regSaida =load( _LI, address, l->memloc);
    	else //l->is_vector == 2
        	regSaida = load( _LW, address, l->memloc);

        /*se a passagem de parametro de saída for um vetor sem a sua posição,
        imprima o valor da primeira posição*/
        if(op2 == NULL)
            pos =  create_element_Operating(immediate, 0);
        else
		{
			switch (op2->kind)
			{
				case IntConst:
					pos =  create_element_Operating(immediate, op2->contents.val);
					insert_instruction_objeto( _ADDI, regSaida, regSaida, pos, LAST);
					break;
				case String:
					pos = load( _LW, address, op2->contents.variable.bucket->memloc);
					insert_instruction_objeto( _ADD, regSaida, regSaida, pos, LAST);
					break;
				default: break;
			}
		}

        insert_instruction_objeto( _LWR, regSaida, regSaida, NULL, LAST);
    }
    else
    {
		switch(op1->kind)
		{
			case IntConst:
	            regSaida = load( _LI, immediate, op1->contents.val);
				break;
			case String:
				regSaida = load( _LW, address, op1->contents.variable.bucket->memloc );
				break;
			case Temp:
				//precisa mudar
				regSaida = CodeAssembly->last->result;
				create_instruction_one( _MOVE, regAddress, registerRV, regSaida, NULL, LAST);
				break;
			default: break;
		}
	}
	//salvar os registradores, para depois ser usado para criar as instruções
	if (cont_param >= 3)
		cont_param = 0;

	paramos[cont_param++] = regSaida;
	// return regSaida;

}

// void changeContext(Operating prog, AssemblyOperation Ao)
// {
//
// 	int i;
// 	Operating addrReg;
//
// 	for(i = 14; i < 32; i++){
// 		addrReg = create_element_Operating(immediate, i);
// 		insert_instruction_objeto( Ao, prog, addrReg, NULL, LAST );
// 	}
//
//
// }


AssemblyCode codeGenerationAssembly(AddressQuad code, FILE * codefile, int is_os)
{
	FuncMem * funcmem = NULL;
	int order_params = 0, lab, memloc;
	Operating op;
	Operating prog, addrReg;

	// IS_OS = is_os;
	//pule call main
    Quadruple aux, qd = code->first->next;

    create_assembly_list();
    initialize_register();

    while(qd != NULL)
    {
        switch(qd->instruction)
        {
            /* Aritimeticas */
            case ADD:
                geraCodigoInstrucaoTipoR(qd, _ADD);
                break;

            case SUB:
                geraCodigoInstrucaoTipoR(qd, _SUB);
                break;

            case MULT:
                geraCodigoInstrucaoTipoR(qd, _MULT);
                break;

            case DIV:
                geraCodigoInstrucaoTipoR(qd, _DIV);
                break;

            /* Lógicas */
            case EQ:
                 op = geraCodigoInstrucaoTipoR(qd, _SEQ);
                break;

            case NEQ:
                op = geraCodigoInstrucaoTipoR(qd, _SNQ);
                break;

            case LTH:
                op = geraCodigoInstrucaoTipoR(qd, _SLT);
                break;

            case LETH:
                 op = geraCodigoInstrucaoTipoR(qd, _SLE);
                break;

            case GTH:
                 op = geraCodigoInstrucaoTipoR(qd, _SGT);
                break;

            case GETH:
                 op = geraCodigoInstrucaoTipoR(qd, _SGE);
                break;

            case ASN:
                geraCodigoInstrucaoAtribuicao(qd, _SW);
                break;

            case FUNC:
                functionActual = qd->op1->contents.variable.bucket;

				insert_function_location(functionActual->name);

				if(!strcmp(functionActual->name, "main"))
					create_instruction_one( _JUMP, address, CodeAssembly->memlocation, NULL, NULL, FIRST);

                break;

            case RTN:
            	if(strcmp(functionActual->name, "main"))
                	geraCodigoReturn(qd, _JR);
                break;

            case PARAM:

				aux = qd;
				/*encontrar a função do parametro*/
				if( funcmem == NULL)
				{

					while(aux != NULL)
					{
						if(aux->instruction == CALL)
							break;
						aux = aux->next;
					}


					if(aux != NULL)
						funcmem = search_function_location(aux->op1->contents.variable.name);
				}

				if(funcmem != NULL)
					geraCodigoParametrosFuncao(funcmem->args, order_params, qd);
				order_params++;
                break;

            case CALL:
				geraCodigoChamadaFuncao(qd, _JUMP);

				Operating reg;
				if(!strcmp(qd->op1->contents.variable.bucket->typedata, "int"))
				{
					reg = create_element_Operating(regAddress, registerRV);

					create_instruction_one( _MOVE, regTemp, qd->op3->contents.val, reg, NULL, LAST);
				}

				order_params = 0;
				funcmem = NULL;

                break;

            case ARGS:
				insert_params_function(functionActual->name, qd->op1->contents.variable.bucket);
                break;

            case JPF:
				op = geraCodigoIf(qd, _BEQ, op);
				lab = qd->op3->contents.val;
				insert_labels(lab, op);

                break;

            case GOTO:

				lab = qd->op3->contents.val;
				memloc = label_exist(lab);
				if(memloc)
				{
					/*Instrução While*/
					op = create_instruction_one( _JUMP, address, memloc, NULL, NULL, LAST);
				}
				else
				{
					/*Instrução ELSE,
					O salto será atualizado depois*/
					op = create_instruction_one( _JUMP, address, 0, NULL, NULL, LAST);
					insert_labels(lab, op);
				}

                break;

            case LBL:

				lab = qd->op3->contents.val;
				int mem = 1;
				if(CodeAssembly->last != NULL)
					mem = CodeAssembly->last->memlocation+1;
				update_jump_label(lab, mem);

                break;

            case HALT:
                insert_instruction_objeto( _HLT, NULL, NULL, NULL, LAST);
                break;

			case IN:
				create_instruction_one( _IN, regAddress, registerINOUT, NULL, NULL, LAST);
				break;

			// case INSENSOR:
			// 	create_instruction_one( _INSENSOR, regAddress, registerINOUT, NULL, NULL, LAST);
			// 	break;

			case OUTPARAM:
				geraCodigoParametrosSaida( qd );
				break;

			case OSPARAM:
				geraCodigoParametrosFuncaoOS( qd );
				break;

			case PARAMCONTEXT:
				if (qd->op1->kind == String)
					geraCodigoParametrosFuncaoOS( qd );
				else
					addrReg = create_element_Operating(immediate, qd->op1->contents.val);
				break;

			case OUT:
				create_instruction_one( _OUT, regAddress, registerINOUT, NULL, NULL, LAST);
				break;

			// case OUTSENSOR:
			// 	create_instruction_one( _OUTSENSOR, regAddress, registerINOUT, NULL, NULL, LAST);
			// 	break;
            //
			// case DATATOHD:
			// 	cont_param = 0;
			// 	insert_instruction_objeto( _DATATOHD, paramos[0], paramos[1], paramos[2], LAST );
			// 	break;
            //
			// case HDTODATA:
			// 	cont_param = 0;
			// 	insert_instruction_objeto( _HDTODATA, paramos[0], paramos[1], paramos[2], LAST );
			// 	break;

			case NEXTPROCESS:
				cont_param = 0;
				insert_instruction_objeto( _NEXTPROCESS, paramos[0], paramos[1], NULL, LAST );
				break;

			case CHANGECONTEXT:
				cont_param = 0;
				// changeContext(prog);
				// address_register_discart(prog);
				break;

			case REGTOMEM:
				cont_param = 0;
				insert_instruction_objeto( _REGTOMEM, paramos[0], addrReg, NULL, LAST );
				address_register_discart(paramos[0]);
				address_register_discart(addrReg);
				address_register_discart(prog);
				break;

			case MEMTOREG:
				cont_param = 0;
				cont_param = 0;
				insert_instruction_objeto( _MEMTOREG, paramos[0], addrReg, NULL, LAST );
				address_register_discart(paramos[0]);
				address_register_discart(addrReg);
				address_register_discart(prog);

				break;

			case HDMI:
				cont_param = 0;
				insert_instruction_objeto( _HDMI, paramos[0], paramos[1], paramos[2], LAST );
				address_register_discart(paramos[0]);
				address_register_discart(paramos[1]);
				address_register_discart(paramos[2]);
				break;

			case CUPR:
				cont_param = 0;
				insert_instruction_objeto( _CUPR, paramos[0], NULL, NULL, LAST );
				address_register_discart(paramos[0]);
				address_register_discart(paramos[1]);
				address_register_discart(paramos[2]);
				break;

            case VEC: break;
        }


        qd = qd->next;
    }

    print_assembly();

    return CodeAssembly;
}

void print_element_assembly(Operating op)
{
    switch (op->type) {
        case regTemp:
            fprintf(listing, " $t%d", op->op);
            break;
        case regAddress:
            fprintf(listing, " $s%d", op->op);
            break;
        case immediate:
            fprintf(listing, " %d", op->op);
            break;
        case address:
            fprintf(listing, " %d", op->op);
            break;
        case regZero:
            fprintf(listing, " $zero");
            break;
        default: break;
    }
}

void print_assembly_instruction(AssemblyOperation Ao)
{
    switch (Ao) {
        case _ADD:
            fprintf(listing, "add");
            break;
        case _ADDI:
            fprintf(listing, "addi");
            break;
        case _SUB:
            fprintf(listing, "sub");
            break;
        case _SUBI:
            fprintf(listing, "subi");
            break;
        case _MULT:
            fprintf(listing, "mult");
            break;
        case _DIV:
            fprintf(listing, "div");
            break;
        case _LW:
            fprintf(listing, "lw");
            break;
            break;
        case _LWR:
            fprintf(listing, "lwr");
            break;
        case _LI:
            fprintf(listing, "li");
            break;
        case _SW:
            fprintf(listing, "sw");
            break;
        case _SWR:
            fprintf(listing, "swr");
            break;
        case _NOT:
            fprintf(listing, "not");
            break;
        case _SLT:
            fprintf(listing, "slt");
            break;
        case _SLE:
            fprintf(listing, "sle");
            break;
        case _SGT:
            fprintf(listing, "sgt");
            break;
        case _SGE:
            fprintf(listing, "sge");
            break;
        case _BEQ:
            fprintf(listing, "beq");
            break;
        case _BNQ:
            fprintf(listing, "bnq");
            break;
        case _BLT:
            fprintf(listing, "blt");
            break;
        case _BGT:
            fprintf(listing, "bgt");
            break;
        case _JUMP:
            fprintf(listing, "jump");
            break;
        case _JR:
            fprintf(listing, "jr");
            break;
        case _SEQ:
            fprintf(listing, "seq");
            break;
        case _SNQ:
            fprintf(listing, "snq");
            break;
        case _MOVE:
            fprintf(listing, "move");
            break;
        case _NOP:
            fprintf(listing, "nop");
            break;
        case _HLT:
            fprintf(listing, "hlt");
            break;
        case _IN:
            fprintf(listing, "in");
            break;
        // case _INSENSOR:
        //     fprintf(listing, "insensor");
        //     break;
        case _OUT:
            fprintf(listing, "out");
            break;
        // case _OUTSENSOR:
        //     fprintf(listing, "outsensor");
        //     break;
       // case _DATATOHD:
       //      fprintf(listing, "datatohd");
       //      break;
	   // case _HDTODATA:
       //      fprintf(listing, "hdtodata");
       //      break;
       case _NEXTPROCESS:
            fprintf(listing, "nextprocess");
            break;
       case _REGTOMEM:
            fprintf(listing, "regtomem");
            break;
       case _MEMTOREG:
            fprintf(listing, "memtoreg");
            break;
       case _HDMI:
            fprintf(listing, "hdmi");
            break;
       case _CUPR:
            fprintf(listing, "cupr");
            break;
       case _CHANGECONTEXT:
            fprintf(listing, "changecontext");
            break;

        default: break;
    }
}

void print_assembly()
{
    fprintf(listing, "\n");

    AssemblyList asn = CodeAssembly->first;
    int count = 0;
    while(asn != NULL)
    {
        fprintf(listing, "%d -> ", count );

        print_assembly_instruction(asn->ao);

        if(asn->result != NULL)
            print_element_assembly(asn->result);
        if(asn->op1 != NULL)
            print_element_assembly(asn->op1);
        if(asn->op2 != NULL)
            print_element_assembly(asn->op2);

        fprintf(listing, "\n");
        asn = asn->next;
        count++;
    }
}
