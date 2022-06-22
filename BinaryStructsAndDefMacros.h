#ifndef GENERALS_h
#define GENERALS_h

#define MACRO_DIRECTIVE         ".define"
#define DATA_DIRECTIVE          ".data"
#define STRING_DIRECTIVE       	".string"
#define EXTERN_DIRECTIVE	".extern"
#define ENTRY_DIRECTIVE   	".entry"
#define	TRUE			1
#define	FALSE 			0
#define	MAX_LEN			81
#define LABEL_MAX_LEN           31
#define	EXTENSTION_ASM		".as"


/* For instruction first line */
typedef struct Instruction
{
	unsigned int ARE:2;
	unsigned int DES:2;
	unsigned int SOURSE:2;
	unsigned int OpCode:4;
	unsigned int free:4;
}Instruction;

/* For 2 registers in the same line */
typedef struct RegisterCode
{
	unsigned int   ARE:2;
	unsigned int   rightCode:3;
	unsigned int   leftCode:3;
	unsigned int   free:6 ;
}RegisterCode;

/* For address\constant */
typedef struct AddressSeg
{
	unsigned int         ARE:2;
	unsigned int   nCodeAdd:12;
}AddressSeg;

typedef struct FinalCode
{
    unsigned int code:14;
}FinalCode;

typedef union codeSegment
{
	Instruction csInst;
	AddressSeg csAddress;
	RegisterCode rgCode;
	FinalCode final;
}codeSegment;

#endif

