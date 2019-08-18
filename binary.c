
#include "binary.h"


void print_binary_instruction(AssemblyOperation ao, int result, int op1, int op2, FILE * codefile)
{

		switch(ao)
		{
			case _ADD:
				fprintf(codefile, "6'd0, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _ADDI:
				fprintf(codefile, "6'd1, 5'd%d, 5'd%d, 16'd%d", result, op1, op2);
				break;

			case _SUB:
				fprintf(codefile, "6'd2, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _SUBI:
				fprintf(codefile, "6'd3, 5'd%d, 5'd%d, 16'd%d", result, op1, op2);
				break;

			case _NOT:
				fprintf(codefile, "6'd4, 5'd%d, 21'd0", result);
				break;

			case _SEQ:
				fprintf(codefile, "6'd5, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _SNQ:
				fprintf(codefile, "6'd6, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _SLT:
				fprintf(codefile, "6'd7, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _SLE:
				fprintf(codefile, "6'd8, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _SGT:
				fprintf(codefile, "6'd9, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _SGE:
				fprintf(codefile, "6'd10, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _LW:
				fprintf(codefile, "6'd11, 5'd%d, 16'd%d, 5'd0", result, op1);
				break;

			case _LI:
				fprintf(codefile, "6'd12, 5'd%d, 16'd%d, 5'd0", result, op1);
				break;

			case _LWR:
				fprintf(codefile, "6'd13, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;

			case _SW:
				fprintf(codefile, "6'd14, 5'd%d, 16'd%d, 5'd0", result, op1);
				break;

			case _SWR:
				fprintf(codefile, "6'd15, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;

			case _MOVE:
				fprintf(codefile, "6'd16, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;

			case _BEQ:
				fprintf(codefile, "6'd17, 5'd%d, 5'd%d, 16'd%d", result, op1, op2);
				break;

			case _BNQ:
				fprintf(codefile, "6'd18, 5'd%d, 5'd%d, 16'd%d", result, op1, op2);
				break;

			case _BLT:
				fprintf(codefile, "6'd19, 5'd%d, 5'd%d, 16'd%d", result, op1, op2);
				break;

			case _BGT:
				fprintf(codefile, "6'd20, 5'd%d, 5'd%d, 16'd%d", result, op1, op2);
				break;

			case _JUMP:
				fprintf(codefile, "6'd21, 16'd%d, 10'd0", result);
				break;

			case _JR:
				fprintf(codefile, "6'd22, 5'd%d, 21'd0", result);
				break;

			case _NOP:
				fprintf(codefile, "6'd23, 26'd0");
				break;

			case _HLT:
				fprintf(codefile, "6'd24, 26'd0");
				break;

			case _IN:
				fprintf(codefile, "6'd25, 5'd%d, 21'd0", result);
				break;

			case _OUT:
				fprintf(codefile, "6'd26, 5'd%d, 21'd0", result);
				break;

			case _INSENSOR:
				fprintf(codefile, "6'd27, 5'd%d, 21'd0", result);
				break;

			case _OUTSENSOR:
				fprintf(codefile, "6'd28, 5'd%d, 21'd0", result);
				break;

			// case _DATATOHD:
			// 	fprintf(codefile, "6'd28, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
			// 	break;

			// case _HDTODATA:
			// 	fprintf(codefile, "6'd28, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
			// 	break;

			case _NEXTPROCESS:
				fprintf(codefile, "6'd29, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;

			case _REGTOMEM:
				fprintf(codefile, "6'd30, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;

			case _MEMTOREG:
				fprintf(codefile, "6'd31, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;

			case _HDMI:
				fprintf(codefile, "6'd32, 5'd%d, 5'd%d, 5'd%d, 11'd0", result, op1, op2);
				break;

			case _CUPR:
				fprintf(codefile, "6'd33, 5'd%d, 21'd0", result);
				break;

			case _CHANGECONTEXT:
				fprintf(codefile, "6'd34, 5'd%d, 5'd%d, 16'd0", result, op1);
				break;
		}

}

void print_binary(AssemblyList asn, FILE * codefile, int prog)
{
    fprintf(codefile, "\n");

    int result, op1, op2;

    while(asn != NULL)
    {
        fprintf(codefile, "HD[%d] = {", asn->memlocation + prog*1000);

        if(asn->result != NULL)
        	result = asn->result->op;
        if(asn->op1 != NULL)
        	op1 = asn->op1->op;
        if(asn->op2 != NULL)
        	op2 = asn->op2->op;

        print_binary_instruction(asn->ao, result, op1, op2, codefile);

        fprintf(codefile, "};\n");

        asn = asn->next;
    }
}

void codeGenerationBinary(AssemblyCode CodeAssembly, FILE * codefile, int prog)
{
	AssemblyList asn = CodeAssembly->first;
	if(asn == NULL) return; // ERROR

	print_binary(asn, codefile, prog);

}
