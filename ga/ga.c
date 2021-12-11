#include		 <io.h>
#include		 <time.h>
#include		 <alloc.h>
#include		 <stdio.h>
#include		 <stdlib.h>
#include		 <string.h>

#define		 BINOK		  1
#define		 BADBIN  2
#define		 BIGBIN  3

#define		 DECOK		  1
#define		 BADDEC  2
#define		 BIGDEC  3

#define		 HEXAOK  1
#define		 BADHEXA 2
#define		 BIGHEXA 3

#define		 STROK		  1
#define		 NOTDIVSTR 2
#define		 STROVER		  3

#define SYSERR  1
#define NORERR  2
#define WARNING 3

#define BELABEL 1
#define NOTLABEL	    2

#define BEEQU	 1
#define NOTEQU  2

#define NOTBIN  4
#define NULLBIN 5

#define NOTHEXA 4
#define NULLHEXA	    5

#define NOTVALUE	    6

#define NOTEDITC	    1

#define NOTERROR	    1
#define ERROR		    -1

#define BINSTR  2
#define DECSTR  10
#define HEXASTR 16

#define ON		    1
#define OFF	 2

#define _1PASS  1
#define _2PASS  2

#define GACHECK '\\'+'G'+'A'+'.'+'E'+'X'+'E'

enum opcodetype
{
	   _0OPER,_1OPER,_2OPER
};

enum operandtype
{
	   BYTE,WORD,SHORT,NEAR,FAR,NOT
};

enum codetype
{
	   _8IMM ,_16IMM,SREG	  ,_8REG ,_16REG,OFFSET,LABEL ,STRING,
	   BXSI	 ,BXDI  ,BPSI	  ,BPDI  ,SI			 ,DI  ,DIRECT,BX,
	   BXSI8 ,BXDI8 ,BPSI8 ,BPDI8 ,SI8 	   ,DI8	  ,BP8   ,BX8,
	   BXSI16,BXDI16,BPSI16,BPDI16,SI16	   ,DI16	,BP16    ,BX16,
	   NULLBLANK,
};

char *gf1="Copyright (C) GoldFish Good Friend 1995 All right reserved\n"
		"GoldFish Assembler version 1.1 by GoldFish Good Friends\n";

char *gf2="ga.exe [-option(s)] sourcefile [binaryfile] [listfile]\n"
		"-lxxx  labelnumber set  -1  1pass stop in error\n"
		"-exxx  equnumber set    -u  user open filename\n";

char *gf3="GoldFish Assembler version 1.1";

FILE *fp1=0,*fp2=0,*fp3=0,*fp4=0;

char *sourceP=0;
char *binaryP=0;

char *labelP=0;
int *labeloffsetP=0;
int labelnumber=100;
int labelcounter=0;

char *equP=0;
char *equvalueP=0;
int equnumber=50;
int equcounter=0;

char *tokarray[50];
int toknumber=0;
int tokcounter=0;

int codesize=0;
int datasize=0;
int offset=0;
int line=0;

int sourceline=0;
int codeline=0;

int warningcounter=0;
int errorcounter=0;

char assemblestop=_2PASS;																									  /* command line */
char useropenfilename=OFF;																									   /* options */

char sourcefilenamebuffer[13];																					 /* open source */
char binaryfilenamebuffer[13];																					  /* open binary */
char listfilenamebuffer[13];																								/* open list */

char linestrbuffer[200]; 																											/* 1 line buffer */
char *buffer1=&linestrbuffer[0];

char opstrbuffer1[150];																										   /* op code buffer*/
char *buffer2=&opstrbuffer1[0];

char opstrbuffer2[200];																										   /* opstr code */
char *buffer2_1=&opstrbuffer2[0];

char tokbuffer[100];																													   /* tok buffer */
char *buffer3=&tokbuffer[0];

char returncodebuffer[1000];																								/* bin buffer */
char *buffer4=&returncodebuffer[0];

char operandinformation[9];																						  /* operand buffer*/
char *buffer5=&operandinformation[0];

char *segmentregister[] =
{
	   "es","cs","ss","ds",NULL
};

char *generalregister8bit[] =
{
	   "al","cl","dl","bl","ah","ch","dh","bh",NULL
};

char *generalregister16bit[] =
{
	   "ax","cx","dx","bx","sp","bp","si","di",NULL
};

char *basememoryregister[] =
{
	   "bx","bp","si","di",NULL
};

char basememorytype[] =
{
	   BX,BXSI,BXSI8,BXSI16,BXDI,BXDI8,BXDI16,BX8,BX16,
	   BP8,BPSI,BPSI8,BPSI16,BPDI,BPDI8,BPDI16,BP8,BP16,
	   SI,SI8,SI16,DI,DI8,DI16,DIRECT
};

char *auxstr[] =
{
	   "byte","word","short","near","far",NULL
};

char *a_opcode[] =
{
	   "aaa","aas","aad","aam","adc","add","and",NULL
};

char *c_opcode[] =
{
	   "cbw","clc","cld","cli","cmc","cmpsb",
	   "cmpsw","cs:","cwd","call","cmp",NULL
};

char *d_opcode[] =
{
	   "daa","das","ds:","dec","div","db","ds","du","dw",NULL
};

char *e_opcode[] =
{
	   "es:","esc",NULL
};

char *h_opcode[] =
{
	   "hlt",NULL
};

char *i_opcode[] =
{
	   "into","iret","int","inc","imul","idiv","in",NULL
};

char *j_opcode[] =
{
	   "jb","jnae","jbe","jna","jcxz","je","jz","jl","jnge",
	   "jle","jng","jnb","jae","jnbe","ja","jne","jnz","jnl",
	   "jge","jnle","jg","jno","jnp","jpo","jns","jo","jp",
	   "jpe","js","jmp",NULL
};

char *l_opcode[] =
{
	   "lahf","lock","lodsb","lodsw","loop","loopnz",
	   "loopne","loopz","loope","lds","lea","les",NULL
};

char *m_opcode[] =
{
	   "movsb","movsw","mul","mov",NULL
};

char *n_opcode[] =
{
	   "nop","neg","not",NULL
};

char *o_opcode[] =
{
	   "org","or","out",NULL
};

char *p_opcode[] =
{
	   "popf","pushf","pop","push",NULL
};

char *r_opcode[] =
{
	   "rep","repe","repz","repne","repnz",
	   "ret","retf","rcl","rcr","rol","ror",NULL
};

char *s_opcode[] =
{
	   "sahf","scasb","scasw","stc","std","sti","stosb",
	   "stosw","ss:","sal","sar","shl","shr","sbb","sub",NULL
};

char *t_opcode[] =
{
	   "test",NULL
};

char *w_opcode[] =
{
	   "wait",NULL
};

char *x_opcode[] =
{
	   "xlat","xchg","xor",NULL
};

char a_binarycode[] =
{
	   0x37,0x3f,0xd5,0xd4,0x10,0x00,0x20
};

char c_binarycode[] =
{
	   0x98,0xf8,0xfc,0xfa,0xf5,0xa6,0xa7,0x2e,0x99
};

char d_binarycode[] =
{
	   0x27,0x2f,0x3e
};

char e_binarycode[] =
{
	   0x26
};

char h_binarycode[] =
{
	   0xf4
};

char i_binarycode[] =
{
	   0xce,0xcf
};

char j_binarycode[] =
{
	   0x72,0x72,0x76,0x76,0xe3,0x74,0x74,0x7c,0x7c,0x7e,
	   0x7e,0x73,0x73,0x77,0x77,0x75,0x75,0x7d,0x7d,0x7f,
	   0x7f,0x71,0x7b,0x7b,0x79,0x70,0x7a,0x7a,0x78
};

char l_binarycode[] =
{
	   0x9f,0xf0,0xac,0xad,0xe2,0xe0,0xe0,0xe1,0xe1,0xc5,0x8d,0xc4
};

char m_binarycode[] =
{
	   0xa4,0xa5
};

char n_binarycode[] =
{
	   0x90
};

char p_binarycode[] =
{
	   0x9d,0x9c
};

char r_binarycode[] =
{
	   0xf3,0xf3,0xf3,0xf2,0xf2,0xc3,0xcb,0x10,0x18,0x00,0x08
};

char s_binarycode[] =
{
	   0x9e,0xae,0xaf,0xf9,0xfd,0xfb,0xaa,0xab,
	   0x36,0x20,0x38,0x20,0x28,0x18,0x28
};

char w_binarycode[] =
{
	   0x9b
};

char x_binarycode[] =
{
	   0xd7
};

char editchararray[] =
{
	   '\\',0x5c,'0',NULL,'a',0x07,'b',0x08,'n',0x0a,
	   'r',0x0d,'t',0x09,'"',0x22,'\'',0x27,NULL,NULL
};

char optionsbuffer[] =
{
	   ON,																	 /* list */
	   ON,																	 /* line number */
	   ON,																	 /* offset show */
	   ON,																	 /* binary show */
	   ON,																	 /* comment */
};

char *onoptionstrarray[] =
{
	   "liston",
	   "linenumberon",
	   "offsetdisplayon",
	   "binarycodedisplayon",
	   "commenton",
	   NULL
};

char *offoptionstrarray[] =
{
	   "listoff",
	   "linenumberoff",
	   "offsetdisplayoff",
	   "binarycodedisplayoff",
	   "commentoff",
	   NULL
};

char    *warnings[] =
{
	   "label name long than 20 bytes",                      /* number is 0 */
	   "equ name long than 20 bytes",
	   "assemble stop in 1pass",
	   "'pop cs' operation code may be down to other system",
	   "base memory code '[bp]' not be itself",
	   "'[bp]' code exchange to '[bp+0]'",                   /* number is 5 */
	   "'pop cs' operation code exchange to 'db 0x0f'",
	   "'word' opertype need",
	   "'word' opertype append",
	   "'far' opertype need",
	   "'far' opertype append",                              /* number is 10 */
	   "'short' opertype need",
	   "'short' opertype append",
	   "'near' opertype need",
	   "'near' opertype append",
	   "1byte char need",                                    /* number is 15 */
	   "buffer maybe have to unknow value",
	   "'esc' opcode not supply",
};

char    *errors[] =
{
	   "command line not correct define",                    /* number is 0 */
	   "label option duble defined",
	   "label number bad",
	   "label number too big",
	   "equ option duble defined",
	   "equ number bad",                                     /* number is 5 */
	   "equ number too big",
	   "sourcefile not open",
	   "binaryfile not open",
	   "listfile not open",
	   "file too many open",                                       /* number is 10 */
	   "source file need",
	   "out of memory for sourcefile buffer",
	   "out of memory for label buffer",
	   "out of memory for equ buffer",
	   "working file not open",                              /* number is 15 */
	   "you must rename ga.exe",
	   "overflow laberbuffer! -Lxxx option resetting",
	   "overflow equbuffer! -Exxx option resetting",
	   "labelname dubbzle defined",
	   "equname dubble defined",                             /* number is 20 */
	   "need value",
	   "equ define syntax is !equname value",
	   "equ value only binary decimal or hexa",
	   "need hexa value",
	   "hexa range is 0x0000 - 0xffff",                      /* number is 25 */
	   "need binary value",
	   "binary range is 0b0000000000000000 - 0b1111111111111111",
	   "decimal renge is 0 - 65535",
	   "need label name",
	   "need equ name",                                      /* number is 30 */
	   "sourcefile can not closer",
	   "binaryfile can not close",
	   "listfile can not close",
	   "working file can not close",
	   "listoption define syntax is .listoptionname",        /* number is 35 */
	   "need listoption name",
	   "bad listoption name",
	   "can not realloc of sourcefile buffer",
	   "assemble stop option duble defined",
	   "user open file name option dubble defined",          /* number is 40 */
	   "syntax is not correct",
	   "bad binary",
	   "bad hexa",
	   "this equ not defined",
	   "this label not defined",                             /* number is 45 */
	   "not 8086/8087 operation code or system code",
	   "this code do not have to operand",
	   "this code have to 1operand",
	   "this code have to 2opernad",
	   "binary code buffer can not alloc",                   /* number is 50 */
	   "this code do not have opertype",
	   "1operand have to imm?? or !equname",
	   "opertype may be short",
	   "1operand have to imm?? !equname or &labelname",
	   "short jump is bigger than 127bytes",                 /* number is 55 */
	   "'int' opcode value is bigger than 0xff",
	   "'pop' opcode do not have 8bit register",
	   "'push' opcode do not have 8bit register",
	   "this 1operand do not have imm?? or !equname",
	   "this 1operand do not have segment register",         /* number is 60 */
	   "this 1operand do not have &labelname",
	   "this 1operand do not have string",
	   "'byte' or 'word' opertype need",
	   "opertype must 'byte' or 'word'",
	   "'byte' opertype maybe 'word'",                       /* number is 65 */
	   "'in' opcode 1operand have to 8bit or 16bit register",
	   "'in' opcode 1operand register have to 'al' or 'ax'",
	   "'in' opcode 2operand have to imm8 or 'dx'",
	   "'in' opcode 2opernad register have to 'dx'",
	   "'out' opcode 1operand have to imm8 or 'dx'",         /* number is 70 */
	   "'out' opcode 2operand have to 8bit or 16bit register",
	   "'out' opcode 2operand register have to 'al' or 'ax'",
	   "'out' opcode 1operand register have to 'dx'",
	   "2operand do not have opertype",
	   "rotate or shift opcode 2operand have to 1 or 'cl'", /* number is 75 */
	   "2operand have to imm?? or !equname",
	   "this code opertype have to 'far'",
	   "this code type must to 'far [basememory]'",
	   "this code opertype have to 'near'",
	   "this code do not have 8bit register",                /* number is 80 */
	   "opertype not same",
	   "this 2operand do not have segment register",
	   "'xchg' opcode 2operand do not have &labelname",
	   "this 2operand do not have string",
	   "1operand have to 16bit register",                    /* number is 85 */
	   "register operand do not have opertype",
	   "2operand have to 'far [basememory]' or 'far _labelname'",
	   "'db' opcode only have to imm8 or !equname(imm8)",
	   "'dw' opcode only have to imm?? !equname or &labelname",
	   "'du' opcode syntax is du size,1byte char",           /* number is 90 */
	   "2operand have to imm8 or !equname(imm8)",
	   "'ds' opcode only have to string operand",
	   "string have to end with char \"'\"",
	   "string have to end whit char '\"'",
	   "string over",                                        /* number is 95 */
	   "buffer size too big than 1000 bytes",
	   "'int' opcode 1operand only have to imm8 or !equname(imm8)",
	   "'xchg' opcode 1operand only have to register",
	   "'xchg' opcode 2operand do not have to imm?? or !equname",
	   "'test' opcode 2operand only have to register imm?? or !equname",
	   "segment register do not have to opertype",           /* number is 101 */
	   "2operand only have to 16bit register or 'word [basememory]'",
	   "'word' opertype need",
};

int f_initiation(int _argc,char *argv[]);
void f_1pass(char *assemblefile);
void f_2pass(void);
void f_close(void);
void f_error(int errortype,int errornumber);
char *f_linescaner(char *lineP,char *divP);
void f_bytewordprocess(void);
char f_editcharprocess(char *strP);
int f_labeladd(char *labelstrP);
int f_labelcheck(char *labelstrP,char *errorreturn);
int f_equadd(char *equstrP);
int f_equcheck(char *equstrP,char *errorreturn);
int f_gethexa(char *hexaP,char *errorreturn);
int f_getbin(char *binP,char *errorreturn);
int f_getvalue(char *strP,char *errorreturn);
void f_valuetostr(char *bufferP,int value,int strtype);
void f_listtitle(void);
int f_listoptionset(char *listoptionstrP);
void f_listprint(int binarycodenumber);
void f_listotherinformation(void);
int f_opcode(int passtype);
int f_operand(int passtype);
int f_suboperand(char *buffer,int passtype);
int f_operandregister(char *buffer,char *strP);
int f_operandmemory(char *buffer,int passtype);
int f_operandlabel(char *buffer,char *strP,int passtype);
int f_operandoffset(char *buffer,char *strP,int passtype);
int f_operandequ(char *buffer,char *strP);
int f_operandstr(char *buffer);
int f_operandvalue(char *buffer,char *strP);
int f_aopcode(void);
int f_copcode(int passtype);
int f_dopcode(int passtype);
int f_eopcode(void);
int f_hopcode(void);
int f_iopcode(void);
int f_jopcode(int passtype);
int f_lopcode(int passtype);
int f_mopcode(void);
int f_nopcode(void);
int f_oopcode(void);
int f_popcode(int passtype);
int f_ropcode(void);
int f_sopcode(void);
int f_topcode(void);
int f_wopcode(void);
int f_xopcode(void);
int f_aad_aamopcode(char binarycode);
int f_decopcode(void);
int f_dbopcode(void);
int f_dsopcode(void);
int f_duopcode(int passtype);
int f_dwopcode(void);
int f_inopcode(void);
int f_incopcode(void);
int f_intopcode(void);
int f_jmpopcode(int passtype);
int f_movopcode(void);
int f_orgopcode(void);
int f_outopcode(void);
int f_push_popopcode(int passtype,int codetype);
int f_ret_retfopcode(char binarycode);
int f_testopcode(void);
int f_xchgopcode(void);
int f_zerooperandcode(char binarycode);
int f_register16operandcode(char binarycode);
int f_valueoperandcode(int *returnvalue);
int f_shortlabeloperandcode(int passtype,char binarycode);
int f_resetIPoperandcode(int passtype,char c1,char c2,char c3,char c4);
int f_oneoperandcode(char *buffer,char binarycode1,char binarycode2);
int f_twooperandcode(char binarycode);
int f_loadaddress(int passtype,char binarycode);
int f_rotateshiftoperandcode(char binarycode1,char binarycode2);
char *f_cpyutilstr(char *buffer,char *strP1,char *strP2);
int	f_strtok(char *tokarray[],char *buffer,char *strP,char *div1,char *div2);
int	f_strarraycmp(char *strarray[],char *strP);
int	f_strtobin(char *strP,char *errorreturn);
int	f_strtodec(char *strP,char *errorreturn);
int	f_strtohexa(char *strP,char *errorreturn);
int	f_ascstr2codestr(char *buffer,char *strP,char *chararray,char *divstr,
																	   char *errorreturn,char c1,char c2);
char    *f_notcharfindcpystr(char *buffer,char *strP1,char *strP2,char c1);
char *f_strskipchar(char *strP,char c);
char *f_strskipstr(char *strP1,char *strP2);
int	f_arrangecheck(char c1,char c2,char c3);

void main(int argc,char *argv[])
{
	   int a;

	   a=f_initiation(argc,&argv[0]);
	   f_1pass(argv[a]);
	   f_2pass();
	   f_close();
}

int f_initiation(int _argc,char *_argv[])
{
	   char *p1;

	   int sourcefilenumber;
	   int openflag,labelflag,equflag,passflag,usfinaflag;
	   int a,b;

	   char c1;

	   puts(gf1);

	   while(*_argv[0]!=NULL) _argv[0]++;
	   while(*_argv[0]!='\\') _argv[0]--;
	   a=0;
	   while(*_argv[0]!=NULL)
	   {
			 a=a+*_argv[0];
			 _argv[0]++;
	   }

	   if(a!=GACHECK) f_error(SYSERR,16);

	   if(_argc==1)
	   {
			 puts(gf2);
			 exit(1);
	   }

	   openflag=0;
	   labelflag=0;
	   equflag=0;
	   passflag=0;
	   usfinaflag=0;																													   /* user file open*/
	   for(a=1;a<_argc;a++)
	   {
			 if(*_argv[a]=='-')
			 {
				    if(openflag!=0) f_error(SYSERR,0);					/* file opend ? */

				    _argv[a]++;

				    if(*_argv[a]=='l')
				    {
						  if(labelflag!=0) f_error(SYSERR,1);	   /* label define? */
						  labelflag++;

						  _argv[a]++;

						  b=f_strtodec(_argv[a],&c1);
						  switch(c1)
						  {
								case BADDEC :
								{
									   f_error(SYSERR,2);
									   break;
								}

								case BIGDEC :
								{
									   f_error(SYSERR,3);
									   break;
								}

						  }

						  if(b==0) f_error(SYSERR,0);
						  labelnumber=b;
						  continue;
				    }

				    if(*_argv[a]=='e')
				    {
						  if(equflag!=0) f_error(SYSERR,4);			/* equ define? */
						  equflag++;

						  _argv[a]++;

						  b=f_strtodec(_argv[a],&c1);
						  switch(c1)
						  {
								case BADDEC :
								{
									   f_error(SYSERR,5);
									   break;
								}

								case BIGDEC :
								{
									   f_error(SYSERR,6);
									   break;
								}

						  }

						  if(b==0) f_error(SYSERR,0);
						  equnumber=b;
						  continue;
				    }

				    if(*_argv[a]=='1')
				    {
						  if(passflag!=0) f_error(SYSERR,39);	   /* pass defined? */
						  passflag++;

						  _argv[a]++;

						  if(*_argv[a]!=NULL) f_error(SYSERR,0);
						  assemblestop=_1PASS;
						  continue;
				    }

				    if(*_argv[a]=='u')
				    {
						  if(usfinaflag!=0) f_error(SYSERR,40);  /* pass defined? */
						  usfinaflag++;

						  _argv[a]++;

						  if(*_argv[a]!=NULL) f_error(SYSERR,0);
						  useropenfilename=ON;
						  continue;
				    }

				    f_error(SYSERR,0);
			 }

			 switch(openflag)
			 {
				    case 0 :
				    {
						  strlwr(_argv[a]);
						  if(useropenfilename!=ON)
						  {
								p1=&sourcefilenamebuffer[0];
								p1=f_notcharfindcpystr(p1,_argv[a],".ga",'.');
								if(p1!=NULL) _argv[a]=p1;
						  }

						  fp1=fopen(_argv[a],"rb");
						  if(fp1==NULL) f_error(SYSERR,7);
						  sourcefilenumber=a;
						  openflag++;
						  break;
				    }

				    case 1 :
				    {
												    strlwr(_argv[a]);
												    if(useropenfilename!=ON)
						  {
								p1=&binaryfilenamebuffer[0];
								p1=f_notcharfindcpystr(p1,_argv[a],".com",'.');
								if(p1!=NULL) _argv[a]=p1;
						  }

						  fp2=fopen(_argv[a],"wb");
						  if(fp1==NULL) f_error(SYSERR,8);
						  openflag++;
						  break;
				    }

				    case 2 :
				    {
												    strlwr(_argv[a]);
												    if(useropenfilename!=ON)
						  {
								p1=&listfilenamebuffer[0];
								p1=f_notcharfindcpystr(p1,_argv[a],".gal",'.');
								if(p1!=NULL) _argv[a]=p1;
						  }

						  fp3=fopen(_argv[a],"wt");
						  if(fp1==NULL) f_error(SYSERR,9);
						  openflag++;
						  break;
				    }

				    default :
				    {
						  f_error(SYSERR,10);
				    }

			 }

	   }

	   fp4=tmpfile();
	   if(fp4==NULL) f_error(SYSERR,15);

	   if(openflag==0) f_error(SYSERR,11);

	   a=fileno(fp1);
	   b=filelength(a);

	   labelP=(char *)malloc(21*labelnumber);
	   if(labelP==NULL) f_error(SYSERR,13);

	   labeloffsetP=(int *)malloc(labelnumber);
	   if(labeloffsetP==NULL) f_error(SYSERR,13);

	   equP=(char *)malloc(21*equnumber);
	   if(equP==NULL) f_error(SYSERR,14);

	   equvalueP=(char *)malloc(19*equnumber);
	   if(equvalueP==NULL) f_error(SYSERR,14);

	   sourceP=(char *)malloc(b+1);
	   if(sourceP==NULL) f_error(SYSERR,12);

	   fread(sourceP,b,1,fp1);

	   if(*(sourceP+b-1)==0x1a) b--;
	   *(sourceP+b)=NULL;

	   if(fp3!=NULL) f_listtitle();

	   return(sourcefilenumber);
}

void f_1pass(char *assemblefile)
{
	   char *p1;
	   int a;

	   printf(" Assembling : %s\n\n",assemblefile);

	   codesize=0;
	   datasize=0;
	   offset=0;
	   p1=sourceP;
	   while(1)
	   {
			 p1=f_linescaner(p1,"[],+");
			 if(p1==NULL) break;

			 while(1)
			 {
				    a=0;																														 /* list control */
				    tokcounter=0;
				    if(toknumber==0) break;

				    if(*tokarray[tokcounter]=='.')
				    {
						  f_listoptionset(tokarray[tokcounter]+1);
						  break;
				    }

				    if(*tokarray[tokcounter]=='!')
				    {
						  f_equadd(tokarray[tokcounter]+1);
						  break;
				    }

				    if(*tokarray[tokcounter]=='_')
				    {
						  f_labeladd(tokarray[tokcounter]+1);
						  if(toknumber==1) break;
						  tokcounter++;
				    }

				    a=f_opcode(_1PASS);
				    if(a!=ERROR)
				    {
						  codesize=codesize+a;
						  offset=offset+a;
						  fprintf(fp4,"%5u%s\n",line,buffer2);
				    }

				    break;
			 }

			 if(fp3!=NULL) f_listprint(a);
	   }

	   line--;																																			  /* line over */
	   sourceline=line; 																														 /* souce save */
	   fputc(NULL,fp4); 																														 /* fileend print */
	   rewind(fp4);
}

void f_2pass(void)
{
	   char *p1,*p2,*p3;

	   int a,b;

	   if(errorcounter!=0)
	   {
			 if(assemblestop==_1PASS)
			 {
				    f_error(WARNING,2);
				    f_close();
				    exit(1);
			 }
	   }

	   a=fileno(fp4);
	   b=filelength(a);

	   sourceP=(char *)realloc(sourceP,b);
	   if(sourceP==NULL) f_error(SYSERR,38);
	   fread(sourceP,b,1,fp4);

	   binaryP=(char *)malloc(codesize);
	   if(binaryP==NULL) f_error(SYSERR,50);

	   codesize=0;
	   datasize=0;
	   offset=0;
	   p1=sourceP; 													   /* source data */
	   p2=binaryP; 													   /* binary data */
	   while(1)
	   {
			 p1=f_cpyutilstr(buffer1,p1,"\r\n");
								if(*p1=='\r' || *p1=='\n') p1++;
			 if(*p1=='\n' || *p1=='\r') p1++;
			 if(p1==NULL) break;
			 codeline++;
			 strcpy(buffer2,buffer1+5);
			 *(buffer1+5)=NULL;
			 line=atoi(buffer1);
			 toknumber=f_strtok(&tokarray[0],buffer3,buffer2,"\t ","[],+");
			 tokcounter=0;
			 if(*tokarray[tokcounter]=='_') tokcounter++;
			 a=f_opcode(_2PASS);
			 if(a!=ERROR)
			 {
				    codesize=codesize+a;
				    offset=offset+a;
				    p3=buffer4;
				    for(b=0;b<a;b++)
				    {
						  *p2=*p3;
						  p2++;
						  p3++;
				    }

			 }

	   }

	   if(fp3!=NULL) f_listotherinformation();

	   if(errorcounter==0 && fp2!=NULL) fwrite(binaryP,codesize,1,fp2);
}

void f_close(void)
{
	   int a;

	   free(sourceP);
	   free(labelP);
	   free(labeloffsetP);
	   free(equP);
	   free(equvalueP);

	   if(fp1!=NULL)
	   {
			 a=fclose(fp1);
			 if(a!=NULL) f_error(SYSERR,31);
	   }

	   if(fp2!=NULL)
	   {
			 a=fclose(fp2);
			 if(a!=NULL) f_error(SYSERR,32);
	   }

	   if(fp3!=NULL)
	   {
			 a=fclose(fp3);
			 if(a!=NULL) f_error(SYSERR,33);
	   }

	   if(fp4!=NULL)
	   {
			 a=fclose(fp4);
			 if(a!=NULL) f_error(SYSERR,34);
	   }

	   printf("%5u Warning(s)\n",warningcounter);
	   printf("%5u Error(s)\n\n",errorcounter);
}

void f_error(int errortype,int errornumber)
{
	   switch(errortype)
	   {
			 case SYSERR :
			 {
				    printf("System error : %s\n\n",errors[errornumber]);
				    exit(1);
			 }

			 case NORERR :
			 {
				    printf("Error(%u) : %s\n",line,errors[errornumber]);
				    if(fp3!=0)
						  fprintf(fp3,"Error(%u) : %s\n",line,errors[errornumber]);

				    errorcounter++;
				    break;
			 }

			 case WARNING :
			 {
				    printf("Warning(%u) : %s\n",line,warnings[errornumber]);
				    if(fp3!=0)
						  fprintf(fp3,"Warning(%u) : %s\n",line,
																	   warnings[errornumber]);

				    warningcounter++;
				    break;
			 }

	   }

}

char *f_linescaner(char *lineP,char *divP)
{
	   lineP=f_cpyutilstr(buffer1,lineP,"\r\n");
	   f_cpyutilstr(buffer2,buffer1,";");
	   strcpy(buffer2_1,buffer2);
	   f_bytewordprocess();
	   toknumber=f_strtok(&tokarray[0],buffer3,buffer2,"\t ",divP);
	   if(*lineP=='\r' || *lineP=='\n') lineP++;
	   if(*lineP=='\n' || *lineP=='\r') lineP++;
	   line++;
	   return(lineP);
}

void f_bytewordprocess(void)
{
	   char *p1,*p2;

	   int a;

	   char c1,c2,c3;

	   p1=buffer2;
	   p2=buffer2_1;
	   while(*p2!=NULL)
	   {
			 if(*p2==0x22 || *p2==0x27)																	    /* 0x22 " 0x27 ' */
			 {
				    c1=*p2;																											   /* div char */
				    p2++;

				    c2=f_editcharprocess(p2);
				    if(c2==NOTEDITC)
				    {
						  c2=*p2;
						  p2++;
				    }
				    else
				    {
						  p2=p2+2;
				    }

				    sprintf(p1,"0x%02x",c2);
				    p1=p1+4;

				    if(*p2!=c1)
				    {
						  c2=f_editcharprocess(p2);
						  if(c2==NOTEDITC)
						  {
								c2=*p2;
								p2++;
						  }
						  else
						  {
								p2=p2+2;
						  }

						  sprintf(p1,"%02x",c2);
						  p1=p1+2;

						  if(*p2!=c1)
						  {
								strcpy(buffer2,buffer2_1);
								p1=&c1;																			    /* dumy code */
								break;
						  }

				    }

				    p2++;
			 }
			 else
			 {
				    *p1=*p2;
				    p1++;
				    p2++;
			 }

	   }

	   *p1=NULL;
}

char f_editcharprocess(char *strP)
{
	   int a;

	   if(*strP!='\\') return(NOTEDITC);

	   strP++;

	   a=0;
	   while(1)
	   {
			 if(editchararray[a]==*strP) return(editchararray[a+1]);
			 if(editchararray[a]==NULL) return(NOTEDITC);
			 a=a+2;
	   }

}

int f_labeladd(char *labelstrP)
{
	   char *p1;

	   int a;
	   char c;

	   if(labelnumber==labelcounter) f_error(SYSERR,17);

	   p1=labelP+(labelcounter*21);

	   strncpy(p1,labelstrP,21);
	   a=strlen(labelstrP);
	   if(a==0)
	   {
			 f_error(NORERR,29);
			 return(ERROR);
	   }

	   if(a>20)
	   {
			 f_error(WARNING,0);
			 *(p1+20)=NULL;
	   }

	   a=f_labelcheck(p1,&c);
	   if(c==BELABEL)
	   {
			 f_error(NORERR,19);
			 return(ERROR);
	   }

	   *(labeloffsetP+labelcounter)=offset;
	   labelcounter++;
	   return(NOTERROR);
}

int f_labelcheck(char *labelstrP,char *errorreturn)
{
	   char *p1;
	   int *p2;
	   int a,b;

	   p1=(char *)labelP;
	   p2=(int *)labeloffsetP;
	   for(a=0;a<labelcounter;a++)
	   {
			 b=strcmp(p1,labelstrP);
			 if(b==NULL)
			 {
				    *errorreturn=BELABEL;
				    return(*p2);
			 }

			 p1=p1+21;
			 p2++;
	   }

	   *errorreturn=NOTLABEL;
	   return(ERROR);
}

int f_equadd(char *equstrP)
{
	   char *p1;

	   int a;
	   char c;

	   if(equnumber==equcounter) f_error(SYSERR,18);

	   p1=equP+(equcounter*21);

	   strncpy(p1,equstrP,21);
	   a=strlen(equstrP);
	   if(a==0)
	   {
			 f_error(NORERR,30);
			 return(ERROR);
	   }

	   if(a>20)
	   {
			 f_error(WARNING,1);
			 *(p1+20)=NULL;
	   }

	   a=f_equcheck(p1,&c);

	   if(c==BEEQU)
	   {
			 f_error(NORERR,20);
			 return(ERROR);
	   }

	   if(toknumber==1)
	   {
			 f_error(NORERR,21);
			 return(ERROR);
	   }

	   if(toknumber!=2)
	   {
			 f_error(NORERR,22);
			 return(ERROR);
	   }

	   a=f_getvalue(tokarray[1],&c);
	   if(c==NOTVALUE) f_error(NORERR,23);
	   if(c!=1) return(ERROR);											   /* BIN,DEC,HEXAOK*/

	   p1=equvalueP+(equcounter*19);
	   strncpy(p1,tokarray[1],20);
	   equcounter++;
	   return(NOTERROR);
}

int f_equcheck(char *equstrP,char *errorreturn)
{
	   char *p1,*p2;
	   int a,b;

	   char c1;

	   p1=(char *)equP;
	   p2=(char *)equvalueP;
	   for(a=0;a<equcounter;a++)
	   {
			 b=strcmp(p1,equstrP);
			 if(b==NULL)
			 {
				    *errorreturn=BEEQU;
				    a=f_getvalue(p2,&c1);
				    return(a);
			 }

			 p1=p1+21;
			 p2=p2+19;
	   }

	   *errorreturn=NOTEQU;
	   return(ERROR);
}

int f_gethexa(char *hexaP,char *errorreturn)
{
	   int a;

	   if(*hexaP!='0')
	   {
			 *errorreturn=NOTHEXA;
			 return(ERROR);
	   }

	   hexaP++;

	   if(*hexaP!='x')
	   {
			 *errorreturn=NOTHEXA;
			 return(ERROR);
	   }

	   hexaP++;

	   if(*hexaP==NULL)
	   {
			 *errorreturn=NULLHEXA;
			 return(ERROR);
	   }

	   a=f_strtohexa(hexaP,errorreturn);
	   return(a);
}

int f_getbin(char *binP,char *errorreturn)
{
	   int a;

	   if(*binP!='0')
	   {
			 *errorreturn=NOTBIN;
			 return(ERROR);
	   }

	   binP++;

	   if(*binP!='b')
	   {
			 *errorreturn=NOTBIN;
			 return(ERROR);
	   }

	   binP++;

	   if(*binP==NULL)
	   {
			 *errorreturn=NULLBIN;
			 return(ERROR);
	   }

	   a=f_strtobin(binP,errorreturn);
	   return(a);
}

int f_getvalue(char *strP,char *errorreturn)
{
	   int a;

	   a=f_gethexa(strP,errorreturn);
	   if(*errorreturn==HEXAOK) return(a);
	   if(*errorreturn==BADHEXA)
	   {
			 f_error(NORERR,43);
			 return(ERROR);
	   }

	   if(*errorreturn==BIGHEXA)
	   {
			 f_error(NORERR,25);
			 return(ERROR);
	   }

	   if(*errorreturn==NULLHEXA)
	   {
			 f_error(NORERR,24);
			 return(ERROR);
	   }

	   a=f_getbin(strP,errorreturn);
	   if(*errorreturn==BINOK) return(a);
	   if(*errorreturn==BADBIN)
	   {
			 f_error(NORERR,42);
			 return(ERROR);
	   }

	   if(*errorreturn==BIGBIN)
	   {
			 f_error(NORERR,27);
			 return(ERROR);
	   }

	   if(*errorreturn==NULLBIN)
	   {
			 f_error(NORERR,26);
			 return(ERROR);
	   }

	   a=f_strtodec(strP,errorreturn);
	   if(*errorreturn==DECOK) return(a);

	   if(*errorreturn==BIGDEC)
	   {
			 f_error(NORERR,28);
			 return(ERROR);
	   }

	   *errorreturn=NOTVALUE;
	   return(ERROR);
}

void f_valuetostr(char *bufferP,int value,int strtype)
{
	   char tmpbuffer[20];
	   char *p1;

	   int a;

	   itoa(value,&tmpbuffer[0],strtype);

	   switch(strtype)
	   {
			 case BINSTR :
			 {
				    p1="0b";
				    break;
			 }

			 case DECSTR :
			 {

				    a=strlen(&tmpbuffer[0]);
				    p1=&tmpbuffer[a];
				    break;
			 }

			 case HEXASTR :
			 {
				    p1="0x";
				    break;
			 }
	   }

	   sprintf(bufferP,"%s%s",p1,&tmpbuffer[0]);
}

void f_listtitle(void)
{
	   time_t assemblestarttime;
	   char tmpbuffer[14];

	   char *p1;

	   memset(&tmpbuffer[0],0x20,13);
	   tmpbuffer[13]=NULL;

	   time(&assemblestarttime);
	   p1=ctime(&assemblestarttime);
	   fprintf(fp3,"%s%s%s\n",gf3,&tmpbuffer[0],p1);
}

int f_listoptionset(char *listoptionstrP)
{
	   int a;

	   if(toknumber!=1)
	   {
			 f_error(NORERR,35);
			 return(ERROR);
	   }

	   if(*listoptionstrP==NULL)
	   {
			 f_error(NORERR,36);
			 return(ERROR);
	   }

	   a=f_strarraycmp(&onoptionstrarray[0],listoptionstrP);
	   if(a!=ERROR)
	   {
			 optionsbuffer[a]=ON;
			 return(NOTERROR);
	   }

	   a=f_strarraycmp(&offoptionstrarray[0],listoptionstrP);
	   if(a!=ERROR)
	   {
			 optionsbuffer[a]=OFF;
			 return(NOTERROR);
	   }

	   f_error(NORERR,37);
	   return(ERROR);
}

void f_listprint(int binarycodenumber)
{
	   char tmpbuffer[31];

	   char *p1;

	   int a,b,c;

	   if(optionsbuffer[0]==ON)																						    /* list on off */
	   {
			 memset(&tmpbuffer[0],0x20,6);
			 tmpbuffer[6]=NULL; 																							   /* line */
			 if(optionsbuffer[1]==ON) sprintf(&tmpbuffer[0],"%5u ",line);

			 fprintf(fp3,"%s",&tmpbuffer[0]);

			 memset(&tmpbuffer[0],0x20,7);
			 tmpbuffer[7]=NULL;
			 if(optionsbuffer[2]==ON)																	    /* offset */
			 {
				    if(binarycodenumber!=0 || *tokarray[0]=='_')
				    {
						  a=offset-binarycodenumber;
						  sprintf(&tmpbuffer[0],"0x%04x ",a);
				    }

			 }

			 fprintf(fp3,"%s",&tmpbuffer[0]);

								memset(&tmpbuffer[0],0x20,30);
			 tmpbuffer[30]=NULL;
								if(optionsbuffer[3]==ON) 																	   /* binary code */
			 {
				    if(binarycodenumber<=6) c=binarycodenumber;
												    else c=6;

				    b=0;
				    for(a=0;a<c;a++)
				    {
						  b=b+sprintf(&tmpbuffer[b],"0x%02x ",*(buffer4+a));
				    }

				    if(b!=30) tmpbuffer[b]=0x20;
			 }

			 fprintf(fp3,"%s",&tmpbuffer[0]);

			 if(optionsbuffer[4]==ON) p1=buffer1;						/* source */
									   else p1=buffer2_1;

			 fprintf(fp3,"%s\n",p1);

			 if(optionsbuffer[3]==ON && binarycodenumber>6)	/* binary code-1 */
			 {
				    a=6;
				    while(1)
				    {
						  memset(&tmpbuffer[0],0x20,13);
						  tmpbuffer[13]=NULL;
						  fprintf(fp3,"%s",&tmpbuffer[0]);
						  b=0;
						  for(c=0;c<6;c++)
						  {
								b=b+sprintf(&tmpbuffer[b],"0x%02x ",*(buffer4+a));
								a++;
								if(a==binarycodenumber)
								{
									   a=0;
									   break;
								}
						  }

						  fprintf(fp3,"%s\n",&tmpbuffer[0]);
						  if(a==0) break;
				    }

			 }

	   }

}

void f_listotherinformation(void)
{
	   char tmpbuffer[21];

	   char *p1,*p2;

	   int *p3;

	   int a,b;

	   fputc('\n',fp3);
	   fprintf(fp3," Label Name            offset\n");

	   p1=labelP;
	   p3=labeloffsetP;
	   for(a=0;a<labelcounter;a++)
	   {
			 b=sprintf(&tmpbuffer[0],"%s",p1);
			 while(b!=20)
			 {
				    tmpbuffer[b]='.';
				    b++;
			 }

			 tmpbuffer[b]=NULL;
			 fprintf(fp3," %s  0x%04x\n",&tmpbuffer[0],*p3);
			 p1=p1+21;
			 p3++;
	   }

			  fputc('\n',fp3);
	   fprintf(fp3," Equ Name              value\n");

	   p1=equP;
	   p2=equvalueP;
	   for(a=0;a<equcounter;a++)
	   {
			 b=sprintf(&tmpbuffer[0],"%s",p1);
			 while(b!=20)
			 {
				    tmpbuffer[b]='.';
				    b++;
			 }

			 tmpbuffer[b]=NULL;
			 fprintf(fp3," %s  %s\n",&tmpbuffer[0],p2);
			 p1=p1+21;
			 p2=p2+19;
	   }

	   fputc('\n',fp3);
	   fprintf(fp3," total label   %u defined\n",labelcounter);
	   fprintf(fp3," total equ     %u defined\n",equcounter);
	   fprintf(fp3," total source  %u line\n",sourceline);
	   fprintf(fp3," total code    %u line\n",codeline);

	   fputc('\n',fp3);
	   fprintf(fp3," total code    %u byte\n",codesize-datasize);
	   fprintf(fp3," total data    %u byte\n",datasize);

	   fputc('\n',fp3);
	   fprintf(fp3,"%5u Warning(s)\n",warningcounter);
	   fprintf(fp3,"%5u Error(s)\n\n",errorcounter);
}

int f_opcode(int passtype)
{
	   int a,b;

	   a=tokcounter;
	   b=f_operand(passtype);
	   if(b==ERROR) return(ERROR);
	   tokcounter=a;

	   switch(*tokarray[tokcounter])
	   {
			 case 'a' : return(f_aopcode());
			 case 'c' : return(f_copcode(passtype));
			 case 'd' : return(f_dopcode(passtype));
			 case 'e' : return(f_eopcode());
			 case 'h' : return(f_hopcode());
			 case 'i' : return(f_iopcode());
			 case 'j' : return(f_jopcode(passtype));
			 case 'l' : return(f_lopcode(passtype));
			 case 'm' : return(f_mopcode());
			 case 'n' : return(f_nopcode());
			 case 'o' : return(f_oopcode());
			 case 'p' : return(f_popcode(passtype));
			 case 'r' : return(f_ropcode());
			 case 's' : return(f_sopcode());
			 case 't' : return(f_topcode());
			 case 'w' : return(f_wopcode());
			 case 'x' : return(f_xopcode());
			 default :
			 {
				    f_error(NORERR,46);
				    return(ERROR);
			 }

	   }

}

int f_operand(int passtype)
{
	   int a;

	   tokcounter++;
	   if(tokcounter==toknumber)
	   {
			 *(buffer5+0)=_0OPER;
			 *(buffer5+1)=NOT;
			 return(NOTERROR);
	   }

	   a=f_suboperand(buffer5+1,passtype);
	   if(a==ERROR) return(ERROR);
	   if(*(buffer5+2)==STRING) return(NOTERROR);

			  tokcounter++;
	   if(tokcounter==toknumber)
	   {
			 *(buffer5+0)=_1OPER;
			 *(buffer5+5)=NOT;
			 return(NOTERROR);
	   }

	   a=strcmp(tokarray[tokcounter],",");
	   if(a!=NULL)
	   {
			 f_error(NORERR,41);
			 return(ERROR);
	   }

			  tokcounter++;
	   if(tokcounter==toknumber)
	   {
			 f_error(NORERR,41);
			 return(ERROR);
	   }

	   a=f_suboperand(buffer5+5,passtype);
	   if(a==ERROR) return(ERROR);

	   tokcounter++;
	   if(tokcounter==toknumber)
	   {
			 *(buffer5+0)=_2OPER;
			 return(NOTERROR);
	   }

	   f_error(NORERR,41);
	   return(ERROR);
}

int f_suboperand(char *buffer,int passtype)
{
	   int a;

	   char c1;

	   a=f_strarraycmp(&auxstr[0],tokarray[tokcounter]);
	   if(a!=ERROR)
	   {
			 *(buffer+0)=a;
			 tokcounter++;
			 if(tokcounter==toknumber)
			 {
				    f_error(NORERR,41);
				    return(ERROR);
			 }

	   }
	   else
	   {
			 *(buffer+0)=NOT;
	   }

	   a=f_operandregister(buffer+1,tokarray[tokcounter]);
	   if(a!=ERROR) return(a);

	   if(*tokarray[tokcounter]=='[')
			 return(f_operandmemory(buffer+1,passtype));

	   if(*tokarray[tokcounter]=='_')
			 return(f_operandlabel(buffer+1,tokarray[tokcounter],passtype));

	   if(*tokarray[tokcounter]=='&')
			 return(f_operandoffset(buffer+1,tokarray[tokcounter],passtype));

	   if(*tokarray[tokcounter]=='!')
			 return(f_operandequ(buffer+1,tokarray[tokcounter]));

	   if(*tokarray[tokcounter]=='"')
			 return(f_operandstr(buffer+1));

	   if(*tokarray[tokcounter]=='\'')
			 return(f_operandstr(buffer+1));

	   return(f_operandvalue(buffer+1,tokarray[tokcounter]));
}

int f_operandregister(char *buffer,char *strP)
{
	   int a;

	   a=f_strarraycmp(&segmentregister[0],strP);
	   if(a!=ERROR)
	   {
			 *(buffer+0)=SREG;
			 *(buffer+1)=a;
			 return(NOTERROR);
	   }

	   a=f_strarraycmp(&generalregister8bit[0],strP);
	   if(a!=ERROR)
	   {
			 *(buffer+0)=_8REG;
			 *(buffer+1)=a;
			 return(NOTERROR);
	   }

	   a=f_strarraycmp(&generalregister16bit[0],strP);
	   if(a!=ERROR)
	   {
			 *(buffer+0)=_16REG;
			 *(buffer+1)=a;
			 return(NOTERROR);
	   }

	   return(ERROR);
}

int f_operandmemory(char *buffer,int passtype)
{
	   int a,b,c;

	   char memoryflagbuffer[4];																					 /* bx bp si di */

	   tokcounter++;																													   /* check '[' */
	   if(tokcounter==toknumber)
	   {
			 f_error(NORERR,41);
			 return(ERROR);
	   }

	   memset(&memoryflagbuffer[0],NULL,4); 													  /* buffer clear */
	   while(1)
	   {
			 c=0;

			 while(1) 																																/* reg check */
			 {
				    a=f_strarraycmp(&basememoryregister[0],tokarray[tokcounter]);
				    if(a==ERROR) break;																			 /* not reg */
				    memoryflagbuffer[a]++;																   /* base reg inc */

				    tokcounter++;
				    if(tokcounter==toknumber)
				    {
						  f_error(NORERR,41);
						  return(ERROR);
				    }

				    a=strcmp("+",tokarray[tokcounter]);
				    if(a!=NULL)
				    {
						  a=strcmp("]",tokarray[tokcounter]);
						  if(a!=NULL)
						  {
								f_error(NORERR,41);
								return(ERROR);
						  }

						  c++;
						  break;																									/* base or index */
				    }

				    tokcounter++;
				    if(tokcounter==toknumber)
				    {
						  f_error(NORERR,41);
						  return(ERROR);
				    }

				    b=f_strarraycmp(&basememoryregister[0],tokarray[tokcounter]);
				    if(b==ERROR) break;																			 /* base+DISP?? */

				    if(a==2 || a==3 || b==0 || b==1 )					/* index+?? */
				    {																															    /* ??+base */
						  f_error(NORERR,41);
						  return(ERROR);
				    }

				    memoryflagbuffer[b]++;																   /* index reg inc */

				    tokcounter++;
				    if(tokcounter==toknumber)
				    {
						  f_error(NORERR,41);
						  return(ERROR);
				    }

				    a=strcmp("+",tokarray[tokcounter]);
				    if(a!=NULL)
				    {
						  a=strcmp("]",tokarray[tokcounter]);
						  if(a!=NULL)
						  {
								f_error(NORERR,41);
								return(ERROR);
						  }

						  c++;
						  break;																									/* base+index */
				    }

											  tokcounter++;
				    if(tokcounter==toknumber)
				    {
						  f_error(NORERR,41);
						  return(ERROR);
				    }

				    a=f_strarraycmp(&basememoryregister[0],tokarray[tokcounter]);
				    if(a!=ERROR)												 /* base+index+reg */
				    {
						  f_error(NORERR,41);
						  return(ERROR);
				    }

				    break;																													 /* base+index+?? */
			 }

			 *(buffer+0)=NULLBLANK;
			 *(buffer+1)=NULL;
			 *(buffer+2)=NULL;
			 if(c!=0) break;

			 switch(*tokarray[tokcounter])
			 {
				    case '!' :
				    {
						  a=f_operandequ(buffer,tokarray[tokcounter]);
						  if(a==ERROR) return(ERROR);
						  break;
				    }

				    case '&' :
				    {
						  a=f_operandoffset(buffer,tokarray[tokcounter],passtype);
						  if(a==ERROR) return(ERROR);
						  *(buffer+0)=_16IMM;
						  break;
				    }

				    default :
				    {
						  a=f_operandvalue(buffer,tokarray[tokcounter]);
						  if(a==ERROR) return(ERROR);
						  if(*(buffer+1)>0x7f)
						  {
							 if(*(buffer+0)==_8IMM)
							 {
								*(buffer+0)=_16IMM;
							 }
							 else
							 {
								if(*(buffer+2)==0xff) *(buffer+0)=_8IMM;
							 }

						  }

						  break;
				    }

			 }

			 tokcounter++;
			 if(tokcounter==toknumber)
			 {
				    f_error(NORERR,41);
				    return(ERROR);
			 }

			 a=strcmp("]",tokarray[tokcounter]);
			 if(a!=NULL)
			 {
				    f_error(NORERR,41);
				    return(ERROR);
			 }

			 break;
	   }

	   while(1)
	   {
			 if(memoryflagbuffer[0]!=NULL)							/* bx */
			 {
				    while(1)
				    {
						  if(memoryflagbuffer[2]!=NULL)				/* bx+si */
						  {
								a=1;
								if(*(buffer+0)==NULLBLANK) break;
								a++;
								if(*(buffer+0)==_16IMM) a++;
								break;
						  }

						  if(memoryflagbuffer[3]!=NULL)				/* bx+di */
						  {
								a=4;
								if(*(buffer+0)==NULLBLANK) break; /* not value */
								a++;
								if(*(buffer+0)==_16IMM) a++;
								break;
						  }

						  a=0;
						  if(*(buffer+0)==NULLBLANK) break;
						  a=7;
						  if(*(buffer+0)==_16IMM) a++;
						  break;
				    }

				    break;
			 }

			 if(memoryflagbuffer[1]!=NULL)							/* bp */
			 {
				    while(1)
				    {
						  if(memoryflagbuffer[2]!=NULL)				/* bp+si */
						  {
								a=10;
								if(*(buffer+0)==NULLBLANK) break;
								a++;
								if(*(buffer+0)==_16IMM) a++;
								break;
						  }

						  if(memoryflagbuffer[3]!=NULL)				/* bp+di */
						  {
								a=13;
								if(*(buffer+0)==NULLBLANK) break;
								a++;
								if(*(buffer+0)==_16IMM) a++;
								break;
						  }

						  a=9;
						  if(*(buffer+0)==NULLBLANK)
						  {
								if(passtype==_1PASS) f_error(WARNING,4);
								if(passtype==_2PASS) f_error(WARNING,5);
								break;
						  }

						  a=16;
						  if(*(buffer+0)==_16IMM) a++;
						  break;
				    }

				    break;
			 }

			 if(memoryflagbuffer[2]!=NULL)							/* si */
			 {
				    while(1)
				    {
						  a=18;
						  if(*(buffer+0)==NULLBLANK) break;
						  a++;
						  if(*(buffer+0)==_16IMM) a++;
						  break;
				    }

				    break;
			 }

			 if(memoryflagbuffer[3]!=NULL)							/* di */
			 {
				    while(1)
				    {
						  a=21;
						  if(*(buffer+0)==NULLBLANK) break;
						  a++;
						  if(*(buffer+0)==_16IMM) a++;
						  break;
				    }

				    break;
			 }

			 a=24;																															  /* direct */
			 break;
	   }

	   *(buffer+0)=basememorytype[a];
	   return(NOTERROR);
}

int f_operandlabel(char *buffer,char *strP,int passtype)
{
	   int a;

	   char c1;

	   if(passtype==_1PASS)
	   {
			 *(buffer+0)=LABEL;
			 *(buffer+1)=NULL;
			 *(buffer+2)=NULL;
			 return(NOTERROR);
	   }
	   else
	   {
			 a=f_labelcheck(strP+1,&c1);
			 if(c1==NOTLABEL)
			 {
				    f_error(NORERR,45);
				    return(ERROR);
			 }

			 *(buffer+0)=LABEL;
			 c1=a;
			 *(buffer+1)=c1;
			 a=a&0xff00;
			 a=a>>8;
			 c1=a;
			 *(buffer+2)=c1;
			 return(NOTERROR);
	   }

}

int f_operandoffset(char *buffer,char *strP,int passtype)
{
	   int a;

	   char c1;

	   if(passtype==_1PASS)
	   {
			 *(buffer+0)=OFFSET;
			 *(buffer+1)=NULL;
			 *(buffer+2)=NULL;
			 return(NOTERROR);
	   }
	   else
	   {
			 a=f_labelcheck(strP+1,&c1);
			 if(c1==NOTLABEL)
			 {
				    f_error(NORERR,45);
				    return(ERROR);
			 }

			 *(buffer+0)=OFFSET;
			 c1=a;
			 *(buffer+1)=c1;
			 a=a&0xff00;
			 a=a>>8;
			 c1=a;
			 *(buffer+2)=c1;
			 return(NOTERROR);
	   }

}

int f_operandequ(char *buffer,char *strP)
{
	   char tmpbuffer[7];

	   int a;

	   char c1;

	   a=f_equcheck(strP+1,&c1);
	   if(c1==NOTEQU)
	   {
			 f_error(NORERR,44);
			 return(ERROR);
	   }

	   f_valuetostr(&tmpbuffer[0],a,HEXASTR);
	   f_operandvalue(buffer,&tmpbuffer[0]);
	   return(NOTERROR);
}

int f_operandstr(char *buffer)
{
	   *(buffer+0)=STRING;
	   return(NOTERROR);
}

int f_operandvalue(char *buffer,char *strP)
{
	   int a,b;

	   char c1;

	   a=f_getvalue(strP,&c1);
	   if(c1!=1)																																			  /* 0b 0 0x ok ? */
	   {
			 f_error(NORERR,41);
			 return(ERROR);
	   }

	   b=a&0xff00;
	   b=b>>8;
	   if(b==0)
	   {
			 *(buffer+0)=_8IMM;
			 c1=a;
			 *(buffer+1)=c1;
			 *(buffer+2)=NULL;
	   }
	   else
	   {
			 *(buffer+0)=_16IMM;
			 c1=a;
			 *(buffer+1)=c1;
			 a=a&0xff00;
			 a=a>>8;
			 c1=a;
			 *(buffer+2)=c1;
	   }

	   return(NOTERROR);
}

int f_aopcode(void)
{
	   int a;

	   a=f_strarraycmp(&a_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=1) return(f_zerooperandcode(a_binarycode[a]));    /* aaa aas */
	   if(a<=3) return(f_aad_aamopcode(a_binarycode[a]));			  /* aad aam */

	   return(f_twooperandcode(a_binarycode[a]));						/* adc add and */
}

int f_copcode(int passtype)
{
	   int a;

	   a=f_strarraycmp(&c_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   } 																																						 /* cbw clc cld */
																				    /* cli cmc cmpsb */
	   if(a<=8) return(f_zerooperandcode(c_binarycode[a]));    /* cmpsw cs: cwd */
																				    /* call */
	   if(a==9) return(f_resetIPoperandcode(passtype,0x10,0x18,0xe8,0x9a));

	   return(f_twooperandcode(0x38)); 														   /* cmp */
}

int f_dopcode(int passtype)
{
	   int a;

	   a=f_strarraycmp(&d_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=2) return(f_zerooperandcode(d_binarycode[a]));    /* daa das ds: */

	   if(a==3) return(f_decopcode()); 																/* dec */
																				    /* div */
	   if(a==4) return(f_oneoperandcode(buffer5+1,0xf6,0x30));

	   if(a==5) return(f_dbopcode());																		   /* db */

	   if(a==6) return(f_dsopcode());																		   /* ds */

	   if(a==7) return(f_duopcode(passtype));											    /* du */

	   if(a==8) return(f_dwopcode());																		   /* dw */
}

int f_eopcode(void)
{
	   int a;

	   a=f_strarraycmp(&e_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a==0) return(f_zerooperandcode(e_binarycode[a]));    /* es: */

	   f_error(WARNING,17);												   /* esc */
	   return(ERROR);
}

int f_hopcode(void)
{
	   int a;

	   a=f_strarraycmp(&h_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   return(f_zerooperandcode(h_binarycode[a]));				  /* hlt */
}

int f_iopcode(void)
{
	   int a;

	   a=f_strarraycmp(&i_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=1) return(f_zerooperandcode(i_binarycode[a]));    /* into iret */

	   if(a==2) return(f_intopcode()); 																/* int */

	   if(a==3) return(f_incopcode()); 																/* inc */
																				    /* imul */
	   if(a==4) return(f_oneoperandcode(buffer5+1,0xf6,0x28));
																				    /* idiv */
	   if(a==5) return(f_oneoperandcode(buffer5+1,0xf6,0x38));

	   if(a==6) return(f_inopcode());																		   /* in */
}

int f_jopcode(int passtype)
{
	   int a;

	   a=f_strarraycmp(&j_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=28) return(f_shortlabeloperandcode(passtype,j_binarycode[a]));
																				    /* j??? */
	   return(f_jmpopcode(passtype));																		   /* jmp */
}

int f_lopcode(int passtype)
{
	   int a;

	   a=f_strarraycmp(&l_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=3) return(f_zerooperandcode(l_binarycode[a]));    /* lahf lock */
																				    /* lodsb lodsw */
	   if(a<=8) return(f_shortlabeloperandcode(passtype,l_binarycode[a]));
																				    /* loop?? */
	   return(f_loadaddress(passtype,l_binarycode[a]));					/* lds lea les */
}

int f_mopcode(void)
{
	   int a;

	   a=f_strarraycmp(&m_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=1) return(f_zerooperandcode(m_binarycode[a]));    /* movsb movsw */
																				    /* mul */
	   if(a==2) return(f_oneoperandcode(buffer5+1,0xf6,0x20));
	   return(f_movopcode());											   /* mov */
}

int f_nopcode(void)
{
	   int a;

	   a=f_strarraycmp(&n_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a==0) return(f_zerooperandcode(n_binarycode[a]));    /* nop */
																				    /* neg */
	   if(a==1) return(f_oneoperandcode(buffer5+1,0xf6,0x18));
																				    /* not */
	   if(a==2) return(f_oneoperandcode(buffer5+1,0xf6,0x10));
}

int f_oopcode(void)
{
	   int a;

	   a=f_strarraycmp(&o_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a==0) return(f_orgopcode()); 								/* org */

	   if(a==1) return(f_twooperandcode(0x08));							/* or */

	   if(a==2) return(f_outopcode()); 								/* out */
}

int f_popcode(int passtype)
{
	   int a;

	   a=f_strarraycmp(&p_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a<=1) return(f_zerooperandcode(p_binarycode[a]));    /* popf pushf */

	   if(a<=3) return(f_push_popopcode(passtype,a)); 			  /* pop push */
}

int f_ropcode(void)
{
	   int a,b;

	   a=f_strarraycmp(&r_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }
																				    /* rep repe repz */
	   if(a<=4) return(f_zerooperandcode(r_binarycode[a]));    /* repne repnz */
	   if(a<=6) return(f_ret_retfopcode(r_binarycode[a]));     /* ret retf */
																				    /* rcl rcr rol */
	   return(f_rotateshiftoperandcode(0xd0,r_binarycode[a]));
																				    /* ror */
}

int f_sopcode(void)
{
	   int a;

	   a=f_strarraycmp(&s_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);																						/* sahf scansb */
			 return(ERROR);																								   /* scasw stc std */
	   } 																																							   /* sti stosb */
																																														   /* stosw ss: */
	   if(a<=8) return(f_zerooperandcode(s_binarycode[a]));    /* sar shl sal */
																				    /* shr */
	   if(a<=12) return(f_rotateshiftoperandcode(0xd0,s_binarycode[a]));

	   return(f_twooperandcode(s_binarycode[a]));						/* sbb sub */
}

int f_topcode(void)
{
	   int a;

	   a=f_strarraycmp(&t_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   return(f_testopcode());											   /* test */
}

int f_wopcode(void)
{
	   int a;

	   a=f_strarraycmp(&w_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   return(f_zerooperandcode(w_binarycode[a]));				  /* wait */
}

int f_xopcode(void)
{
	   int a;

	   a=f_strarraycmp(&x_opcode[0],tokarray[tokcounter]);
	   if(a==ERROR)
	   {
			 f_error(NORERR,46);
			 return(ERROR);
	   }

	   if(a==0) return(f_zerooperandcode(x_binarycode[a]));    /* xlat */

	   if(a==1) return(f_xchgopcode());									   /* xchg */

	   if(a==2) return(f_twooperandcode(0x30));							/* xor */
}

int f_aad_aamopcode(char binarycode)
{
	   int a;

	   a=f_zerooperandcode(binarycode);
	   if(a==ERROR) return(ERROR);
	   *(buffer4+1)=0x0a;														 /* aad aam */
	   return(2);
}

int f_decopcode(void)
{
	   if(*(buffer5+2)==_16REG) return(f_register16operandcode(0x48));
	   return(f_oneoperandcode(buffer5+1,0xfe,0x08));
}

int f_dbopcode(void)
{
	   if(*(buffer5+0)!=_1OPER)
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8IMM)
	   {
			 f_error(NORERR,88);
			 return(ERROR);
	   }

	   *(buffer4+0)=*(buffer5+3);
	   datasize++;
	   return(1);
}

int f_dsopcode(void)
{
	   char *p1;

	   int a;

	   char c1,c2;

	   if(*(buffer5+2)!=STRING)
	   {
			 f_error(NORERR,92);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }


	   p1=strpbrk(buffer2,"'\"");
	   c1=*p1;

	   a=f_ascstr2codestr(buffer4,p1,&editchararray[0],"\t ",&c2,c1,'\\');

	   if(c2==STROK)
	   {
			 datasize=datasize+a;
			 return(a);
	   }

	   if(c2==NOTDIVSTR)
	   {
			 if(c1=='"') f_error(NORERR,94);
						  else f_error(NORERR,93);

			 return(ERROR);
	   }

	   f_error(NORERR,95);																											/* STROVER */
	   return(ERROR);
}

int f_duopcode(int passtype)
{
	   int a;

	   if(*(buffer5+0)==_0OPER)
	   {
			 f_error(NORERR,90);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+2)>=SREG)
	   {
			 f_error(NORERR,52);
			 return(ERROR);
	   }

	   a=0;
	   a=*(buffer5+4);
	   a=a<<8;
	   a=a|*(buffer5+3);

	   if(*(buffer5+0)==_1OPER)
	   {
			 if(passtype==_1PASS) f_error(WARNING,15);
								 else f_error(WARNING,16);

			 return(a);
	   }

	   if(*(buffer5+5)!=NOT)
	   {
			 f_error(NORERR,74);
			 return(ERROR);
	   }

	   if(*(buffer5+6)!=_8IMM)
	   {
			 f_error(NORERR,91);
			 return(ERROR);
	   }

	   if(a>1000)
	   {
			 f_error(NORERR,96);
			 return(ERROR);
	   }

	   memset(buffer4,*(buffer5+7),a);
	   datasize=datasize+a;
	   return(a);
}

int f_dwopcode(void)
{
	   if(*(buffer5+0)!=_1OPER)
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=OFFSET && *(buffer5+2)>=SREG)
	   {
			 f_error(NORERR,89);
			 return(ERROR);
	   }

	   *(buffer4+0)=*(buffer5+3);
	   *(buffer4+1)=*(buffer5+4);
	   datasize=datasize+2;
	   return(2);
}

int f_inopcode(void)
{
	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8REG && *(buffer5+2)!=_16REG)
	   {
			 f_error(NORERR,66);
			 return(ERROR);
	   }

	   if(*(buffer5+3)!=0)																											/* must al or ax */
	   {
			 f_error(NORERR,67);
			 return(ERROR);
	   }

	   if(*(buffer5+5)!=NOT) 																						    /* 2 operand */
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+6)!=_8IMM && *(buffer5+6)!=_16REG)
	   {
			 f_error(NORERR,68);
			 return(ERROR);
	   }

	   if(*(buffer5+6)==_16REG)																						    /* in ??,dx */
	   {
			 if(*(buffer5+7)!=2)																						/* must dx */
			 {
				    f_error(NORERR,69);
				    return(ERROR);
			 }

			 if(*(buffer5+2)==_8REG) *(buffer4+0)=0xec;
											 else *(buffer4+0)=0xed;

			 return(1);
	   }

	   if(*(buffer5+2)==_8REG) *(buffer4+0)=0xe4;
									   else *(buffer4+0)=0xe5;

	   *(buffer4+1)=*(buffer5+7); 																					    /* imm8 */
	   return(2);
}

int f_incopcode(void)
{
	   if(*(buffer5+2)==_16REG) return(f_register16operandcode(0x40));
	   return(f_oneoperandcode(buffer5+1,0xfe,0x00));
}

int f_intopcode(void)
{
	   if(*(buffer5+0)!=_1OPER)																		 /* operand is */
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT) 						    /* syntax is */
	   {
			 f_error(NORERR,52);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==_16IMM)																		 /* bigger than */
	   {
			 f_error(NORERR,56);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8IMM)																		 /* 1oper only */
	   {
			 f_error(NORERR,97);
			 return(ERROR);
	   }

	   if(*(buffer5+3)==0x03)
	   {
			 *(buffer4+0)=0xcc;
			 return(1);
	   }

	   *(buffer4+0)=0xcd;
	   *(buffer4+1)=*(buffer5+3);
	   return(2);
}

int f_jmpopcode(int passtype)
{
	   if(*(buffer5+1)==SHORT) return(f_shortlabeloperandcode(passtype,0xeb));

	   return(f_resetIPoperandcode(passtype,0x20,0x21,0xe9,0xea));
}

int f_movopcode(void)
{
	   int a;

	   char c1;

	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==SREG)											   /* mov sreg,?? */
	   {
			 if(*(buffer5+1)!=NOT)
			 {
				    f_error(NORERR,101);
				    return(ERROR);
			 }

			 if(*(buffer5+6)!=_16REG && *(buffer5+6)!=LABEL && *(buffer5+6)<=STRING)
			 {
				    f_error(NORERR,102);
				    return(ERROR);
			 }

			 if(*(buffer5+6)==_16REG)
			 {
				    if(*(buffer5+5)!=NOT)
				    {
						  f_error(NORERR,74);
						  return(ERROR);
				    }

			 }
			 else
			 {
				    if(*(buffer5+5)==NOT)
				    {
						  f_error(NORERR,103);
						  return(ERROR);
				    }

				    if(*(buffer5+5)==BYTE)
				    {
						  f_error(NORERR,65);
						  return(ERROR);
				    }

			 }

			 *(buffer5+0)=_1OPER;										   /* flag setting */
			 c1=*(buffer5+3);
			 c1=c1<<3;
			 a=f_oneoperandcode(buffer5+5,0x8e,c1);
			 if(a==ERROR) return(ERROR);
			 *(buffer4+0)=*(buffer4+0)&0xfe;							/* code reset */
			 return(a);
	   }

	   if(*(buffer5+2)==_8REG || *(buffer5+2)==_16REG)
	   {
			 if(*(buffer5+6)==OFFSET) *(buffer5+6)=_16IMM;	/* code reset */

			 if(*(buffer5+6)<=_16IMM)
			 {
				    if(*(buffer5+2)==_8REG && *(buffer5+6)==_16IMM)
				    {
						  f_error(NORERR,81);
						  return(ERROR);
				    }

				    c1=*(buffer5+2)-_8REG;
				    c1=c1<<3;
				    *(buffer4+0)=0xb0|c1|*(buffer5+3);
				    *(buffer4+1)=*(buffer5+7);
				    if(c1==0) return(2);
				    *(buffer4+2)=*(buffer5+8);
				    return(3);
			 }

			 if(*(buffer5+6)==SREG)
			 {
				    if(*(buffer5+1)!=_16REG)
				    {
						f_error(NORERR,85);
						return(ERROR);
				    }

				    if(*(buffer5+5)!=NOT)
				    {
						  f_error(NORERR,74);
						  return(ERROR);
				    }

				    *(buffer5+0)=_1OPER; 								   /* flag setting */
				    c1=*(buffer5+7);
				    c1=c1<<3;
				    a=f_oneoperandcode(buffer5+1,0x8c,c1);
				    if(a==ERROR) return(ERROR);
				    *(buffer4+0)=*(buffer4+0)&0xfe;						   /* code reset */
				    return(a);
			 }

			 return(f_twooperandcode(0x88));
	   }

	   if(*(buffer5+6)==SREG)
	   {
			if(*(buffer5+1)!=WORD)
			{
				f_error(NORERR,103);
				return(ERROR);
			}

			 if(*(buffer5+5)!=NOT)
			 {
				    f_error(NORERR,74);
				    return(ERROR);
			 }

			 *(buffer5+0)=_1OPER;									/* flag setting */
			 c1=*(buffer5+7);
			 c1=c1<<3;
			 a=f_oneoperandcode(buffer5+1,0x8c,c1);
			 if(a==ERROR) return(ERROR);
			 *(buffer4+0)=*(buffer4+0)&0xfe;							/* code reset */
			 return(a);
	   }

	   a=f_twooperandcode(0x00);
	   if(a==ERROR) return(ERROR);

	   if(*(buffer5+6)<=_16IMM)
	   {
			 c1=*(buffer4+0)&0x01;
			 *(buffer4+0)=c1|0xc6;
	   }
	   else
	   {
			 *(buffer4+0)=*(buffer4+0)|0x88;
	   }

	   return(a);
}

int f_orgopcode(void)
{
	   if(f_valueoperandcode(&offset)==ERROR) return(ERROR);
	   return(0);
}

int f_outopcode(void)
{
	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8IMM && *(buffer5+2)!=_16REG)
	   {
			 f_error(NORERR,70);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==_16REG)																						    /* if dx ? */
	   {
			 if(*(buffer5+3)!=2)
			 {
				    f_error(NORERR,73);
				    return(ERROR);
			 }

	    }

			  if(*(buffer5+5)!=NOT)																									 /* 2 operand */
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+6)!=_8REG && *(buffer5+6)!=_16REG)
	   {
			 f_error(NORERR,71);
			 return(ERROR);
	   }

	   if(*(buffer5+7)!=0)
	   {
			 f_error(NORERR,72);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==_16REG)																						    /* out dx,acc */
	   {
			 if(*(buffer5+6)==_8REG) *(buffer4+0)=0xee;
											 else *(buffer4+0)=0xef;

			 return(1);
	   }

	   if(*(buffer5+6)==_8REG) *(buffer4+0)=0xe6;
									   else *(buffer4+0)=0xe7;

	   *(buffer4+1)=*(buffer5+3);
	   return(2);
}

int f_push_popopcode(int passtype,int codetype)
{
	   int a;

	   int c1;

	   if(*(buffer5+2)==_8REG)
	   {
			 if(codetype==2) f_error(NORERR,57);							    /* pop */
						   else f_error(NORERR,58);								  /* push */

			 return(ERROR);
	   }

	   if(*(buffer5+2)<=_16IMM)
	   {
			 f_error(NORERR,59);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==OFFSET)
	   {
			 f_error(NORERR,61);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==STRING)
	   {
			 f_error(NORERR,62);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==SREG)																								  /* sreg */
	   {
								if(*(buffer5+0)!=_1OPER)
			 {
				    f_error(NORERR,48);
				    return(ERROR);
			 }

			 if(*(buffer5+1)!=NOT)
			 {
				    f_error(NORERR,51);
				    return(ERROR);
			 }

			 *(buffer4+0)=0x06; 																							   /* push */
			 if(codetype==2) *(buffer4+0)=*(buffer4+0)|0x01;	/* pop */

			 a=0;
			 a=*(buffer5+3);
			 a=a<<3;
			 *(buffer4+0)=*(buffer4+0)|a;
			 if(*(buffer4+0)==0x0f)																				  /* pop cs */
			 {
				    if(passtype==_1PASS) f_error(WARNING,3);
									    else f_error(WARNING,6);
			 }

			 return(1);
	   }

	   if(*(buffer5+2)==_16REG)
	   {
			 c1=0x50; 																																/* push */
			 if(codetype==2) c1=c1|0x08;														   /* pop */
			 return(f_register16operandcode(c1));									  /* 16 reg */
	   }

	   if(*(buffer5+1)==BYTE)
	   {
			 f_error(NORERR,65);
			 return(ERROR);
	   }

	   if(*(buffer5+1)==NOT) 								  /* p??? only word*/
	   {
			 if(passtype==_1PASS) f_error(WARNING,7);
								 else f_error(WARNING,8);

			 *(buffer5+1)=WORD;
	   }
																				    /* pop push */
	   if(codetype==2) return(f_oneoperandcode(buffer5+1,0x8f,0x00));
					else return(f_oneoperandcode(buffer5+1,0xff,0x30));
}

int f_ret_retfopcode(char binarycode)
{
	   int a;

	   if(*(buffer5+0)==_0OPER) return(f_zerooperandcode(binarycode));

	   if(f_valueoperandcode(&a)==ERROR) return(ERROR);
	   *(buffer4+0)=binarycode&0xfe;
	   *(buffer4+1)=a;
	   a=a&0xff00;
	   a=a>>8;
	   *(buffer4+2)=a;
	   return(3);
}

int f_testopcode(void)
{
	   int a;

	   char c1,c2;

	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==_8REG || *(buffer5+2)==_16REG)			  /* reg,??? */
	   {
			 if(*(buffer5+6)==_8REG || *(buffer5+6)==_16REG)	/* code setup */
			 {
				    c1=*(buffer5+7);
				    *(buffer5+7)=*(buffer5+3);
				    *(buffer5+3)=c1;
			 }

			 if(*(buffer5+6)==OFFSET) *(buffer5+6)=_16IMM;  /* code reset */

			 if(*(buffer5+6)<=_16IMM) c1=0x00;
									   else c1=0x84;

			 a=f_twooperandcode(c1);
			 if(a==ERROR) return(ERROR);

			 if(*(buffer5+6)<=_16IMM)
			 {
				    c1=*(buffer4+0);
				    c1=c1&0x01;																							  /* get W */

				    if(*(buffer5+3)==0) c2=0xa8;												  /* acc,imm */
									   else c2=0xf6;									/* reg,imm */
				    c1=c1|c2;
				    *(buffer4+0)=c1;
			 }
																				    /* code reset */
			 if(*(buffer5+6)>=BXSI) *(buffer4+0)=*(buffer4+0)&0xfd;

			 return(a);
	   }

	   if(*(buffer5+6)!=OFFSET && *(buffer5+6)>=SREG)
	   {
			 f_error(NORERR,100);
			 return(ERROR);
	   }

	   a=f_twooperandcode(0x00);
	   if(a==ERROR) return(ERROR);

	   *(buffer4+0)=*(buffer4+0)|0xf6; 																/* mem,imm?? */
	   return(a);
}

int f_xchgopcode(void)
{
	   char c1;

	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8REG && *(buffer5+2)!=_16REG)
	   {
			 f_error(NORERR,98);
			 return(ERROR);
	   }

	   if(*(buffer5+6)==_8REG || *(buffer5+6)==_16REG)			  /* code setup */
	   {
			 c1=*(buffer5+7);
			 *(buffer5+7)=*(buffer5+3);
			 *(buffer5+3)=c1;
	   }

	   if(*(buffer5+2)==_16REG && *(buffer5+3)==0)						/* if AX ? */
	   {
			 if(*(buffer5+6)==_16REG)																	    /* AX,reg16 */
			 {
				    if(*(buffer5+1)!=NOT && *(buffer5+5)!=NOT)
				    {
						  f_error(NORERR,51);
						  return(ERROR);
				    }

				    *(buffer4+0)=0x91|*(buffer5+7);
				    return(1);
			 }

	   }

	   if(*(buffer5+6)==OFFSET)
	   {
			 f_error(NORERR,83);
			 return(ERROR);
	   }

	   if(*(buffer5+6)<=_16IMM)
	   {
			 f_error(NORERR,99);
			 return(ERROR);
	   }

	   return(f_twooperandcode(0x86));
}

int f_zerooperandcode(char binarycode)															  /* 0 opcode */
{
	   if(*(buffer5+0)!=_0OPER)
	   {
			 f_error(NORERR,47);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   *(buffer4+0)=binarycode;
	   return(1);
}

int f_register16operandcode(char binarycode) 								   /* inc dec */
{																																											  /* push pop */
	   int a;

	   if(*(buffer5+0)!=_1OPER)
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   *(buffer4+0)=binarycode;

	   a=0;
	   a=*(buffer5+3);
	   *(buffer4+0)=*(buffer4+0)|a;
	   return(1);
}

int f_valueoperandcode(int *returnvalue)														  /* ret retf */
{																																											  /* org */
	   if(*(buffer5+0)!=_1OPER)
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,51);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8IMM && *(buffer5+2)!=_16IMM)
	   {
			 f_error(NORERR,52);
			 return(ERROR);
	   }

	   *returnvalue=0;
	   *returnvalue=*(buffer5+4);
	   *returnvalue=*returnvalue<<8;
	   *returnvalue=*returnvalue|*(buffer5+3);
	   return(NOTERROR);
}

int f_shortlabeloperandcode(int passtype,char binarycode) /* short j??? */
{																																											  /* short loop?? */
	   int a,b,c;

	   char c1;

	   if(*(buffer5+0)!=_1OPER)
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_8IMM && *(buffer5+2)!=_16IMM && *(buffer5+2)!=OFFSET)
	   {
			 f_error(NORERR,54);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=SHORT && *(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,53);
			 return(ERROR);
	   }

	   if(*(buffer5+1)==NOT)
	   {
			 if(passtype==_1PASS) f_error(WARNING,11);
								 else f_error(WARNING,12);
	   }

	   if(passtype==_1PASS)
	   {
			 *(buffer4+0)=NULL;
			 *(buffer4+1)=NULL;
			 return(2);
	   }

	   a=0;
	   a=*(buffer5+4);
	   a=a<<8;
	   a=a|*(buffer5+3);

	   b=offset+2;

	   c=a-b;

	   if(b>=a)
	   {
			 if(c<-128)
			 {
				    f_error(NORERR,55);
				    return(ERROR);
			 }
			 else
			 {
				    c1=c;
			 }

	   }
	   else
	   {
			 if(c>127)
			 {
				    f_error(NORERR,55);
				    return(ERROR);
			 }
			 else
			 {
				    c1=c;
			 }

	   }

	   *(buffer4+0)=binarycode;
	   *(buffer4+1)=c1;
	   return(2);
}

int f_resetIPoperandcode(int passtype,char c1,char c2,char c3,char c4)
{
	   int a;

	   if(*(buffer5+0)==_2OPER || *(buffer5+1)==FAR)				  /* far IP */
	   {
			 if(*(buffer5+0)==_2OPER)																	    /* far imm,imm */
			 {
				    if(*(buffer5+2)!=_8IMM && *(buffer5+2)!=_16IMM)
				    {
						  f_error(NORERR,52);
						  return(ERROR);
				    }

				    if(*(buffer5+6)!=_8IMM && *(buffer5+6)!=_16IMM)
				    {
						  f_error(NORERR,76);
						  return(ERROR);
				    }

				    if(*(buffer5+1)!=FAR && *(buffer5+1)!=NOT)
				    {
						  f_error(NORERR,77);
						  return(ERROR);
				    }

				    if(*(buffer5+5)!=NOT)
				    {
						  f_error(NORERR,74);
						  return(ERROR);
				    }

				    if(*(buffer5+1)==NOT)
				    {
						  if(passtype==_1PASS) f_error(WARNING,9);
											  else f_error(WARNING,10);
				    }

				    *(buffer4+0)=c4;
				    *(buffer4+1)=*(buffer5+3);
				    *(buffer4+2)=*(buffer5+4);
				    *(buffer4+3)=*(buffer5+7);
				    *(buffer4+4)=*(buffer5+8);
				    return(5);
			 }

			 if(*(buffer5+2)<=STRING)																	    /* far [mem] */
			 {
				    f_error(NORERR,78);
				    return(ERROR);
			 }

			 *(buffer5+1)=WORD; 																							   /* flag setting */
			 return(f_oneoperandcode(buffer5+1,0xff,c2));
	   }

	   if(*(buffer5+2)==OFFSET || *(buffer5+2)<=_16IMM)					/* near proc */
	   {
				    if(*(buffer5+0)!=_1OPER)
			 {
				    f_error(NORERR,48);
				    return(ERROR);
			 }

			 if(*(buffer5+1)!=NEAR && *(buffer5+1)!=NOT)
			 {
				    f_error(NORERR,79);
				    return(ERROR);
			 }

			 if(*(buffer5+1)==NOT)
			 {
				    if(passtype==_1PASS) f_error(WARNING,13);
									    else f_error(WARNING,14);
			 }

			 if(*(buffer5+2)==OFFSET && passtype==_1PASS)
			 {
				    *(buffer4+0)=NULL;
				    *(buffer4+1)=NULL;
				    *(buffer4+2)=NULL;
				    return(3);
			 }

			 a=0;
			 a=*(buffer5+4);
			 a=a<<8;
			 a=a|*(buffer5+3);
			 a=a-(offset+3);																								   /* get near IP */

			 *(buffer4+0)=c3;
			 *(buffer4+1)=a;
			 a=a&0xff00;
			 a=a>>8;
			 *(buffer4+2)=a;
			 return(3);
	   }

	   if(*(buffer5+2)==_8REG)																						    /* reg ptr 16 */
	   {
			 f_error(NORERR,80);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NEAR && *(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,79);
			 return(ERROR);
	   }

	   if(*(buffer5+1)==NOT)
	   {
			 if(passtype==_1PASS) f_error(WARNING,13);
								 else f_error(WARNING,14);
	   }

	   if(*(buffer5+2)==_16REG) *(buffer5+1)=NOT;									 /* flag setting */
								else *(buffer5+1)=WORD;

	   return(f_oneoperandcode(buffer5+1,0xff,c1));
}

int f_oneoperandcode(char *buffer,char binarycode1,char binarycode2)
{																																											  /* 1 opcode */
	   char c1,c2,c3;

	   if(*(buffer5+0)!=_1OPER)
	   {
			 f_error(NORERR,48);
			 return(ERROR);
	   }

	   if(*(buffer+1)==_8IMM || *(buffer+1)==_16IMM)
	   {
			 f_error(NORERR,59);
			 return(ERROR);
	   }

	   if(*(buffer+1)==SREG)
	   {
			 f_error(NORERR,60);
			 return(ERROR);
	   }

	   if(*(buffer+1)==OFFSET)
	   {
			 f_error(NORERR,61);
			 return(ERROR);
	   }

	   if(*(buffer+1)==STRING)
	   {
			 f_error(NORERR,62);
			 return(ERROR);
	   }

	   if(*(buffer+1)==LABEL) *(buffer+1)=DIRECT;

	   if(*(buffer+1)==_8REG || *(buffer+1)==_16REG)
	   {
			 if(*(buffer+0)!=NOT)
			 {
				    f_error(NORERR,51);
				    return(ERROR);
			 }

			 *(buffer4+0)=binarycode1;
			 if(*(buffer+1)==_16REG) *(buffer4+0)=*(buffer4+0)|0x01;
			 *(buffer4+1)=0xc0|binarycode2|*(buffer+2);
			 return(2);
	   }

	   if(*(buffer+0)==NOT)
	   {
			 f_error(NORERR,63);
			 return(ERROR);
	   }

	   if(*(buffer+0)!=BYTE && *(buffer+0)!=WORD)
	   {
			 f_error(NORERR,64);
			 return(ERROR);
	   }

	   c1=0;
	   c2=0;
	   c3=0;
	   c1=*(buffer+1)-BXSI;
	   c2=c1;

	   c1=c1&0x18; 																															 /* mod */
	   c1=c1<<3;

	   c2=c2&0x07; 																															 /* r/m */

	   if(*(buffer+1)>=BXSI8 && *(buffer+1)<=BX8) c3=1;					/* value */
	   if(*(buffer+1)>=BXSI16 && *(buffer+1)<=BX16) c3=2;
	   if(*(buffer+1)==DIRECT) c3=2;

	   *(buffer4+0)=binarycode1;
	   if(*(buffer+0)==WORD) *(buffer4+0)=*(buffer4+0)|0x01;
	   *(buffer4+1)=c1|binarycode2|c2;

	   if(c3==0) return(2);
	   *(buffer4+2)=*(buffer+2);																					 /* value ret */
	   if(c3==1) return(3);
	   *(buffer4+3)=*(buffer+3);
	   return(4);
}

int f_twooperandcode(char binarycode)
{
	   int a;

	   char c1,c2;

	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+2)==_8REG || *(buffer5+2)==_16REG)			  /* acc,imm */
	   {
			 if(*(buffer5+3)==0 && *(buffer5+6)<=_16IMM)
			 {
				    if(*(buffer5+1)!=NOT || *(buffer5+5)!=NOT)
				    {
						  f_error(NORERR,51);
						  return(ERROR);
				    }

				    if(*(buffer5+2)==_8REG && *(buffer5+6)==_16IMM)
				    {
						  f_error(NORERR,81);
						  return(ERROR);
				    }

				    *(buffer4+0)=binarycode|0x04;
				    *(buffer4+1)=*(buffer5+7);
				    if(*(buffer5+2)==_8REG) return(2);
				    *(buffer4+0)=*(buffer4+0)|0x01;
				    *(buffer4+2)=*(buffer5+8);
				    return(3);
			 }

	   }

	   if(*(buffer5+2)>=BXSI)																								  /* [mem],[mem] */
	   {
			 if(*(buffer5+6)>=BXSI)
			 {
				    f_error(NORERR,41);
				    return(ERROR);
			 }

	   }

	   *(buffer5+0)=_1OPER;																											/* flag setting */

	   a=f_oneoperandcode(buffer5+1,binarycode,0x00);
	   if(a==ERROR) return(ERROR);

	   if(*(buffer5+6)==SREG)
	   {
			 f_error(NORERR,82);
			 return(ERROR);
	   }

	   if(*(buffer5+6)==OFFSET) *(buffer5+6)=_16IMM;				  /* code reset */

	   if(*(buffer5+6)==STRING)
	   {
			 f_error(NORERR,84);
			 return(ERROR);
	   }

	   if(*(buffer5+6)<=_16IMM)			   /* ???,imm */
	   {
			 if(*(buffer5+5)!=NOT)
			 {
				    f_error(NORERR,74);
				    return(ERROR);
			 }

			 if(*(buffer5+1)==BYTE || *(buffer5+2)==_8REG) c1=0;
																 else c1=1;

			 if(c1==0 && *(buffer5+6)==_16IMM)
			 {
				    f_error(NORERR,81);
				    return(ERROR);
			 }

			 *(buffer4+0)=0x80|c1;
			 *(buffer4+1)=*(buffer4+1)|binarycode;
			 *(buffer4+a)=*(buffer5+7);
			 a++;
			 if(c1==0) return(a);
			 if(*(buffer5+8)==0xff)																				  /* S bit */
			 {
				    if(*(buffer5+7)>=0x80)
				    {
						  a--;
						  *(buffer4+0)=*(buffer4+0)|0x02;
						  *(buffer4+a)=*(buffer5+7);
						  a++;
						  return(a);
				    }

			 }

			 *(buffer4+a)=*(buffer5+8);
			 a++;
			 return(a);
	   }

	   if(*(buffer5+6)==_8REG || *(buffer5+6)==_16REG)			  /* ???,reg */
	   {
			 if(*(buffer5+5)!=NOT)
			 {
				    f_error(NORERR,74);
				    return(ERROR);
			 }

			 c1=*(buffer5+6)-_8REG;
			 if(*(buffer5+1)==BYTE || *(buffer5+2)==_8REG) c2=0;
																 else c2=1;

			 if(c1!=c2)
			 {
				    f_error(NORERR,81);
				    return(ERROR);
			 }

			 c1=*(buffer5+7);
			 c1=c1<<3;
			 *(buffer4+1)=*(buffer4+1)|c1;
			 return(a);
	   }

	   if(*(buffer5+5)==NOT)
	   {
			 f_error(NORERR,63);
			 return(ERROR);
	   }

	   if(*(buffer5+5)!=BYTE && *(buffer5+5)!=WORD)
	   {
			 f_error(NORERR,64);
			 return(ERROR);
	   }

	   c1=*(buffer5+2)-_8REG;
	   if(*(buffer5+5)==BYTE) c2=0;
								 else c2=1;

	   if(c1!=c2)
	   {
			 f_error(NORERR,81);
			 return(ERROR);
	   }

	   binarycode=binarycode|0x02;

	   c1=*(buffer4+1);
	   c1=c1&0x07;
	   c1=c1<<3;

	   return(f_oneoperandcode(buffer5+5,binarycode,c1));
}

int f_loadaddress(int passtype,char binarycode)
{
	   char c1;

	   if(*(buffer5+0)!=_2OPER)
	   {
				    f_error(NORERR,49);
			 return(ERROR);
	   }

	   if(*(buffer5+2)!=_16REG)
	   {
			 f_error(NORERR,85);
			 return(ERROR);
	   }

	   if(*(buffer5+1)!=NOT)
	   {
			 f_error(NORERR,86);
			 return(ERROR);
	   }

	   if(*(buffer5+6)!=LABEL && *(buffer5+6)<=STRING)
	   {
			 f_error(NORERR,87);
			 return(ERROR);
	   }

	   if(binarycode==0x8d)																											/* lea */
	   {
			 if(*(buffer5+5)!=NEAR && *(buffer5+5)!=NOT)
			 {
				    f_error(NORERR,79);
				    return(ERROR);
			 }

			 if(*(buffer5+5)==NOT)
			 {
				    if(passtype==_1PASS) f_error(WARNING,13);
									    else f_error(WARNING,14);
			 }

	   }
	   else																																						/* lds les */
	   {
				    if(*(buffer5+5)!=FAR && *(buffer5+5)!=NOT)
			 {
				    f_error(NORERR,77);
				    return(ERROR);
			 }

			 if(*(buffer5+5)==NOT)
			 {
				    if(passtype==_1PASS) f_error(WARNING,9);
									    else f_error(WARNING,10);
			 }

	   }

	   c1=*(buffer5+3);
	   c1=c1<<3;

	   *(buffer5+0)=_1OPER;																											/* flag setting */
	   *(buffer5+5)=BYTE;																												   /* flag setting */
	   return(f_oneoperandcode(buffer5+5,binarycode,c1));
}

int f_rotateshiftoperandcode(char binarycode1,char binarycode2)
{
	   int a;

	   if(*(buffer5+0)!=_2OPER)
	   {
			 f_error(NORERR,49);
			 return(ERROR);
	   }

	   *(buffer5+0)=_1OPER;																											/* flag setting */

	   a=f_oneoperandcode(buffer5+1,binarycode1,binarycode2);
	   if(a==ERROR) return(ERROR);

	   if(*(buffer5+5)!=NOT)
	   {
			 f_error(NORERR,74);
			 return(ERROR);
	   }

	   if(*(buffer5+6)!=_8IMM && *(buffer5+6)!=_8REG)
	   {
			 f_error(NORERR,75);
			 return(ERROR);
	   }

	   if(*(buffer5+7)!=1)																											/* imm 1 or cl */
	   {
			 f_error(NORERR,75);
			 return(ERROR);
	   }

	   if(*(buffer5+6)==_8REG) *(buffer4+0)=*(buffer4+0)|0x02;

	   return(a);
}

char    *f_cpyutilstr(char *buffer,char *strP1,char *strP2)
{
	   char	 *p1;
	   char	 c1;

	   p1=strpbrk(strP1,strP2);

	   if(p1!=NULL)
	   {
			 c1=*p1;
			 *p1=NULL;
	   }

	   strcpy(buffer,strP1);

	   if(p1!=NULL) *p1=c1;

	   return(p1);
}

int f_strtok(char *tokarray[],char *buffer,char *strP,char *div1,char *div2)
{
	   char	 *p1,*p2,*p3;
	   int a;

	   p1=strP;
	   p2=buffer;

	   while(*p1!=NULL)
	   {
			 p3=strchr(div2,*p1);
			 if(p3!=NULL)
			 {
				    *p2=*div1;
				    p2++;
				    *p2=*p1;
				    p2++;
				    *p2=*div1;
				    p2++;
			 }
			 else
			 {
				    *p2=*p1;
				    p2++;
			 }

			 p1++;
	   }

	   *p2=NULL;

	   a=0;
	   p1=strtok(buffer,div1);
	   while(p1!=NULL)
	   {
			 tokarray[a]=p1;
			 p1=strtok(NULL,div1);
			 a++;
	   }

	   return(a);
}

int f_strarraycmp(char *strarray[],char *strP)
{
	   int a;

	   a=0;

	   while(strarray[a]!=NULL)
	   {
			 if(strcmp(strarray[a],strP)==NULL) return(a);
			 a++;
	   }

	   return(-1);
}

int f_strtobin(char *strP,char *errorreturn)
{
	   char	 *p1;
	   int a,b,c;

	   strP=f_strskipchar(strP,'0');

	   a=0;
	   p1=strP;
	   while(*p1!=NULL)
	   {
			 a++;

			 if(f_arrangecheck('0','1',*p1)==1)
			 {
				    p1++;
				    continue;
			 }

			 *errorreturn=BADBIN;
			 return(-1);
	   }

	   if(a>16)
	   {
			 *errorreturn=BIGBIN;
			 return(-1);
	   }

	   c=0;
	   p1=strP;
	   while(a>0)
	   {
			 b=*p1-'0'+0x00;
			 c=c|(b<<a-1);
			 p1++;
			 a--;
	   }

	   *errorreturn=BINOK;
	   return(c);
}

int f_strtodec(char *strP,char *errorreturn)
{

	   char	 *p1;
	   char	 buffer[7];
	   int a;
	   int miners;
	   long	 int aa;

	   if(*strP=='-')
	   {
			 miners=1;
			 buffer[0]='-';
			 strP++;
	   }
	   else
	   {
			 miners=0;
	   }

	   strP=f_strskipchar(strP,'0');
/*	   if(*strP==NULL)
	   {
			 *errorreturn=DECOK;
			 return(0);
	   }*/

	   a=0;
	   p1=strP;
	   while(*p1!=NULL)
	   {
			 a++;

			 if(f_arrangecheck('0','9',*p1)==1)
			 {
				    p1++;
				    continue;
			 }

			 *errorreturn=BADDEC;
			 return(-1);
	   }

	   if(a>5)
	   {
			 *errorreturn=BIGDEC;
			 return(-1);
	   }

	   strcpy(&buffer[miners],strP);
	   aa=atol(&buffer[0]);

	   if(miners==1)
	   {
			 if(aa<-32768)
			 {
				    *errorreturn=BIGDEC;
				    return(-1);
			 }
	   }
	   else
	   {
			 if(aa>65535)
			 {
				    *errorreturn=BIGDEC;
				    return(-1);
			 }
	   }

	   a=aa;
	   *errorreturn=DECOK;
	   return(a);
}

int f_strtohexa(char *strP,char *errorreturn)
{
	   char	 *p1;
	   int a,b,c;

	   strP=f_strskipchar(strP,'0');

	   a=0;
	   p1=strP;
	   while(*p1!=NULL)
	   {
			 a++;

			 if(f_arrangecheck('0','9',*p1)==1 ||
					f_arrangecheck('A','F',*p1)==1 ||
					f_arrangecheck('a','f',*p1)==1 )
			 {
				    p1++;
				    continue;
			 }

			 *errorreturn=BADHEXA;
			 return(-1);
	   }

	   if(a>4)
	   {
			 *errorreturn=BIGHEXA;
			 return(-1);
	   }

	   c=0;
	   p1=strP;
	   while(a>0)
	   {
			 if(f_arrangecheck('0','9',*p1)==1) b=*p1-'0'+0x00;
			 if(f_arrangecheck('A','F',*p1)==1) b=*p1-'A'+0x0a;
			 if(f_arrangecheck('a','f',*p1)==1) b=*p1-'a'+0x0a;

			 c=c|(b<<(a*4-4));
			 p1++;
			 a--;
	   }

	   *errorreturn=HEXAOK;
	   return(c);
}

int f_ascstr2codestr(char *buffer,char *strP,char *chararray,char *divstr,
								char *errorreturn,char c1,char c2)
{
	   char	 *p1;
	   char	 c3;

	   int a;

	   strP=f_strskipstr(strP,divstr);
	   if(*strP!=c1)
	   {
			 *errorreturn=NOTDIVSTR;
			 return(-1);
	   }

	   strP++;

	   a=0;
	   while(*strP!=c1)
	   {
			 if(*strP==NULL)
			 {
				    *errorreturn=NOTDIVSTR;
				    return(-1);
			 }

			 if(*strP==c2)
			 {
				    c3=*strP;
				    strP++;
				    p1=chararray;
				    while(1)
				    {
						  if(*p1==NULL)
						  {
								*buffer=c3;
								c3=*strP;
								buffer++;
								a++;
								break;
						  }

						  if(*p1==*strP)
						  {
								c3=*(p1+1);
								break;
						  }

						  p1=p1+2;
				    }

			 }
			 else
			 {
				    c3=*strP;
			 }

			 *buffer=c3;
			 buffer++;
			 strP++;
			 a++;
	   }

	   strP++;
	   strP=f_strskipstr(strP,divstr);

	   if(*strP!=NULL)
	   {
			 *errorreturn=STROVER;
			 return(-1);
	   }

	   *errorreturn=STROK;
	   return(a);
}

char    *f_notcharfindcpystr(char *buffer,char *strP1,char *strP2,char c1)
{
	   char	 *p1;

	   p1=strchr(strP1,c1);

	   if(p1==NULL)
	   {
			 sprintf(buffer,"%s%s",strP1,strP2);
			 p1=buffer;
	   }
	   else
	   {
			 p1=NULL;
	   }

	   return(p1);
}

char    *f_strskipchar(char *strP,char c)
{
	   while(*strP==c) strP++;
	   return(strP);
}

char    *f_strskipstr(char *strP1,char *strP2)
{
	   char	 *p1;

	   while(*strP1!=NULL)
	   {
			 p1=strchr(strP2,*strP1);
			 if(p1==NULL) return(strP1);
			 strP1++;
	   }

	   return(strP1);
}

int f_arrangecheck(char c1,char c2,char c3)
{
	   if(c3>=c1 && c3<=c2) return(1);
	   return(-1);
}


