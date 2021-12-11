/* ----- Made by Yang Ill Doung and GoldFish Crop Brain ------------------- */
#include <dos.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

/* ---- fuction define ---------------------------------------------------- */
int     f_a_code(char *tok[],char *bp,int toksize);
int     f_c_code(char *tok[],char *bp,int toksize,int offset);
int     f_d_code(char *tok[],char *bp,int toksize,char *strP,char *b1);
int     f_e_code(char *tok[],char *bp,int toksize);
int     f_h_code(char *tok[],char *bp,int toksize);
int     f_i_code(char *tok[],char *bp,int toksize);
int     f_j_code(char *tok[],char *bp,int toksize,int offset);
int     f_l_code(char *tok[],char *bp,int toksize,int offset);
int     f_m_code(char *tok[],char *bp,int toksize);
int     f_n_code(char *tok[],char *bp,int toksize);
int     f_o_code(char *tok[],char *bp,int toksize);
int     f_p_code(char *tok[],char *bp,int toksize);
int     f_r_code(char *tok[],char *bp,int toksize);
int     f_s_code(char *tok[],char *bp,int toksize);
int     f_t_code(char *tok[],char *bp,int toksize);
int     f_w_code(char *tok[],char *bp,int toksize);
int     f_x_code(char *tok[],char *bp,int toksize);

int	f_op(char *bp,int toksize,int writebyte);
int	f_data(char *strP,char *bp,char *b1,int datatype);
int	f_short_label(char *strP,char *bp,int offset);
int	f_label_proc(char *tok[],char *bp,int toksize,int offset,int S);
int	f_load(char *tok[],char *bp,int toksize,unsigned char c1);
int	f_1oper(char *tok[],char *bp,int toksize,unsigned char c1,
		unsigned char c2,unsigned char c3,unsigned char c4,
		unsigned char c5);
int	f_2oper(char *tok[],char *bp,int toksize,int S,unsigned char c1,
		unsigned char c6);

int	f_reg(char *tok[],char *bp,int toksize,int opertype,int tokcounter);
int	f_mem(char *tok[],char *bp,int toksize,int opertype,int tokcounter);

int	f_strcmp(char *strP,char *ta1_code[]);
int	f_reg_cmp(char *strP,int regmode);
int	f_char_find(char *strP,char cc);
int	f_str2hexa(char *strP,char *bp);

/* ---- GoldFish Tiny Assembler test version ------------------------------ */
char    GF1[]="Unregistered GoldFish friend 1994 All right unreserved\n"
	      "Tiny  Assembler 1 test version by your friend GoldFish\n\n"
	      "-- CATION --------------------------------------------\n"
	      "This program not supplied both 8087 operation code and\n"
	      "ESC operand in 8086.  In addtion to only supplied to *\n"
	      ".com file.  Please call me TEL 02-999-2010 if BUG in t\n"
	      "he Tiny Assembler were founded -----------------------\n";

char    GF2[]="EX>TA1.EXE  BINfilename\n";

char    *reg[8][4] = {
	"bx+si","al","ax","es",
	"bx+di","cl","cx","cs",
	"bp+si","dl","dx","ss",
	"bp+di","bl","bx","ds",
	"si",   "ah","sp","es",
	"di",   "ch","bp","cs",
	"bp",   "dh","si","ss",
	"bx",   "bh","di","ds",
	};

char	*base[]={"bxsi","bxdi","bpsi","bpdi",
		 "sibx","dibx","sibp","dibp",
		 "bxbp","bpbx","sidi","disi",
		 "bxbx","bpbp","sisi","didi",NULL};

char	*str[]={"byte","word","ptr","near","far",NULL};

/* ---- Error massage *pointer -------------------------------------------- */
char	*error[]={"Syntex is not correct",
		  "Not 8086 operation code",
		  "Only not operation code",
		  "Syntax is OP 1oper,2oper",
		  "Type is not equal",
		  "2 operland is '1' or 'CL'",
		  "Syntax is OP 1oper",
		  "Syntax is OP byte ptr or word ptr",
		  "Memory type(byte,word) need",
		  "Not hexa string",
		  "Bigger than 2 byte",
		  "Syntax is J???  offset",
		  "Offset is wrong",
		  "Offset bigger than 127 byte",
		  "1oper is 00 - ff",
		  "Syntax is L?????  offset",
		  "Meory address need '[' or ']'",
		  "Base index mode is not correct",
		  "Syntax is byte ptr or word ptr [base addressing]",
		  "Syntax is [base+index+direct]",
		  "Base register is bx,bp,si,di",
		  "bp register is [bp+disp8] or [bp+disp16]",
		  "Direct mode is wrong",
		  "Syntax is OP 1oper",
		  "Syntax is [base+index]",
		  "Syntax is [base or index + disp8 or disp16]",
		  "Syntax is [base+index+disp8 or disp16]",
		  "Register is ?x,?i,?p,?h,?l",
		  "',' not found",
		  "Syntax is push or pop 16bit_reg",
		  "Syntax is push or pop word ptr [base] or [base]",
		  "Bigger than 1 byte",
		  "Syntax is OP reg,reg or mem or imm",
		  "Syntax is not OP mem,mem",
		  "1oper is wrong",
		  "1oper is not segment register",
		  "2oper is not segment register",
		  "1oper is only 16bit register",
		  "Syntax is 16imm:16imm(cs:ip)",
		  "Syntax is OP near or far [base]",
		  "near or far need",
		  "Syntax is L?? reg16,[base]",
		  "Syntax is in acc,imm8 or DX",
		  "Syntax is out imm8 or DX,acc",
		  "imm is 00 - ff",
		  "Syntax is not OP sreg,sreg",
		  "Syntax is mov sreg,16reg",
		  "Syntax is mov 16reg,sreg",
		  "2oper is only imm",
		  "Syntax is xchg reg,reg or mem"};

/* ---- Error massage constant define ------------------------------------- */
#define	ER00	0
#define ER01	1
#define ER02	2
#define ER03	3
#define ER04    4
#define ER05	5
#define	ER06	6
#define	ER07	7
#define	ER08	8
#define ER09	9
#define	ER10	10
#define ER11	11
#define ER12	12
#define ER13	13
#define ER14	14
#define ER15	15
#define ER16	16
#define	ER17	17
#define	ER18	18
#define ER19	19
#define ER20	20
#define ER21	21
#define ER22	22
#define ER23	23
#define ER24	24
#define ER25	25
#define ER26	26
#define ER27	27
#define ER28	28
#define ER29	29
#define ER30	30
#define ER31	31
#define ER32	32
#define ER33	33
#define ER34	34
#define ER35	35
#define ER36	36
#define ER37	37
#define ER38	38
#define ER39	39
#define ER40	40
#define ER41	41
#define ER42	42
#define ER43	43
#define ER44	44
#define ER45	45
#define ER46	46
#define ER47	47
#define ER48	48
#define ER49	49

/* ---- Other constant define --------------------------------------------- */
#define _BASE	0
#define _8BIT	1
#define	_16BIT	2
#define _SEG	3

#define	_1OPER	1
#define _2OPER	2

#define	_DB	-2
#define _DW	-3
#define _DATA	-4

#define _JMP	1
#define _CALL	2

#define	ERROR	-1

/* ---- start main -------------------------------------------------------- */
void    main(int argc,char *argv[])
{

	FILE    *fp1;

	int	toksize;
	char	*p1,*tok[25];
	char	toks[3]={0x09,0x20,0x00};        /* - TAB SPACE NULL ------ */

	char	*b0,*b1,b2[50],b3[10],b4[50];

	int	offset=0x0100,size=0,ret,a;

        puts(&GF1[0]);

	if ( argc != 2 ) {
		puts(&GF2[0]);
		exit(1);
		}

	if ( (fp1=fopen(argv[1],"wb")) == NULL ) {
                printf("%s file cannot open\n",argv[1]);
                exit(1);
		}

	if ( ( b0=malloc(1000) ) == NULL ) {
		puts("Out of Memory");
		exit(1);
		}

	b1=b0;

/* ---- main routine ------------------------------------------------------ */

	while( 1 ) {

		printf("%04x:%04x ",_DS,offset);
		gets(&b2[0]);

		strcpy(&b4[0],&b2[0]);
		strlwr(&b2[0]);

		if ( b2[0] == NULL ) {
			if ( size != 0 ) fwrite(b0,size,1,fp1);
			printf("\n%d byte writed\n",size);
			free(b0);
			fclose(fp1);
			exit(1);
			}

		toksize=0;
		p1=strtok(&b2[0],&toks[0]);
		while ( p1 != NULL ) {
			tok[toksize]=p1;
			toksize++;
			p1=strtok(NULL,&toks[0]);
			}

		switch ( *tok[0] ) {

			case 'a' : {
				ret=f_a_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'c' : {
				ret=f_c_code(&tok[0],&b3[0],toksize,offset);
				break;
				}

			case 'd' : {
				ret=f_d_code(&tok[0],&b3[0],toksize,&b4[0],b1);
				break;
				}

			case 'e' : {
				ret=f_e_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'h' : {
				ret=f_h_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'i' : {
				ret=f_i_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'j' : {
				ret=f_j_code(&tok[0],&b3[0],toksize,offset);
				break;
				}

			case 'l' : {
				ret=f_l_code(&tok[0],&b3[0],toksize,offset);
				break;
				}

			case 'm' : {
				ret=f_m_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'n' : {
				ret=f_n_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'o' : {
				ret=f_o_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'p' : {
				ret=f_p_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'r' : {
				ret=f_r_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 's' : {
				ret=f_s_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 't' : {
				ret=f_t_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'w' : {
				ret=f_w_code(&tok[0],&b3[0],toksize);
				break;
				}

			case 'x' : {
				ret=f_x_code(&tok[0],&b3[0],toksize);
				break;
				}

			default  : {
				ret=ERROR;
				b3[0]=ER01;
				}
			}

		if ( ret == ERROR ) {
			printf("          ERROR : %s\n",error[b3[0]]);
			continue;
			}

		if ( ret == _DATA ) {
			ret=0;
			ret=b3[1];
			ret=ret<<8;
			ret=ret|b3[0];
			b1=b1+ret;
			}
			else {
			for ( a=0 ; a<ret ; a++ ) {
				*b1=b3[a];
				b1++;
				}
			}

                offset=offset+ret;
		size=size+ret;
		}

}

/* ---- Sub routine ------------------------------------------------------- */
char	*a_code[]={"aaa","aad","aam","aas","adc","add","and",NULL};
int     f_a_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&a_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	switch ( a ) {

		case 0 : {
			*bp=0x37;                /* - aaa ----------------- */
			return(f_op(bp,toksize,1));
			}

		case 1 : {
			*bp=0xd5;                /* - aad ----------------- */
			*(bp+1)=0x0a;
			return(f_op(bp,toksize,2));
			}

		case 2 : {
			*bp=0xd4;                /* - aam ----------------- */
			*(bp+1)=0x0a;
                        return(f_op(bp,toksize,2));
			}

		case 3 : {
			*bp=0x3f;                /* - aas ----------------- */
			return(f_op(bp,toksize,1));
			}


		case 4 : {                       /* - adc ----------------- */
			return(f_2oper(&tok[0],bp,toksize,1,0x10,0));
			}

		case 5 : {                       /* - add ----------------- */
			return(f_2oper(&tok[0],bp,toksize,1,0x00,0));
			}

		case 6 : {           		 /* - and ----------------- */
			return(f_2oper(&tok[0],bp,toksize,0,0x20,0));
			}
		}
}

char	*c_code[]={"call","cbw","clc","cld","cli","cmc","cmp","cmpsb",
		   "cmpsw","cwd","cs:",NULL};
char	c_bin1[]={0x98,0xf8,0xfc,0xfa,0xf5};     /* - cbw clc cld cli cmc - */
char	c_bin2[]={0xa6,0xa7,0x99};		 /* - cmpsb cmpsw cwd ----- */
int     f_c_code(char *tok[],char *bp,int toksize,int offset)
{
 	int	a;

	a=f_strcmp(tok[0],&c_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( a >=1 && a<=5 ) {
		*bp=c_bin1[a-1];                 /* - cbw clc cld cli cmc - */
		return(f_op(bp,toksize,1));
		}

	if ( a >=7 && a<=9 ) {
		*bp=c_bin2[a-7];	         /* - cmpsb cmpsw cwd ----- */
		return(f_op(bp,toksize,1));
		}

	switch ( a ) {

		case 0 : {                       /*- call ----------------- */
			return(f_label_proc(&tok[0],bp,toksize,offset,_CALL));
			}

		case 6 : {			 /*- cmp ------------------ */
			return(f_2oper(&tok[0],bp,toksize,1,0x38,0));
			}

		case 10: {
			*bp=0x2e;                /* - cs: ----------------- */
			return(f_op(bp,toksize,1));
			}
		}

}

char	*d_code[]={"daa","das","db","dec","div","dw","ds:",NULL};
int     f_d_code(char *tok[],char *bp,int toksize,char *strP,char *b1)
{
 	int	a;

	a=f_strcmp(tok[0],&d_code[0]);

        if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	switch ( a ) {

		case 0 : {
			*bp=0x27;                /* - daa ----------------- */
			return(f_op(bp,toksize,1));
			}

		case 1 : {
			*bp=0x2f;                /* - das ----------------- */
			return(f_op(bp,toksize,1));
			}

		case 2 : {                       /* - db ------------------ */
			return(f_data(strP,bp,b1,_DB));
			}

		case 3 : {			 /* - dec ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0xfe,0x08,0x48,1,0));
			}

		case 4 : {                       /* - div ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0xf6,0x30,0,0,0));
			}

		case 5 : {                       /* - dw ------------------ */
			return(f_data(strP,bp,b1,_DW));
			}

		case 6 : {
			*bp=0x3e;                /* - ds: ----------------- */
			return(f_op(bp,toksize,1));
			}
		}
}

char	*e_code[]={"es:",NULL};
int     f_e_code(char *tok[],char *bp,int toksize)
{
	int	a;

	a=f_strcmp(tok[0],&e_code[0]);

        if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	*bp=0x26;                                /* - es: ----------------- */
	return(f_op(bp,toksize,1));
}

char	*h_code[]={"hlt",NULL};
int     f_h_code(char *tok[],char *bp,int toksize)
{
	int	a;

	a=f_strcmp(tok[0],&h_code[0]);

        if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	*bp=0xf4;                                /* - hlt ----------------- */
	return(f_op(bp,toksize,1));
}

char	*i_code[]={"idiv","imul","in","inc","int","into","iret",NULL};
int     f_i_code(char *tok[],char *bp,int toksize)
{
	unsigned char	c1;
	char	*p1;
	int	a,W;

	a=f_strcmp(tok[0],&i_code[0]);

        if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( a == 2 ) {                          /* - in ------------------ */
		a=f_reg(&tok[0],bp,toksize,_2OPER,2);
		if ( a == ERROR ) {
			if ( *bp == ER03 ) *bp=ER42;
			return(ERROR);
			}
		if ( *(bp+2) == 0xff ) {         /* - if segment reg ? ---- */
			*bp=ER35;
			return(ERROR);
			}
		if ( (*(bp+3)&0x07) != 0 ) {     /* - if not acc ? -------- */
			*bp=ER42;
			return(ERROR);
			}

		W=*(bp+2);

		if ( *(bp+1) == 1 ) {
			p1=strchr(tok[1],',');
			p1++;
			tok[1]=p1;
			}

		c1=*bp;
		*bp=NULL;
		a=f_str2hexa(tok[1],bp);
		if ( *bp == NULL ) {
			if ( a > 0x00ff ) {
				*bp=ER44;
				return(ERROR);
				}
			*bp=0xe4|W;
			*(bp+1)=a;
			return(2);
			}
			else {
			if ( *bp == ER10 ) {
				*bp=ER44;
				return(ERROR);
				}
			}
		*bp=c1;

		a=f_reg(&tok[0],bp,toksize,_1OPER,*bp);
		if ( a == ERROR ) {
			if ( *bp == ER23 ) *bp=ER42;
				return(ERROR);
			}
			else {
			if ( *(bp+2) == 0xff ) { /* - if segment reg ? ---- */
				*bp=ER36;
				return(ERROR);
				}
			if ( *(bp+2) == 0 ) {
				*bp=ER42;
				return(ERROR);
				}		/* - if not DX ? ---------- */
			if ( (*(bp+3)&0x07) != 0x02 ) {
				*bp=ER42;
				return(ERROR);
				}
			*bp=0xec|W;
			return(1);
			}
		}

	if ( a == 4 ) {                          /* - int ----------------- */

		if ( toksize != 2 ) {
			*bp=ER06;
			return(ERROR);
			}
		*bp=NULL;
		a=f_str2hexa(tok[1],bp);
		if ( *bp != NULL ) {
			if ( *bp == ER10 ) *bp=ER14;
			return(ERROR);
			}

		if ( a > 0x00ff ) {
			*bp=ER14;
			return(ERROR);
			}
		if ( a == 0x0003 ) {
			*bp=0xcc;                /* - int 3 --------------- */
			return(1);
			}

		*bp=0xcd;                        /* - int ?? -------------- */
		*(bp+1)=a;
		return(2);
		}

	switch ( a ) {

		case 0 : {                       /* - idiv ---------------- */
			return(f_1oper(&tok[0],bp,toksize,0xf6,0x38,0,0,0));
			}

		case 1 : {                       /* - imul ---------------- */
			return(f_1oper(&tok[0],bp,toksize,0xf6,0x28,0,0,0));
			}

		case 3 : {                       /* - inc ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0xfe,0x00,0x40,1,0));
			}

		case 5 : {
			*bp=0xce;                /* - into ---------------- */
			return(f_op(bp,toksize,1));
			}

		case 6 : {
			*bp=0xcf;                /* - iret ---------------- */
			return(f_op(bp,toksize,1));
			}
		}
}

char	*j_code[]={"jb","jnae","jbe","jna","jcxz","je","jz","jl","jnge",
		   "jle","jng","jmp","jnb","jae","jnbe","ja","jne","jnz",
		   "jnl","jge","jnle","jg","jno","jnp","jpo","jns","jo",
		   "jp","jpe","js",NULL};
int     f_j_code(char *tok[],char *bp,int toksize,int offset)
{
	int	a;

	a=f_strcmp(tok[0],&j_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( toksize != 2 ) {
		if ( a != 11 ) {
			*bp=ER11;
			return(ERROR);
			}
		}

	switch ( a ) {

		case 0 : {
			*bp=0x72;                /* - jb jnae ------------- */
			break;
			}

		case 1 : {
			*bp=0x72;                /* - jb jnae ------------- */
			break;
			}

		case 2 : {
			*bp=0x76;                /* - jbe jna ------------- */
			break;
			}

		case 3 : {
			*bp=0x76;                /* - jbe jna ------------- */
			break;
			}

		case 4 : {
			*bp=0xe3;                /* - jcxz ---------------- */
			break;
			}

		case 5 : {
			*bp=0x74;                /* - je ------------------ */
			break;
			}

		case 6 : {
			*bp=0x74;                /* - jz ------------------ */
			break;
			}

		case 7 : {
			*bp=0x7c;                /* - jl jnge ------------- */
			break;
			}

		case 8 : {
			*bp=0x7c;                /* - jl jnge ------------- */
			break;
			}

		case 9 : {
			*bp=0x7e;                /* - jle jng ------------- */
			break;
			}

		case 10: {
			*bp=0x7e;                /* - jle jng ------------- */
			break;
			}

		case 11: {                       /* - jmp ----------------- */
			return(f_label_proc(&tok[0],bp,toksize,offset,_JMP));
			}

		case 12: {
			*bp=0x73;                /* - jnb jae ------------- */
			break;
			}

		case 13: {
			*bp=0x73;                /* - jnb jae ------------- */
			break;
			}

		case 14: {
			*bp=0x77;                /* - jnbe ja ------------- */
			break;
			}

		case 15: {
			*bp=0x77;                /* - jnbe ja ------------- */
			break;
			}

		case 16: {
			*bp=0x75;                /* - jne jnz ------------- */
			break;
			}

		case 17: {
			*bp=0x75;                /* - jne jnz ------------- */
			break;
			}

		case 18: {
			*bp=0x7d;                /* - jnl jge ------------- */
			break;
			}

		case 19: {
			*bp=0x7d;                /* - jnl jge ------------- */
			break;
			}

		case 20: {
			*bp=0x7f;                /* - jnle jg ------------- */
			break;
			}

		case 21: {
			*bp=0x7f;                /* - jnle jg ------------- */
			break;
			}

		case 22: {
			*bp=0x71;                /* - jno ----------------- */
			break;
			}

		case 23: {
			*bp=0x7b;                /* - jnp jpo ------------- */
			break;
			}

		case 24: {
			*bp=0x7b;                /* - jnp jpo ------------- */
			break;
			}

		case 25: {
			*bp=0x79;                /* - jns ----------------- */
			break;
			}

		case 26: {
			*bp=0x70;                /* - jo ------------------ */
			break;
			}

		case 27: {
			*bp=0x7a;                /* - jp jpe -------------- */
			break;
			}

		case 28: {
			*bp=0x7a;                /* - jp jpe -------------- */
			break;
			}

		case 29:{
			*bp=0x78;                /* - js ------------------ */
			break;
			}
		}

	return(f_short_label(tok[1],bp,offset));
}

char	*l_code[]={"lahf","lds","lea","les","lock","lodsb","lodsw",
		   "loop","loopnz","loopne","loopz","loope",NULL};
char	l_bin[]={0xe2,0xe0,0xe0,0xe1,0xe1};
int     f_l_code(char *tok[],char *bp,int toksize,int offset)
{
 	int	a;

	a=f_strcmp(tok[0],&l_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( a > 6 ) {
		if ( toksize != 2 ) {
			*bp=ER15;
			return(ERROR);
			}
		*bp=l_bin[a-7];
		return(f_short_label(tok[1],bp,offset));
		}

	switch ( a ) {

		case 0 : {
			*bp=0x9f;                /* - lahf ---------------- */
			return(f_op(bp,toksize,1));
			}

		case 1 : {                       /* - lds ----------------- */
			return(f_load(&tok[0],bp,toksize,0xc5));
			}

		case 2 : {			 /* - lea ----------------- */
			return(f_load(&tok[0],bp,toksize,0x8d));
			}

		case 3 : {                       /* - les ----------------- */
			return(f_load(&tok[0],bp,toksize,0xc4));
			}

		case 4 : {
			*bp=0xf0;                /* - lock ---------------- */
			return(f_op(bp,toksize,1));
			}

		case 5 : {
			*bp=0xac;                /* - lodsb --------------- */
			return(f_op(bp,toksize,1));
			}

		case 6 : {
			*bp=0xad;                /* - lodsw --------------- */
			return(f_op(bp,toksize,1));
			}
		}

}

char	*m_code[]={"mov","movsb","movsw","mul",NULL};
int     f_m_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&m_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	switch ( a ) {

		case 0 : {                       /* - mov ----------------- */
			return(f_2oper(&tok[0],bp,toksize,3,0x88,0));
			}

		case 1 : {
			*bp=0xa4;                /* - movsb --------------- */
			return(f_op(bp,toksize,1));
			}

		case 2 : {
			*bp=0xa5;                /* - mobsw --------------- */
			return(f_op(bp,toksize,1));
			}

		case 4 : {                       /* - mul ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0xf6,0x20,0,0,0));
			}
		}

}

char	*n_code[]={"neg","nop","not",NULL};
int     f_n_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&n_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	switch ( a ) {

		case 0 : {                       /* - neg ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0xf6,0x18,0,0,0));
			}

		case 1 : {
			*bp=0x90;                /* - nop ----------------- */
			return(f_op(bp,toksize,1));
			}

		case 2 : {                       /* - not ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0xf6,0x10,0,0,0));
			}
		}

}

char	*o_code[]={"or","out",NULL};
int     f_o_code(char *tok[],char *bp,int toksize)
{
	unsigned char	c1,c2;
	char	*p1;
	int	a,sw,W;

	a=f_strcmp(tok[0],&o_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( a == 1 ) {           		 /* - out ----------------- */
		a=f_char_find(tok[1],',');
		if ( a > 1 ) {
			*bp=ER43;
			return(ERROR);
			}
		if ( a == 1 ) {
			p1=strchr(tok[1],',');
			*p1=NULL;
			sw=1;
			}
			else {
			sw=0;
			}

		W=0;
		*bp=NULL;
		a=f_str2hexa(tok[1],bp);
		if ( *bp == NULL ) {
			if ( a > 0x00ff ) {
				*bp=ER14;
				return(ERROR);
				}
			c1=a;
			if ( sw == 1 ) {
				if ( *(p1+1) == NULL ) {
					if ( toksize > 2 ) {
						*bp=3;
						}
						else {
						*bp=ER43;
						return(ERROR);
						}
					}
					else {
					p1++;
					tok[1]=p1;
					*bp=2;
					}
				}
				else {
				if ( toksize > 2 ) {
					if ( *tok[2] != ',' ) {
						*bp=ER28;
						return(ERROR);
						}
					if ( *(tok[2]+1) == NULL ) {
						if ( toksize > 3 ) {
							*bp=4;
							}
							else {
							*bp=ER43;
							return(ERROR);
							}
						}
						else {
						p1=tok[2];
						p1++;
						tok[2]=p1;
						*bp=3;
						}
					}
					else {
                                        *bp=ER43;
					return(ERROR);
					}
				}

			*(bp+1)=0;
			}
			else {
			if ( sw == 1 ) *p1=',';
			a=f_reg(&tok[0],bp,toksize,_2OPER,2);
			if ( a == ERROR ) {
				if ( *bp == ER03 ) *bp=ER43;
				return(ERROR);
				}
				else {           /* - if segment reg ? ---- */
				if ( *(bp+2) == 0xff ) {
					*bp=ER35;
					return(ERROR);
					}
				if ( *(bp+2) == 0 ) {
					*bp=ER43;
					return(ERROR);
					}	 /* - if not DX ? --------- */
				if ( (*(bp+3)&0x07) != 0x02 ) {
					*bp=ER43;
					return(ERROR);
					}
				W=1;
				}
			}

		if ( *(bp+1) == 1 ) {
			p1=strchr(tok[*bp-1],',');
			p1++;
			tok[*bp-1]=p1;
			}

		a=f_reg(&tok[0],bp,toksize,_1OPER,*bp);
		if ( a == ERROR ) {
			if ( *bp == ER23 ) *bp=ER43;
			return(ERROR);
			}
		if ( *(bp+2) == 0xff ) {         /* - if segment reg ? ---- */
			*bp=ER36;
			return(ERROR);
			}
		if ( (*(bp+3)&0x07) != 0 ) {     /* - if not acc ? -------- */
			*bp=ER42;
			return(ERROR);
			}

		if ( W == 1 ) {
			*bp=0xee|*(bp+2);
			return(1);
			}
			else {
			*bp=0xe6|*(bp+2);
			*(bp+1)=c1;
			return(2);
			}
		}

	switch ( a ) {

		case 0 : {                       /* - or ------------------ */
			return(f_2oper(&tok[0],bp,toksize,0,0x08,0));
			}

		}

}

char	*p_code[]={"pop","popf","push","pushf",NULL};
int     f_p_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&p_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	switch ( a ) {

		case 0 : {                       /* - pop ----------------- */
			return(f_1oper(&tok[0],bp,toksize,0x8f,0x00,0x58,2,
				       0x07));
			}

		case 1 : {
			*bp=0x9d;                /* - popf ---------------- */
			return(f_op(bp,toksize,1));
			}

		case 2 : {                       /* - push ---------------- */
			return(f_1oper(&tok[0],bp,toksize,0xff,0x30,0x50,2,
				       0x06));
			}

		case 3 : {
			*bp=0x9c;                /* - pushf --------------- */
			return(f_op(bp,toksize,1));
			}

		}

}

char	*r_code[]={"rcl","rcr","rep","repe","repz","repne",
		   "repnz","ret","retf","rol","ror",NULL};
char	r_bin[]={0xc2,0xca};                     /* - ret retf ------------ */
int     f_r_code(char *tok[],char *bp,int toksize)
{
	unsigned char	cc;
 	int	a;

	a=f_strcmp(tok[0],&r_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( a >=2 && a<=4 ) {
		*bp=0xf3;                        /* - rep repe repz ------- */
		return(f_op(bp,toksize,1));
		}

	if ( a == 5 || a == 6 ) {
		*bp=0xf2;                        /* - repne repnz --------- */
		return(f_op(bp,toksize,1));
		}

	if ( a == 7 || a == 8 ) {
		*bp=r_bin[a-7];                  /* - ret retf ------------ */
		if ( toksize == 1 ) {
			*bp=*bp|0x01;            /* - ret retf no pop-value */
			return(1);
			}
		if ( toksize != 2 ) {
			*bp=ER06;
			return(ERROR);
			}
		cc=*bp;
		*bp=NULL;
		a=f_str2hexa(tok[1],bp);
		if ( *bp != NULL ) return(ERROR);
		*bp=cc;                          /* - ret retf pop-value -- */
		*(bp+1)=a;
		a=a&0xff00;
		a=a>>8;
		*(bp+2)=a;
		return(3);
		}

	switch ( a ) {

		case 0 : {                       /* - rcl ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x10));
			}

		case 1 : {                       /* - rcr ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x18));
			}

		case 9 : {                       /* - rol ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x00));
			}

		case 10: {                       /* - ror ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x08));
			}
		}
}

char	*s_code[]={"sahf","sar","sbb","scasb","scasw","shl","sal","shr",
		   "stc","std","sti","stosb","stosw","sub","ss:",NULL};
char	s_bin[]={0xf9,0xfd,0xfb,0xaa,0xab};      /* - stc std sti stosb_w --*/
int     f_s_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&s_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	if ( a>=8 && a<=12 ) {
		*bp=s_bin[a-8];                  /* - stc std sti stosb_w - */
		return(f_op(bp,toksize,1));
		}

	switch ( a ) {

		case 0 : {
			*bp=0x9e;                /* - sahf ---------------- */
			return(f_op(bp,toksize,1));
			}

		case 1 : {                       /* - sar ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x38));
			}

		case 2 : {                       /* - sbb ----------------- */
			return(f_2oper(&tok[0],bp,toksize,1,0x18,0x00));
			}

		case 3 : {
			*bp=0xae;                /* - scasb --------------- */
			return(f_op(bp,toksize,1));
			}

		case 4 : {
			*bp=0xaf;                /* - scasw --------------- */
			return(f_op(bp,toksize,1));
			}

		case 5 : {                       /* - shl ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x20));
			}

		case 6 : {                       /* - sal ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x20));
			}

		case 7 : {                       /* - shr ----------------- */
			return(f_2oper(&tok[0],bp,toksize,2,0xd0,0x28));
			}

		case 13: {                       /* - sub ----------------- */
			return(f_2oper(&tok[0],bp,toksize,1,0x28,0x00));
			}

		case 14: {
			*bp=0x36;                /* - ss: ----------------- */
			return(f_op(bp,toksize,1));
			}
		}
}

char	*t_code[]={"test",NULL};
int     f_t_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&t_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}
						 /* - test ---------------- */
	return(f_2oper(&tok[0],bp,toksize,4,0x84,0x00));
	}

char	*w_code[]={"wait",NULL};
int     f_w_code(char *tok[],char *bp,int toksize)
{
	int	a;

	a=f_strcmp(tok[0],&w_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	*bp=0x9b;                                /* - wait ---------------- */
	return(f_op(bp,toksize,1));
}

char	*x_code[]={"xchg","xlat","xor",NULL};
int     f_x_code(char *tok[],char *bp,int toksize)
{
 	int	a;

	a=f_strcmp(tok[0],&x_code[0]);

	if ( a == ERROR ) {
		*bp=ER01;
		return(ERROR);
		}

	switch ( a ) {

		case 0 : {                       /* - xchg ---------------- */
			return(f_2oper(&tok[0],bp,toksize,5,0x84,0));
			}

		case 1 : {
			*bp=0xd7;                /* - xlat ---------------- */
			return(f_op(bp,toksize,1));
			}

		case 2 : {                       /* - xor ----------------- */
			return(f_2oper(&tok[0],bp,toksize,0,0x30,0));
			}
		}
}

/* ---- Sub Sub routine --------------------------------------------------- */
int	f_op(char *bp,int toksize,int writebyte)
{
	if ( toksize == 1 ) {
		return(writebyte);
		}
		else {
		*bp=ER02;
		return(ERROR);
		}

}

int	f_data(char *strP,char *bp,char *b1,int datatype)
{
	unsigned char	c1;
	char	*p1,*p2,*p3;
	int	a,b;

	p1=strP;
	p2=b1;

	p1=p1+2;				 /* - db or dw string skip  */

	while ( *p1 == 0x09  || *p1 == 0x20 ) p1++;
						 /* - 0x09 TAB 0x20 SPACE - */
	if ( *p1 == NULL || *p1 == 0x2c ) {      /* - 0x2c , -------------- */
		*bp=ER00;
		return(ERROR);
		}

	a=0;

	while ( *p1 != NULL ) {
		if ( *p1 == 0x22 ) {             /* - 0x22 " -------------- */
			p1++;
			while ( *p1 != 0x22 ) {
				if ( *p1 == NULL ) {
                                	*bp=ER00;
					return(ERROR);
					}
				*p2=*p1;
				p1++;
				p2++;
				a++;
				}
			p1++;
			while ( *p1 == 0x09 || *p1 == 0x20 ) p1++;
			if ( *p1 != NULL && *p1 != 0x2c ) {
				*bp=ER00;
				return(ERROR);
				}
			continue;
			}

		if ( *p1 == 0x27 ) {             /* - 0x27 ' -------------- */
			p1++;
			while ( *p1 != 0x27 ) {
				if ( *p1 == NULL ) {
                                	*bp=ER00;
					return(ERROR);
					}
				*p2=*p1;
				p1++;
				p2++;
				a++;
				}
			p1++;
			while ( *p1 == 0x09 || *p1 == 0x20 ) p1++;
			if ( *p1 != NULL && *p1 != 0x2c ) {
				*bp=ER00;
				return(ERROR);
				}
			continue;
			}

		if ( *p1 == 0x2c ) {             /* - 0x2c , -------------- */
			p1++;
			while ( *p1 == 0x09 || *p1 == 0x20 ) p1++;
			if ( *p1 == NULL ) {
                        	*bp=ER00;
				return(ERROR);
				}
			continue;
			}

		p3=p1;
		p1++;
		while ( 1 ) {
			if ( *p1 == NULL ) break;
			if ( *p1 == 0x09 ) break;
			if ( *p1 == 0x20 ) break;
			if ( *p1 == 0x2c ) break;
			p1++;
			}
		c1=*p1;
		*p1=NULL;
		*bp=0;
		b=f_str2hexa(p3,bp);
		if ( *bp != NULL ) {
			if ( datatype == _DB ) {
				if ( *bp == ER10 ) *bp=ER31;
				}
			return(ERROR);
			}

		if ( datatype == _DB ) {
			if ( b > 0x00ff ) {
				*bp=ER31;
				return(ERROR);
				}
			*p2=b;
			p2++;
			a++;
			}
			else {
			if ( datatype == _DW ) {
				*p2=b;
				p2++;
				b=b&0xff00;
				b=b>>8;
				*p2=b;
				p2++;
				a=a+2;
				}
			}

		*p1=c1;
                while ( *p1 == 0x09 || *p1 == 0x20 ) p1++;
		}

	*bp=a;
	a=a&0xff00;
	a=a>>8;
	*(bp+1)=a;
	return(_DATA);
}

int	f_short_label(char *strP,char *bp,int offset)
{
	unsigned char cc;
	int	a,b;

	cc=*bp;
	*bp=NULL;

	a=f_str2hexa(strP,bp);

	if ( *bp != NULL ) {
		if ( *bp == ER09 ) *bp=ER12;
		return(ERROR);
		}

	*bp=cc;
	offset=offset+2;
	b=a-offset;

	if ( offset >= a ) {

		if ( b < -128 ) {
			*bp=ER13;
			return(ERROR);
			}
			else {
			*(bp+1)=b;
			}
		}
		else {

		if ( b > 127 ) {
			*bp=ER13;
			return(ERROR);
			}
			else {
			*(bp+1)=b;
			}
		}

	return(2);
}

int	f_label_proc(char *tok[],char *bp,int toksize,int offset,int S)
{
	unsigned char 	c1;
	char	*p1;
	int	a,b;

	a=f_reg(&tok[0],bp,toksize,_1OPER,2);    /* - regptr 16 ----------- */
        if ( a == ERROR ) {
		if ( *bp != ER23 ) {
			if ( *bp != ER27 ) {
				if ( *bp != ER28 ) {
					return(ERROR);
					}
				}
			}
		}
		else {
		if ( *(bp+2) == 0 ) {
			*bp=ER37;
			return(ERROR);
			}
		if ( *(bp+2) == 0xff ) {
			*bp=ER35;
			return(ERROR);
			}
		c1=*(bp+3);
		if ( S == _JMP ) {
			c1=c1|0x20;
			}
			else {
			c1=c1|0x10;
			}

		*bp=0xff;
		*(bp+1)=c1;
		return(2);
		}

	*bp=NULL;
	a=f_str2hexa(tok[1],bp);                 /* - near ??? ------------ */
	if ( *bp == NULL ) {
		if ( toksize != 2 ) {
			*bp=ER06;
			return(ERROR);
			}

		if ( S == _JMP ) {
			c1=0xe9;

			offset=offset+2;
			b=a-offset;
			if ( offset >= a ) {
				if ( b < -128 ) {
					offset=offset-2;
					}
					else {
					c1=c1|0x02;
					*bp=c1;
					*(bp+1)=b;
					return(2);
					}
				}
				else {
				if ( b > 127 ) {
					offset=offset-2;
					}
					else {
					c1=c1|0x02;
					*bp=c1;
					*(bp+1)=b;
					return(2);
					}
				}
			}
			else {
			c1=0xe8;
			}
		a=a-offset-3;
		*bp=c1;
		*(bp+1)=a;
		a=a&0xff00;
		a=a>>8;
		*(bp+2)=a;
		return(3);
		}
		else {
		if ( *bp == ER10 ) {
			return(ERROR);
			}
		}

	a=f_char_find(tok[1],':');
	if ( a == 1 ) {                          /* - far ??? ------------- */
        	if ( toksize != 2 ) {
			*bp=ER06;
			return(ERROR);
			}
		p1=strchr(tok[1],':');
		*p1=NULL;

		*bp=NULL;
		a=f_str2hexa(tok[1],bp);
		if ( *bp != NULL ) return(ERROR);
		p1++;
		if ( *p1 == NULL ) {
			*bp=ER38;
			return(ERROR);
			}
		b=f_str2hexa(p1,bp);
		if ( *bp != NULL ) return(ERROR);
		if ( S == _JMP ) c1=0xea;
			else c1=0x9a;
		*bp=c1;                          /* - code save ----------- */
		*(bp+1)=b;                       /* - ip save ------------- */
		b=b&0xff00;
		b=b>>8;
		*(bp+2)=b;
		*(bp+3)=a;                       /* - cs save ------------- */
		a=a&0xff00;
		a=a>>8;
		*(bp+4)=a;
		return(5);
		}
		else {
		if ( a > 1 ) {
			*bp=ER38;
			return(ERROR);
			}
		}

	a=f_strcmp(tok[1],&str[0]);              /* - near far ??? -------- */
	if ( a == ERROR ) {
		if ( *tok[1] == '[' ) {
			*bp=ER40;
			return(ERROR);
			}
		*bp=ER09;
		return(ERROR);
		}
	if ( a < 3 ) {
		*bp=ER40;
		return(ERROR);
		}
	if ( a == 3 ) {
		if ( S == _JMP ) c1=0x20;
			else c1=0x10;
		}
		else {
		if ( S == _JMP ) c1=0x28;
			else c1=0x18;
		}

	a=f_mem(&tok[0],bp,toksize,_1OPER,3);
	if ( a != ERROR ) {
		if ( *(bp+2) != 0xff ) {
			*bp=ER39;
			return(ERROR);
			}

		*bp=0xff;
		*(bp+1)=c1|*(bp+3);

                switch ( *(bp+3)&0xc0 ) {

			case 0x00 : {            /* - Direct mode 0x00_0x06 */
				if ( (*(bp+3)&0x07) == 6 ) {
					*(bp+2)=*(bp+4);
					*(bp+3)=*(bp+5);
					return(4);
					}
				return(2);
				}

			case 0x40 : {            /* - disp8 --------------- */
				*(bp+2)=*(bp+4);
				return(3);
				}

			case 0x80 : {            /* - disp16 --------------- */
				*(bp+2)=*(bp+4);
				*(bp+3)=*(bp+5);
				return(4);
				}
			}

		}
		else {
		return(ERROR);
		}

}

int	f_load(char *tok[],char *bp,int toksize,unsigned char c1)
{
	unsigned char	c2;
	char	*p1;
	int	a;

	a=f_reg(&tok[0],bp,toksize,_2OPER,2);
	if ( a == ERROR ) {
		*bp=ER41;
		return(ERROR);
		}
	if ( *(bp+2) == 0 ) {
		*bp=ER37;
		return(ERROR);
		}
	if ( *(bp+2) == 0xff ) {
		*bp=ER35;
		return(ERROR);
		}
	c2=*(bp+3);
	c2=c2&0x07;
	c2=c2<<3;

	if ( *(bp+1) == 1 ) {
		p1=strchr(tok[1],',');
		p1++;
		tok[1]=p1;
		}

	a=f_mem(&tok[0],bp,toksize,_1OPER,*bp);
	if ( a == ERROR ) {
		if ( *bp == ER23 ) *bp=ER41;
		return(ERROR);
		}
	if ( *(bp+2) != 0xff ) {
		*bp=ER41;
		return(ERROR);
		}

	c2=c2|*(bp+3);
	*bp=c1;
	*(bp+1)=c2;

        switch ( *(bp+3)&0xc0 ) {

		case 0x00 : {                    /* - Direct mode 0x00_0x06 */
			if ( (*(bp+3)&0x07) == 6 ) {
				*(bp+2)=*(bp+4);
				*(bp+3)=*(bp+5);
				return(4);
				}
			return(2);
			}

		case 0x40 : {                    /* - disp8 --------------- */
			*(bp+2)=*(bp+4);
			return(3);
			}

		case 0x80 : {                   /* - disp16 --------------- */
			*(bp+2)=*(bp+4);
			*(bp+3)=*(bp+5);
			return(4);
			}
		}

}

int	f_1oper(char *tok[],char *bp,int toksize,unsigned char c1,
		unsigned char c2,unsigned char c3,unsigned char c4,
		unsigned char c5)
{
	int	a;

	a=f_reg(&tok[0],bp,toksize,_1OPER,2);
	if ( a == ERROR ) {
		if ( *bp != ER23 ) {
			if ( *bp != ER27 ) {
				if ( *bp != ER28 ) {
					return(ERROR);
					}
				}
			}
		}
		else {
		if ( c4 == 2 ) {                 /* - push pop mode ------- */
			if ( *(bp+2) == 0 ) {    /* - 8bit mode is error -- */
				*bp=ER29;
				return(ERROR);
				}
			if ( *(bp+2) == 1 ) {    /* - 16bit mode ---------- */
                                *bp=c3;
				c4=*(bp+3);
				c4=c4&0x3f;      /* - mod bit delete ------ */
				*bp=*bp|c4;
				return(1);
				}
				else {           /* - SEG mode ------------ */
				*bp=c5;
				c4=*(bp+3);
				c4=c4&0x3f;      /* - mod bit delete ------ */
				c4=c4<<3;
				*bp=*bp|c4;
				return(1);
				}
			}
		if ( c4 == 1 ) {                 /* - inc dec mode -------- */
			if ( *(bp+2) == 1 ) {
				*bp=c3;
				c4=*(bp+3);
				c4=c4&0x3f;      /* - mod bit delete ------ */
				*bp=*bp|c4;
				return(1);
				}
			}
		*bp=c1;                          /* - other mode div idiv.. */
		*bp=*bp|*(bp+2);
		*(bp+1)=*(bp+3)|c2;
		return(2);
		}

	a=f_mem(&tok[0],bp,toksize,_1OPER,2);
        if ( a == ERROR ) return(ERROR);

	if ( c4 == 2 ) {
		if ( *(bp+2) == 0 ) {
			*bp=ER30;
			return(ERROR);
			}
			else {
			*(bp+2)=1;
			}		 	 /* - W setting word ------ */
		}

	if ( *(bp+2) == 0xff ) {
		*bp=ER08;
		return(ERROR);
		}

	*bp=c1;
	*bp=*bp|*(bp+2);
	*(bp+1)=*(bp+3)|c2;

	switch ( *(bp+3)&0xc0 ) {

		case 0x00 : {                    /* - Direct mode 0x00_0x06 */
			if ( (*(bp+3)&0x07) == 6 ) {
				*(bp+2)=*(bp+4);
				*(bp+3)=*(bp+5);
				return(4);
				}
			return(2);
			}

		case 0x40 : {                    /* - disp8 --------------- */
			*(bp+2)=*(bp+4);
			return(3);
			}

		case 0x80 : {                   /* - disp16 --------------- */
			*(bp+2)=*(bp+4);
			*(bp+3)=*(bp+5);
			return(4);
			}
		}
}

int	f_2oper(char *tok[],char *bp,int toksize,int S,unsigned char c1,
		unsigned char c6)
{
	unsigned char	c2,c3,c4,c5;
	char	*p1;
	int	a,b,tokcounter,W;

	a=f_reg(&tok[0],bp,toksize,_2OPER,2);    /* - reg,??? mode -------- */
	if ( a != ERROR ) {
		tokcounter=*bp;
		if ( *(bp+2) == 0xff ) {         /* - if segment reg ? ---- */
			if ( S != 3 ) {          /* - mov seg,??? --------- */
				*bp=ER35;
				return(ERROR);
				}
			}
		W=*(bp+2);

		if ( S == 2 ) {
			c1=c1|*(bp+2);
			c2=*(bp+3);
			c2=c2|c6;
			}
			else {
			c1=c1|*(bp+2);   	 /* - code W or ----------- */
			c2=*(bp+3);
			c2=c2&0xc0;              /* - mode setting -------- */
			c3=*(bp+3);
			c3=c3&0x07;
			c3=c3<<3;
			c2=c2|c3;		 /* - 2operand code or ---- */
			}

		if ( *(bp+1) == 1 ) {
			p1=strchr(tok[*bp-1],',');
			p1++;
			tok[*bp-1]=p1;
			}
						 /* - reg,reg mode -------- */
		a=f_reg(&tok[0],bp,toksize,_1OPER,*bp);
		if ( a != ERROR ) {
			if ( S != 3 ) {          /* - if segment reg ? ---- */
				if ( *(bp+2) == 0xff ) {
					*bp=ER36;
					return(ERROR);
					}
				}

			if ( S == 2 ) {          /* - R?? S?? code -------- */
				if ( *(bp+2) != 0 ) {
					*bp=ER05;
					return(ERROR);
					}
				c3=*(bp+3);
				c3=c3&0x07;
				if ( c3 != 0x01 ) {
					*bp=ER05;
					return(ERROR);
					}
				c1=c1|0x02;
				*bp=c1;
				*(bp+1)=c2;
				return(2);
				}

			if ( S == 3 ) {          /* - mov seg RTN --------- */
				if ( W == 0xff ) {
					if ( *(bp+2) == 0xff ) {
						*bp=ER45;
						return(ERROR);
						}
					if ( *(bp+2) == 0 ) {
						*bp=ER46;
						return(ERROR);
						}
					c2=c2&0x38;
					*bp=0x8e;
					*(bp+1)=c2|*(bp+3);
					return(2);
					}
					else {
					if ( *(bp+2) == 0xff ) {
						if ( W == 0 ) {
							*bp=ER47;
							return(ERROR);
							}
						c2=c2&0x38;
						c2=c2>>3;
						c2=c2|0xc0;
						c3=*(bp+3);
						c3=c3&0x07;
						c3=c3<<3;
						c2=c2|c3;
						*bp=0x8c;
						*(bp+1)=c2;
						return(2);
						}
					}
				}

			if ( W == *(bp+2) ) {
				if ( S != 4 ) c1=c1|0x02;
				c3=*(bp+3);
				c2=c2|c3;

				*bp=c1;
				*(bp+1)=c2;
				return(2);
				}
				else {
				*bp=ER04;
				return(ERROR);
				}
			}

		*bp=NULL;			 /* - reg,imm mode -------- */
		a=f_str2hexa(tok[tokcounter-1],bp);
		if ( *bp == NULL ) {
			if ( tokcounter != toksize ) {
				*bp=ER03;
				return(ERROR);
				}

			if ( S == 5 ) {
				*bp=ER49;
				return(ERROR);
				}

			if ( S == 2 ) {          /* - R?? S?? code -------- */
				if ( a != 1 ) {
					*bp=ER05;
					return(ERROR);
					}
				*bp=c1;
				*(bp+1)=c2;
				return(2);
				}

			if ( S == 3 ) {          /* - mov reg,imm --------- */
				if ( W == 0xff ) {
					*bp=ER35;
					return(ERROR);
					}
				c2=c2&0x38;
				c2=c2>>3;

				if ( W == 0 ) {
					if ( a > 0x00ff ) {
						*bp=ER31;
						return(ERROR);
						}
					c2=c2|0xb0;
					*bp=c2;
					*(bp+1)=a;
					return(2);
					}
					else {
					c2=c2|0xb8;
					*bp=c2;
					*(bp+1)=a;
					a=a&0xff00;
					a=a>>8;
					*(bp+2)=a;
					return(3);
					}
				}

			c3=c2;
			c3=c3&0x38;              /* - ax reg check ? ------ */
			if ( c3 == 0 ) {
				c1=c1|0x04;
				if ( S == 4 ) c1=0xa8|W;
				*bp=c1;
				*(bp+1)=a;
				if ( W == 0 ) {
					if ( a > 0x00ff ) {
						*bp=ER31;
						return(ERROR);
						}
					return(2);
					}
				a=a&0xff00;
				a=a>>8;
				*(bp+2)=a;
				return(3);
				}

			c3=c2;
			c3=c3&0x38;
			c3=c3>>3;                /* - reg setting --------- */

			c2=c1;
			c2=c2&0x38;              /* - reg,imm 2oper setting */
			c2=c2|0xc0;
			c2=c2|c3;

			c1=c1&0xc7;
			c1=c1|0x80;              /* - reg,imm 1oper setting */

			if ( S == 1 ) {          /* - S bit is 1 ---------- */
				if ( (a&0xff00) == 0xff00 ) {
					if ( (a&0x00ff) >= 0x0080 ) {
						c1=c1|0x02;
						a=a&0x00ff;
						W=0;
						 /* - For next processing - */
						}
					}
				}

			if ( S == 4 ) c1=0xf6|W;
			*bp=c1;
			*(bp+1)=c2;
			*(bp+2)=a;

			if ( W == 0 ) {
				if ( a > 0x00ff ) {
					*bp=ER31;
					return(ERROR);
					}
				return(3);
				}

			a=a&0xff00;
			a=a>>8;
			*(bp+3)=a;
			return(4);
			}

						 /* - reg,mem mode -------- */
		a=f_mem(&tok[0],bp,toksize,_1OPER,tokcounter);
		if ( a != ERROR ) {
			if ( S == 2 ) {          /* - S bit is 1 ---------- */
				*bp=ER05;
				return(ERROR);
				}

			if ( S == 3 ) {
				if ( W == 0xff ) {
					if ( *(bp+2) == 0 ) {
						*bp=ER04;
						return(ERROR);
						}
					c2=c2&0x38;
					c2=c2|*(bp+3);
					*bp=0x8e;
					*(bp+1)=c2;
					switch ( *(bp+3)&0xc0 ) {

						case 0x00 : {
						if ( (*(bp+3)&0x07) == 6 ) {
							*(bp+2)=*(bp+4);
							*(bp+3)=*(bp+5);
							return(4);
							}
						return(2);
						}

						case 0x40 : {
							*(bp+2)=*(bp+4);
							return(3);
							}

						case 0x80 : {
							*(bp+2)=*(bp+4);
							*(bp+3)=*(bp+5);
							return(4);
							}
						}
					}
				}

			if ( W == *(bp+2) || 0xff == *(bp+2) ) {
						 /* - operand change ------ */
				if ( S != 4 ) c1=c1|0x02;
				c2=c2&0x3f;
				c3=*(bp+3);
				c2=c2|c3;

				*bp=c1;
				*(bp+1)=c2;

				switch ( *(bp+3)&0xc0 ) {

					case 0x00 : {
						if ( (*(bp+3)&0x07) == 6 ) {
							*(bp+2)=*(bp+4);
							*(bp+3)=*(bp+5);
							return(4);
							}
						return(2);
						}

					case 0x40 : {
						*(bp+2)=*(bp+4);
						return(3);
						}

					case 0x80 : {
						*(bp+2)=*(bp+4);
						*(bp+3)=*(bp+5);
						return(4);
						}
					}
				}
				else {
				*bp=ER04;
				return(ERROR);
				}
			}
			else {
			if ( *bp == ER23 ) *bp=ER03;
			return(ERROR);
			}

		}

	a=f_mem(&tok[0],bp,toksize,_2OPER,2);    /* - mem,??? mode -------- */
	if ( a != ERROR ) {
		tokcounter=*bp;
		W=*(bp+2);
		c2=*(bp+3);                      /* - mod setting --------- */

		if ( S == 5 ) {
			*bp=ER49;
			return(ERROR);
			}

                switch ( *(bp+3)&0xc0 ) {

			case 0x00 : {
				if ( (*(bp+3)&0x07) == 6 ) {
					c4=*(bp+4);
					c5=*(bp+5);
					b=2;
					break;
					}
				b=0;
                                break;
				}

			case 0x40 : {
				c4=*(bp+4);
				b=1;
                                break;
				}

			case 0x80 : {
				c4=*(bp+4);
				c5=*(bp+5);
				b=2;
                                break;
				}
			}

		if ( *(bp+1) == 1 ) {
			p1=strchr(tok[*bp-1],',');
			p1++;
			tok[*bp-1]=p1;
			}
						 /* - mem,reg mode -------- */
		a=f_reg(&tok[0],bp,toksize,_1OPER,*bp);
		if ( a != ERROR ) {              /* - mod auto setting ---- */
			if ( *(bp+2) == 0xff ) { /* - if segment reg ? ---- */
				if ( S != 3 ) {
					*bp=ER36;
					return(ERROR);
					}
				}

                        if ( S == 2 ) {          /* - R?? S?? code -------- */
				if ( *(bp+2) != 0 ) {
					*bp=ER05;
					return(ERROR);
					}
				c3=*(bp+3);
				c3=c3&0x07;
				if ( c3 != 0x01 ) {
					*bp=ER05;
					return(ERROR);
					}
				if ( W == 0xff ) {
					*bp=ER08;
					return(ERROR);
					}

				c1=c1|W;
				c1=c1|0x02;
				c2=c2|c6;
				*bp=c1;
				*(bp+1)=c2;
				switch ( b ) {
					case 0 : {
						return(2);
						}

					case 1 : {
						*(bp+2)=c4;
						return(3);
						}

					case 2 : {
						*(bp+2)=c4;
						*(bp+3)=c5;
						return(4);
						}
					}
				}

			if ( S == 3 ) {
				if ( *(bp+2) == 0xff ) {
					if ( W == 0 ) {
						*bp=ER04;
						return(ERROR);
						}

					c3=*(bp+3);
					c3=c3&0x07;
					c3=c3<<3;
					c2=c2|c3;
					*bp=0x8c;
					*(bp+1)=c2;
					switch ( b ) {

						case 0 : {
							return(2);
							}

						case 1 : {
							*(bp+2)=c4;
							return(3);
							}

						case 2 : {
							*(bp+2)=c4;
							*(bp+3)=c5;
							return(4);
							}
						}

					}
				}

			if ( W == 0xff ) W=*(bp+2);
			if ( W == *(bp+2) ) {
				c1=c1|W;         /* - code W or ----------- */

				c3=*(bp+3);
				c3=c3&0x07;
				c3=c3<<3;
				if ( S == 4 ) {
					*bp=ER48;
					return(ERROR);
					}
					else {
					c2=c2|c3;
					}
				*bp=c1;
				*(bp+1)=c2;
				switch ( b ) {
					case 0 : {
						return(2);
						}

					case 1 : {
						*(bp+2)=c4;
						return(3);
						}

					case 2 : {
						*(bp+2)=c4;
						*(bp+3)=c5;
						return(4);
						}
					}
				}
				else {
				*bp=ER04;
				return(ERROR);
				}
			}

		*bp=NULL;			 /* - mem,imm mode -------- */
		a=f_str2hexa(tok[tokcounter-1],bp);
		if ( *bp == NULL ) {
			if ( W == 0xff ) {       /* - mod check ? --------- */
				*bp=ER08;        /* - if mod is ff ERROR -- */
				return(ERROR);
				}
			if ( tokcounter != toksize ) {
				*bp=ER03;
				return(ERROR);
				}

			if ( S == 2 ) {          /* - R?? S?? ------------- */
				if ( a != 1 ) {
					*bp=ER05;
					return(ERROR);
					}
				if ( W == 0xff ) {
					*bp=ER08;
					return(ERROR);
					}

				c1=c1|W;
				c2=c2|c6;
				*bp=c1;
				*(bp+1)=c2;

                                switch ( b ) {
					case 0 : {
						return(2);
						}

					case 1 : {
						*(bp+2)=c4;
						return(3);
						}

					case 2 : {
						*(bp+2)=c4;
						*(bp+3)=c5;
						return(4);
						}
					}
				}

			if ( S == 3 ) {
				if ( W == 0xff ) {
					*bp=ER08;
					return(ERROR);
					}

				*bp=0xc6|W;
				*(bp+1)=c2;

                                switch ( b ) {
					case 0 : {
						b=2;
						break;
						}

					case 1 : {
						*(bp+2)=c4;
						b=3;
                                                break;
						}

					case 2 : {
						*(bp+2)=c4;
						*(bp+3)=c5;
						b=4;
                                                break;
						}
					}

				*(bp+b)=a;

				if ( W == 0 ) {
					if ( a > 0x00ff ) {
						*bp=ER31;
						return(ERROR);
						}
					return(b+1);
					}

				a=a&0xff00;
				a=a>>8;
				*(bp+b+1)=a;
				return(b+2);
				}

			c1=c1|W;                 /* - code W or ----------- */
			c3=c1;
			c3=c3&0x38;
			c2=c2|c3;                /* - 2oper --------------- */

			c1=c1&0xc7;
			c1=c1|0x80;              /* - reg,imm 1oper setting */

			if ( S == 1 ) {          /* - S bit is 1 ---------- */
				if ( (a&0xff00) == 0xff00 ) {
					if ( (a&0x00ff) >= 0x0080 ) {
						c1=c1|0x02;
						a=a&0x00ff;
						W=0;
						 /* - For next processing - */
						}
					}
				}
			if ( S == 4 ) {
				c1=0xf6|W;
				c2=*(bp+3);
				}
			*bp=c1;
			*(bp+1)=c2;

                        switch ( b ) {
				case 0 : {
					b=2;
                                        break;
					}

				case 1 : {
					*(bp+2)=c4;
					b=3;
                                        break;
					}

				case 2 : {
					*(bp+2)=c4;
					*(bp+3)=c5;
					b=4;
                                        break;
					}
				}

			*(bp+b)=a;

			if ( W == 0 ) {
				if ( a > 0x00ff ) {
					*bp=ER31;
					return(ERROR);
					}
				return(b+1);
				}

			a=a&0xff00;
			a=a>>8;
			*(bp+b+1)=a;
			return(b+2);
			}

						 /* - reg,mem mode -------- */
		a=f_mem(&tok[0],bp,toksize,_1OPER,tokcounter);
		if ( a != ERROR ) {
			if ( S == 2 ) {          /* - S bit is 1 ---------- */
				*bp=ER05;
				return(ERROR);
				}
			*bp=ER33;
			return(ERROR);
			}

		*bp=ER09;
		return(ERROR);
		}

	*bp=ER34;
	return(ERROR);
}

int	f_reg(char *tok[],char *bp,int toksize,int opertype,int tokcounter)
{
	unsigned char	c1;
	char	b1[10],*p1;
	int	a,sw,W;

	if ( tokcounter > toksize ) {
		if ( opertype == _1OPER ) {
			*bp=ER06;
			}
			else {
			*bp=ER03;
			}

		return(ERROR);
		}

	a=f_char_find(tok[tokcounter-1],',');
	if ( a != 0 ) {
		if ( opertype == _1OPER ) {
			*bp=ER23;
			return(ERROR);
			}
		if ( a != 1 ) {
			*bp=ER00;
			return(ERROR);
			}
		p1=strchr(tok[tokcounter-1],',');

		c1=*p1;
		*p1=NULL;
		sprintf(&b1[0],"%s",tok[tokcounter-1]);
		*p1=c1;

		if ( *(p1+1) == NULL ) {
			tokcounter++;
			if ( tokcounter > toksize ) {
				*bp=ER03;
				return(ERROR);
				}
			sw=0;
			}
			else {
			sw=1;
			}
		}
		else {
		sprintf(&b1[0],"%s",tok[tokcounter-1]);
		tokcounter++;
		if ( tokcounter > toksize ) {
			if ( opertype == _2OPER ) {
                        	*bp=ER03;
				return(ERROR);
				}
			}
			else {
			if ( opertype == _1OPER ) {
				*bp=ER23;
				return(ERROR);
				}
			if ( *(tok[tokcounter-1]) != ',' ) {
				*bp=ER28;
				return(ERROR);
				}
			if ( *(tok[tokcounter-1]+1) == NULL ) {
				tokcounter++;
				if ( tokcounter > toksize ) {
					*bp=ER03;
					return(ERROR);
					}
				sw=0;
				}
				else {
				sw=1;
				}
			}
		}

	*bp=tokcounter;
	*(bp+1)=sw;

	a=f_reg_cmp(&b1[0],_8BIT);
	if ( a != ERROR ) {
		W=0;
		*(bp+2)=W;
		c1=0xc0;
		*(bp+3)=c1|a;
		return(1);
		}

	a=f_reg_cmp(&b1[0],_16BIT);
	if ( a != ERROR ) {
		W=1;
		*(bp+2)=W;
		c1=0xc0;
		*(bp+3)=c1|a;
		return(1);
		}

	a=f_reg_cmp(&b1[0],_SEG);
	if ( a != ERROR ) {
		W=-1;
		*(bp+2)=W;
		c1=0xc0;
		*(bp+3)=c1|a;
		return(1);
		}

	*bp=ER27;
	return(ERROR);
}

int	f_mem(char *tok[],char *bp,int toksize,int opertype,int tokcounter)
{
	unsigned char	c1;
	char	b1[30],*b2[10],b3[30],*p1,*p2;
	int	a,b,sw,len,basetok,W;

	if ( tokcounter > toksize ) {
		if ( opertype == _1OPER ) {
			*bp=ER06;
			}
			else {
			*bp=ER03;
			}

		return(ERROR);
		}

	a=f_strcmp(tok[tokcounter-1],&str[0]);
	if ( a == 0 || a == 1 ) {
		W=a;
		tokcounter++;
		if ( tokcounter > toksize ) {
                	if ( opertype == _1OPER ) {
				*bp=ER06;
				}
				else {
				*bp=ER03;
				}
			return(ERROR);
			}

		a=f_strcmp(tok[tokcounter-1],&str[0]);
		if ( a != 2 ) {
			*bp=ER07;
			return(ERROR);
			}

                tokcounter++;
		if ( tokcounter > toksize ) {
                	if ( opertype == _1OPER ) {
				*bp=ER06;
				}
				else {
				*bp=ER03;
				}
			return(ERROR);
			}
		}
		else {
		if ( a == 2 ) {
			*bp=ER07;
			return(ERROR);
			}
		if ( a == 3 || a == 4 ) {
			*bp=ER08;
			return(ERROR);
			}
		if ( *tok[tokcounter-1] != '[' ) {
			*bp=ER00;
			return(ERROR);
			}
		W=-1;
		}

	if ( *tok[tokcounter-1] != '[' ) {
		*bp=ER16;
		return(ERROR);
		}
	if ( *(tok[tokcounter-1]+1) == NULL ) {
		tokcounter++;
		if ( tokcounter > toksize ) {
                        if ( opertype == _1OPER ) {
				*bp=ER06;
				}
				else {
				*bp=ER03;
				}
			return(ERROR);
			}
		if ( *(tok[tokcounter-1]) == '+' ) {
			*bp=ER00;
			return(ERROR);
			}
		}
		else {
		if ( *(tok[tokcounter-1]+1) == '+' ) {
			*bp=ER00;
			return(ERROR);
			}
		}

	len=0;
	p1=&b1[0];
	while ( 1 ) {
		a=f_char_find(tok[tokcounter-1],',');
		if ( a != 0 ) {
			if ( opertype == _1OPER ) {
				*bp=ER23;
				return(ERROR);
				}
			if ( a != 1 ) {
				*bp=ER00;
				return(ERROR);
				}
			p2=strchr(tok[tokcounter-1],',');

			c1=*p2;
			*p2=NULL;
			a=sprintf(p1+len,"%s",tok[tokcounter-1]);
			len=len+a;
			*p2=c1;

			if ( *(p2+1) == NULL ) {
				tokcounter++;
				if ( tokcounter > toksize ) {
					*bp=ER03;
					return(ERROR);
					}
				sw=0;
				break;
				}
				else {
				sw=1;
				break;
				}
			}

		a=sprintf(p1+len,"%s",tok[tokcounter-1]);
		len=len+a;
		tokcounter++;
		if ( tokcounter > toksize ) {
			if ( opertype == _1OPER ) {
				break;
				}
			*bp=ER03;
			return(ERROR);
			}
		}

	p1=&b1[0];
	if ( *p1 != '[' ) {
		a=f_char_find(p1,'[');
		if ( a != 0 ) {
			*bp=ER18;
			return(ERROR);
			}
		}
		else {
		a=f_char_find(p1,'[');
		if ( a !=1 ) {
			*bp=ER18;
			return(ERROR);
			}
		}

	a=f_char_find(p1,']');
	if ( a != 1 ) {
		*bp=ER18;
		return(ERROR);
		}
	p2=strchr(p1,']');
	if ( *(p2+1) != NULL ) {
		*bp=ER18;
		return(ERROR);
		}

	a=f_char_find(p1,'+');
	if ( a > 2 ) {
		*bp=ER17;
		return(ERROR);
		}
	p2=p1;
	for ( b=0 ; b<a ; b++ ) {
		p2=strchr(p2,'+');
		if ( *(p2+1) == '+' || *(p2+1) == ']' ) {
			*bp=ER19;
			return(ERROR);
			}
		}

	basetok=0;
	p2=strtok(p1,"[]+");
	while ( p2 != NULL ) {
		b2[basetok]=p2;
		basetok++;
		p2=strtok(NULL,"[]+");
		}

	*bp=tokcounter;
	*(bp+1)=sw;
	*(bp+2)=W;

	for ( b=0 ; b<basetok ; b++ ) {
		a=f_reg_cmp(b2[b],_SEG);
		if ( a != ERROR ) {
			*bp=ER20;
			return(ERROR);
			}
		a=f_reg_cmp(b2[b],_8BIT);
		if ( a != ERROR ) {
			*bp=ER20;
			return(ERROR);
			}
		a=f_reg_cmp(b2[b],_16BIT);
		if ( a != ERROR ) {
			if ( a < 3 || a == 4 ) {
				*bp=ER20;
				return(ERROR);
				}
			}
		}

	switch ( basetok ) {

		case 0 : {
			*bp=ER19;
			return(ERROR);
			}

		case 1 : {
			a=f_reg_cmp(b2[0],_BASE);
			if ( a != ERROR ) {
				if ( a == 6 ) {
					*bp=ER21;
					return(ERROR);
					}
				c1=0;
				c1=c1|a;
				*(bp+3)=c1;
				}
				else {
				c1=*bp;
				*bp=0;
				a=f_str2hexa(b2[0],bp);
				if ( *bp != NULL ) {
					if ( *bp == ER09 ) *bp=ER22;
					return(ERROR);
					}
				*bp=c1;
				c1=0x06;	 /* - Direct mode --------- */
				*(bp+3)=c1;
				*(bp+4)=a;
				a=a&0xff00;
				a=a>>8;
				*(bp+5)=a;
				}

			return(1);
			}
		case 2 : {
			sprintf(&b3[0],"%s%s",b2[0],b2[1]);
			a=f_strcmp(&b3[0],&base[0]);
			if ( a != ERROR ) {
				if ( a > 3 ) {
					*bp=ER24;
					return(ERROR);
					}
				c1=0;
				c1=c1|a;
				*(bp+3)=c1;
				return(1);
				}

			a=f_reg_cmp(b2[0],_BASE);
			if ( a == ERROR ) {
				*bp=ER25;
				return(ERROR);
				}

			c1=*bp;
			*bp=NULL;
			b=f_str2hexa(b2[1],bp);
			if ( *bp != NULL ) {
				*bp=ER12;
				return(ERROR);
				}
			*bp=c1;
			c1=0;
			if ( b > 0x00ff ) {
				c1=0x80;
				c1=c1|a;
				*(bp+3)=c1;
				*(bp+4)=b;
				b=b&0xff00;
				b=b>>8;
				*(bp+5)=b;
				}
				else {
				c1=0x40;
				c1=c1|a;
				*(bp+3)=c1;
				*(bp+4)=b;
				}

			return(1);
			}

		case 3 : {
                	sprintf(&b3[0],"%s%s",b2[0],b2[1]);
			a=f_strcmp(&b3[0],&base[0]);
			if ( a != ERROR ) {
				if ( a > 3 ) {
					*bp=ER26;
					return(ERROR);
					}
				}
				else {
				*bp=ER26;
				return(ERROR);
				}

			b=f_reg_cmp(b2[2],_BASE);
			if ( b != ERROR ) {
				*bp=ER26;
				return(ERROR);
				}

			c1=*bp;
			*bp=NULL;
			b=f_str2hexa(b2[2],bp);
			if ( *bp != NULL ) {
				*bp=ER12;
				return(ERROR);
				}
			*bp=c1;
			c1=0;
			if ( b > 0x00ff ) {
				c1=0x80;
				c1=c1|a;
				*(bp+3)=c1;
				*(bp+4)=b;
				b=b&0xff00;
				b=b>>8;
				*(bp+5)=b;
				}
				else {
				c1=0x40;
				c1=c1|a;
				*(bp+3)=c1;
				*(bp+4)=b;
				}

			return(1);
			}
		}
}

int	f_strcmp(char *strP,char *ta1_code[])
{
	int	a=0;

	while( ta1_code[a] != NULL ) {
		if ( strcmp(strP,ta1_code[a]) == NULL ) {
			return(a);
			}
		a++;
		}

	return(ERROR);
}

int	f_reg_cmp(char *strP,int regmode)
{
	int	a1,a2;

	a1=0;

	if ( regmode == _SEG ) {
		while ( a1<4 ) {
			a2=strcmp(strP,reg[a1][regmode]);
			if ( a2 == NULL ) return(a1);
			a1++;
			}

		return(ERROR);
		}

	while ( a1<8 ) {
		a2=strcmp(strP,reg[a1][regmode]);
		if ( a2 == NULL ) return(a1);
		a1++;
		}

	return(ERROR);
}

int	f_char_find(char *strP,char cc)
{
	int	counter=0;

	while ( *strP != NULL ) {
		if ( *strP == cc ) counter++;
		strP++;
		}

	return(counter);
}


int	f_str2hexa(char *strP,char *bp)
{
	char	*p2;
	int	a=0,b=0,c,cc;

	while ( *strP == '0' ) strP++;

	p2=strP;

	while ( *strP != NULL ) {
		b++;

		if ( *strP >= '0' && *strP <= '9' ) {
			strP++;
			continue;
			}

		if ( *strP >= 'A' && *strP <= 'F' ) {
			strP++;
			continue;
			}

		if ( *strP >= 'a' && *strP <= 'f' ) {
			strP++;
			continue;
			}

		*bp=ER09;
		return(ERROR);
		}

	if ( b>4 ) {
		*bp=ER10;
		return(ERROR);
		}

	p2=p2+b;

	for ( c=0 ; c<b*2 ; c=c+2 ) {
		p2--;
		cc=0;

		if ( *p2 >= '0' && *p2 <= '9' ) cc=*p2-'0';
		if ( *p2 >= 'A' && *p2 <= 'F' ) cc=*p2-'A'+0x0a;
		if ( *p2 >= 'a' && *p2 <= 'f' ) cc=*p2-'a'+0x0a;

		cc=cc<<(c*4/2);

		a=a|cc;
		}

	return(a);
}