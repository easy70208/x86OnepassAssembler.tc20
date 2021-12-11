#include <io.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>

#define	ESC	0x001b

void	cursorON(void);
void	cursorOFF(void);

/* ----------------------------------------
When this file compile, you must -K option
supply to compiler.  By the way nuknow bug
looked by display ---------------------- */

int     f_db(char *p,char *b1,char *b2);
int     f_Hb000(char *p,char *b1,char *b2);
int     f_Hb001(char *p,char *b1,char *b2);
int     f_Hb010(char *p,char *b1,char *b2);
int     f_Hb011(char *p,char *b1,char *b2,int offset);
int     f_Hb100(char *p,char *b1,char *b2);
int     f_Hb101(char *p,char *b1,char *b2);
int     f_Hb110(char *p,char *b1,char *b2);
int     f_Hb111(char *p,char *b1,char *b2,int offset);

int     f_MOD(char *p,char *b2);
int     f_acc_imm(char *p,char *b2);
int     f_reg_mem_imm(char *p,char *b2);
int     f_reg16_sreg(char *p,char *b2);
int     f_reg_mem_cl(char *p,char *b2);
int     f_reg8_16__mem8_16(char *p,char *b2);

char    GF1[]="Unregistered GoldFish friend 1994 All right unreserved\n"
	      "Tiny Unasembler 1 test version by your friend GoldFish\n\n"
	      "-- CATION --------------------------------------------\n"
	      "This program not supplied both 8087 operation code and\n"
	      "ESC operand in 8086.  In addtion to only supplied to *\n"
	      ".com file.  Please call me TEL 02-999-2010 if BUG in t\n"
	      "he Tiny Unassember were founded ----------------------\n";

char    GF2[]="EX>TU1.EXE  BINARY.COM  [file-out-filename]\n";

char    *str[8][4] = {
        "BX+SI","AL","AX","ES",
        "BX+DI","CL","CX","CS",
        "BP+SI","DL","DX","SS",
        "BP+DI","BL","BX","DS",
        "SI",   "AH","SP","b",
        "DI",   "CH","BP","b",
        "BP",   "DH","SI","b",
        "BX",   "BH","DI","b",
        };



void    main(int argc,char *argv[])
{

        FILE    *fp1,*fp2;

/* ---- Time structor ----- */
	struct tm *tm_now;
	time_t secs_now;
	char  *str_now;
/* ------------------------ */

        int     a;

        int     handle,offset=0x100,c1=0,line=0;
        long    size;

        char    *p1;

        char    b[40];
        char    *b1=&b[0],*b2=&b[10];

        char    *space="              ";


	cursorOFF();


        puts(&GF1[0]);

        if ( argc != 2 ) {
                if ( argc !=3 ) {
                        puts(&GF2[0]);
                        exit(1);
                        }
                }

        if ( (fp1=fopen(argv[1],"rb")) == NULL ) {
                printf("%s file cannot open\n",argv[1]);
                exit(1);
                }

        if ( argc == 3 ) {
                if ( (fp2=fopen(argv[2],"wt")) == NULL ) {
                        printf("%s file cannot open\n",argv[2]);
                        exit(1);
                        }

			printf("%s file processing.....\n",argv[1]);

                        time(&secs_now);
                        str_now = ctime(&secs_now);
                        tm_now = localtime(&secs_now);
                        str_now = asctime(tm_now);
                        fprintf(fp2,"%s\n",str_now);


                }

        handle = fileno(fp1);
        size = filelength(handle);

        if ( (p1=malloc(size)) == NULL ) {
                puts("Out of Memory");
                exit(1);
                }

        fread(p1,size,1,fp1);

        handle = 0;

        while ( handle < size ) {

		if ( argc == 2 ) printf("%04X:%04X ",_DS,offset);
                        else     fprintf(fp2,"%04X:%04X ",_DS,offset);

                switch ( *p1&0xe0 ) {

                        case    0x00 :  c1=f_Hb000(p1,b1,b2); break;
                        case    0x20 :  c1=f_Hb001(p1,b1,b2); break;
                        case    0x40 :  c1=f_Hb010(p1,b1,b2); break;
                        case    0x60 :  c1=f_Hb011(p1,b1,b2,offset); break;
                        case    0x80 :  c1=f_Hb100(p1,b1,b2); break;
                        case    0xa0 :  c1=f_Hb101(p1,b1,b2); break;
                        case    0xc0 :  c1=f_Hb110(p1,b1,b2); break;
                        case    0xe0 :  c1=f_Hb111(p1,b1,b2,offset); break;
                        default      :  c1=f_db(p1,b1,b2);
			}

                p1 = p1 + c1;
                handle = handle + c1;
		offset = offset + c1;

                if ( argc == 2 ) {
                        for ( a=0 ; a<c1 ; a++) {
                                printf("%02X",*(p1+a));
                                }

			printf("%s%s%s\n",space+(c1*2),b1,b2);

/*--------------------- LINE STOP BEFORE 25 ----------------------------*/

			if ( line == 22 ) {
				puts("Please any key continue......");
				line=getch();
				if ( line == ESC ) {
					cursorON();
					free(p1);
					exit(1);
					}
                                line=0;
                                }
                                else {
                                line++;
                                }

                        }
                        else {
                        for ( a=0 ; a<c1 ; a++) {
                                fprintf(fp2,"%02X",*(p1+a));
                                }

			fprintf(fp2,"%s%s%s\n",space+(c1*2),b1,b2);
			printf("%05d byte writed\r",handle);
                        }



		if ( handle >= 0x4000 ) {
			fprintf(fp2,"\nProcessing is stop by Bing error");
			puts("\nProcessing is stop by Big error");
			cursorON();
			free(p1);
			exit(1);
			}

                }

	if ( argc == 3 ) fclose(fp2);

	cursorON();
	free(p1);

}


/*-------------------------------------------------------------*/
int     f_db(char *p,char *b1,char *b2)
{
        sprintf(b1,"DB      ");
        sprintf(b2,"%02X",*p);

        return(1);
}


int     f_Hb000(char *p,char *b1,char *b2)
{
        char    *oper1[] = {"ADD     ","OR      ","ADC     ","SBB     "};

        unsigned char   Lb4,sreg;

        Lb4 = *p & 0x04;
        Lb4 = Lb4 >> 2;

        sreg = *p & 0x18;
        sreg = sreg >> 3;

        if ( (*p&0x07)==0x06 )  {
                sprintf(b1,"PUSH    ");
                sprintf(b2,"%s",str[sreg][3]);
                return(1);
                }

        if ( (*p&0x07)==0x07 )  {
                sprintf(b1,"POP     ");
                sprintf(b2,"%s",str[sreg][3]);
                return(1);
                }

        sprintf(b1,"%s",oper1[sreg]);

        if ( Lb4 == 0 ) return(f_MOD(p,b2));
                else    return(f_acc_imm(p,b2));
}


int     f_Hb001(char *p,char *b1,char *b2)
{
        char    *oper1[] = {"AND     ","SUB     ","XOR     ","CMP     "};
        char    *oper2[] = {"DAA     ","DAS     ","AAA     ","AAS     "};

        unsigned char   Hb1Lb8,Lb4;

        Hb1Lb8 = *p & 0x18;
        Hb1Lb8 = Hb1Lb8 >> 3;

        Lb4 = *p & 0x04;
        Lb4 = Lb4 >> 2;

        if ( ( *p & 0x07 ) == 0x06 )  {
                sprintf(b1,"%s:",str[Hb1Lb8][3]);
                *b2 = NULL;
                return(1);
                }

        if ( ( *p & 0x07 )== 0x07 )  {
                sprintf(b1,"%s",oper2[Hb1Lb8]);
                *b2 = NULL;
                return(1);
                }

        sprintf(b1,"%s",oper1[Hb1Lb8]);

        if ( Lb4 == 0 ) return(f_MOD(p,b2));
                else    return(f_acc_imm(p,b2));

}


int     f_Hb010(char *p,char *b1,char *b2)
{
        unsigned char   Hb1Lb8,reg;

        char    *oper1[] = {"INC     ","DEC     ","PUSH    ","POP     "};

        Hb1Lb8 = *p & 0x18;
        Hb1Lb8 = Hb1Lb8 >> 3;
        sprintf(b1,"%s",oper1[Hb1Lb8]);

        reg = *p&0x07;
        sprintf(b2,"%s",str[reg][2]);

        return(1);
}


int     f_Hb011(char *p,char *b1,char *b2,int offset)
{
        char    *oper1[4][4] = {"JO      ","JNO     ","JB      ","JNB     ",
                                "JZ      ","JNZ     ","JBE     ","JA      ",
                                "JS      ","JNS     ","JPE     ","JPO     ",
                                "JL      ","JGE     ","JLE     ","JG      "};

        unsigned char   Hb1,Lb84,Lb21;

        int     a;

        Hb1 = *p & 0x10;
        if ( Hb1 == 0 ) return(f_db(p,b1,b2));

        Lb84 = *p & 0x0c;
        Lb84 = Lb84 >> 2;

        Lb21 = *p & 0x03;



        a=sprintf(b1,"%s",oper1[Lb84][Lb21]);

        if ( *(p+1) >= 0 && *(p+1) <= 0x7f )
                a = ( offset + 2 ) + *(p+1);
                else
                a = ( offset + 2 ) - ( 0x00 - *(p+1) );

        a=sprintf(b2,"%04X",a);

        return(2);
}


int     f_Hb100(char *p,char *b1,char *b2)
{
        char    *oper1[] = {"ADD     ","OR      ","ADC     ","SBB     ",
                            "AND     ","SUB     ","XOR     ","CMP     "};

        char    *oper2[] = {"CBW     ","CWD     ","CALL    ","WAIT    "};

        char    *oper3[] = {"PUSHF   ","POPF    ","SAHF    ","LAHF    "};

        unsigned char   operand;
        int     a;

        operand = *p & 0x1c;
        operand = operand >> 2;

        switch ( operand ) {

                case 0 : {
                        operand = *(p+1) & 0x38;
                        operand = operand >> 3;
                        sprintf(b1,"%s",oper1[operand]);
                        return(f_reg_mem_imm(p,b2));
                        }

                case 1 : {
                        operand = *p & 0x02;
                        operand = operand >> 1;

                                if ( operand == 0 ) {
                                        sprintf(b1,"TEST    ");
                                        operand = *p;
                                        *p = *p | 0x02;
                                        a=f_MOD(p,b2);
                                        *p = operand;
                                        return(a);
                                        }
                                        else {
                                        sprintf(b1,"XCHG    ");
                                        return(f_MOD(p,b2));
                                        }
                        }

                case 2 : {
                        sprintf(b1,"MOV     ");
                        return(f_MOD(p,b2));
                        }

                case 3 : {
                        operand = *p & 0x01;
                                if ( operand == 0 ) {
                                        sprintf(b1,"MOV     ");
                                        return(f_reg16_sreg(p,b2));
                                        }

                                operand = *p & 0x02;
                                operand = operand >> 1;


                                if ( operand == 0 ) {
                                        sprintf(b1,"LEA     ");
                                        operand = *p;
                                        *p = *p | 0x02;
                                        a=f_MOD(p,b2);
                                        *p = operand;
                                        return(a);
                                        }
                                        else {
                                        sprintf(b1,"POP     ");
                                        a=f_MOD(p,b2);
                                        sprintf(b2,"%s",b2+3);
                                        return(a);
                                        }
                        }

                case 4 : {
                        operand = *p & 0x07;

                                if ( operand == 0 ) {
                                        sprintf(b1,"NOP     ");
                                        *b2 = NULL;
                                        return(1);
                                        }
                                        else {
                                        sprintf(b1,"XCHG    ");
                                        sprintf(b2,"%s,AX",str[operand][2]);
                                        return(1);
                                        }
                        }

                case 5 : {
                        operand = *p & 0x07;
                        sprintf(b1,"XCHG    ");
                        sprintf(b2,"%s,AX",str[operand][2]);
                        return(1);
                        }

                case 6 : {
                        operand = *p & 0x03;
                        sprintf(b1,"%s",oper2[operand]);

                                if ( operand == 2 ) {
                                        sprintf(b2,"%02X%02X:%02X%02X",*(p+4),*(p+3),*(p+2),*(p+1));
                                        return(5);
                                        }
                                        else {
                                        *b2 = NULL;
                                        return(1);
                                        }
                        }

                case 7 : {
                        operand = *p & 0x03;
                        sprintf(b1,"%s",oper3[operand]);
                        *b2 = NULL;
                        return(1);
                        }

        }
}


int     f_Hb101(char *p,char *b1,char *b2)
{
        char    *oper1[] = {"MOVSB","MOVSW","CMPSB","CMPSW"};

        char    *oper2[] = {"LODSB","LODSW","SCASB","SCASW"};

        unsigned char   operand,Lb2,W,reg;

        operand = *p & 0x1c;
        operand = operand >> 2;

        Lb2 = *p & 0x02;
        Lb2 = Lb2 >> 1;

        W = *p & 0x01;

        switch ( operand ) {

                case 0 : {
                        sprintf(b1,"MOV     ");

                        if ( Lb2 == 0 ) {
                                sprintf(b2,"%s,[%02X%02X]",str[1][1+W],*(p+2),*(p+1));
                                }
                                else {
                                sprintf(b2,"[%02X%02X],%s",*(p+2),*(p+1),str[1][1+W]);
                                }
                        return(3);
                        }

                case 1 : {
                        operand = *p & 0x03;
                        sprintf(b1,"%s",oper1[operand]);
                        *b2 = NULL;
                        return(1);
                        }

                case 2 : {
                        if ( Lb2 == 0 ) {
                                sprintf(b1,"TEST    ");
                                return(f_acc_imm(p,b2));
                                }
                                else {
                                if ( W == 0 ) sprintf(b1,"STOSB");
                                        else    sprintf(b1,"STOSW");
                                *b2 = NULL;
                                return(1);
                                }
                        }

                case 3 : {
                        operand = *p & 0x03;
                        sprintf(b1,"%s",oper2[operand]);
                        *b2 = NULL;
                        return(1);
                        }

                default : {

                        W = *p & 0x08;
                        W = W >> 3;
                        reg = *p & 0x07;

                        sprintf(b1,"MOV     ");

                        if ( W == 0 ) {
                                sprintf(b2,"%s,%02X",str[reg][1],*(p+1));
                                return(2);
                                }
                                else {
                                sprintf(b2,"%s,%02X%02X",str[reg][2],*(p+2),*(p+1));
                                return(3);
                                }
                        }

        }
}


int     f_Hb110(char *p,char *b1,char *b2)
{
        char    *oper1[] = {"LES     ","LDS     "};
        char    *oper2[] = {"INT     ","INT     ","INTO    ","IRET    "};
        char    *oper3[] = {"ROL     ","ROR     ","RCL     ","RCR     ",
                            "SHL     ","SHR     ","bbbbbbbb","SAR     "};

        unsigned char   operand,Lb2,W,mod,r_m;
        int     a;

        operand = *p & 0x1c;
        operand = operand >> 2;

        Lb2 = *p & 0x02;
        Lb2 = Lb2 >> 1;

        W = *p & 0x01;


        switch ( operand ) {

                case 0 : {
                        if ( Lb2 == 0 ) return(f_db(p,b1,b2));

                        sprintf(b1,"RET     ");

                        if ( W == 0 ) {
                                sprintf(b2,"%02X%02X",*(p+2),*(p+1));
                                return(3);
                                }
                                else {
                                *b2 = NULL;
                                return(1);
                                }
                        }

                case 1 : {
                        if ( Lb2 == 0 ) {
                                sprintf(b1,"%s",oper1[W]);
                                operand = *p;
                                *p = *p | 0x03;
                                a = f_MOD(p,b2);
                                *p = operand;
                                return(a);
                                }
                                else {
                                sprintf(b1,"MOV     ");
                                operand = *p;
                                *p = *p & 0xfd;
                                a = f_reg_mem_imm(p,b2);
                                *p = operand;
                                return(a);
                                }
                        }

                case 2 : {
                        if ( Lb2 == 0 ) return(f_db(p,b1,b2));

                        sprintf(b1,"RETF    ");

                        if ( W == 0 ) {
                                sprintf(b2,"%02X%02X",*(p+2),*(p+1));
                                return(3);
                                }
                                else {
                                *b2 = NULL;
                                return(1);
                                }
                        }

                case 3 : {
                        operand = *p & 0x03;

                        sprintf(b1,"%s",oper2[operand]);
                        if ( Lb2 == 0 ) {
                                if ( W == 0 ) {
                                        sprintf(b2,"3");
                                        return(1);
                                        }
                                        else {
                                        sprintf(b2,"%02X",*(p+1));
                                        return(2);
                                        }
                                }
                                else {
                                *b2 = NULL;
                                return(1);
                                }
                        }

                case 4 : {
                        operand = *(p+1) & 0x38;
                        operand = operand >> 3;
                        if ( operand == 6 ) return(f_db(p,b1,b2));

                        sprintf(b1,"%s",oper3[operand]);
                        return(f_reg_mem_cl(p,b2));
                        }

                case 5 : {
                        if ( Lb2 == 0 ) {
                                if ( *(p+1) != 0x0a ) return(f_db(p,b1,b2));
                                        else {
                                        if ( W == 0 ) sprintf(b1,"AAM");
                                                else  sprintf(b1,"AAD");
                                        *b2 = NULL;
                                        return(2);
                                        }
                                }

                        if ( W == 0 ) return(f_db(p,b1,b2));
                                else {
                                sprintf(b1,"XLAT");
                                *b2 = NULL;
                                return(1);
                                }

                        }
/* ----------- 'case 6' is ESC operation code not application -----------*/
                case 6 : {
                        return(f_db(p,b1,b2));
                        }

                }


}

int     f_Hb111(char *p,char *b1,char *b2,int offset)
{
        char    *oper1[] = {"LOOPNZ  ","LOOPZ   ","LOOP    ","JCXZ    "};
        char    *oper2[] = {"LOCK    ","bbbbbbbb","REPNZ   ","REPZ    "};
	char    *oper3[] = {"TEST  ","bbbbbbbb","NOT     ","NEG     ",
                            "MUL     ","IMUL    ","DIV     ","IDIV    "};
        char    *oper4[] = {"CLC     ","STC     ","CLI     ","STI     "};
        char    *oper5[] = {"INC     ","DEC     ","CALL    ","CALL    ",
                            "JMP     ","JMP     ","PUSH    ","bbbbbbbb"};

        unsigned char   operand,Lb21,Lb2,W,mod,reg,r_m;
        int     a=0;

        operand = *p & 0x1c;
        operand = operand >> 2;

        Lb21 = *p & 0x03;

        Lb2 = *p & 0x02;
        Lb2 = Lb2 >> 1;

        W = *p & 0x01;


        switch ( operand ) {

                case 0 : {
                        sprintf(b1,"%s",oper1[Lb21]);
                        if ( *(p+1) >= 0 && *(p+1) <= 0x7f )
                                a = ( offset + 2 ) + *(p+1);
                                else
                                a = ( offset + 2 ) - ( 0x00 - *(p+1) );

                        a=sprintf(b2,"%04X",a);
                        return(2);
                        }

                case 1 : {
                        if ( Lb2 == 0 ) {
                                sprintf(b1,"IN      ");
                                sprintf(b2,"%s,%02X",str[0][1+W],*(p+1));
                                }
                                else {
                                sprintf(b1,"OUT     ");
                                sprintf(b2,"%02X,%s",*(p+1),str[0][1+W]);
                                }
                        return(2);
                        }

                case 2 : {
                        if ( Lb21 == 0 ) sprintf(b1,"CALL    ");
                                else     sprintf(b1,"JMP     ");

                        if ( Lb2 == 0 ) {
                                a = *(p+2);
                                a = a << 8;
                                a = a + offset + 3 + *(p+1);
                                sprintf(b2,"%04X",a);
                                return(3);
                                }
                                else {
                                if ( W == 0 ) {
                                        sprintf(b2,"%02X%02X:%02X%02X",*(p+4),*(p+3),*(p+2),*(p+1));
                                        return(5);
                                        }
                                        else {
                                        if ( *(p+1) >= 0 && *(p+1) <= 0x7f )
                                                a = ( offset + 2 ) + *(p+1);
                                                else
                                                a = ( offset + 2 ) - ( 0x00 - *(p+1) );

                                        a=sprintf(b2,"%04X",a);
                                        return(2);
                                        }
                                }
                        }

                case 3 : {
                        if ( Lb2 == 0 ) {
                                sprintf(b1,"IN      ");
                                sprintf(b2,"%s,DX",str[0][1+W]);
                                }
                                else {
                                sprintf(b1,"OUT     ");
                                sprintf(b2,"DX,%s",str[0][1+W]);
                                }
                        return(1);
                        }

                case 4 : {
                        if ( Lb21 == 1 ) return(f_db(p,b1,b2));
                                else {
                                sprintf(b1,"%s",oper2[Lb21]);
                                *b2 = NULL;
                                return(1);
                                }
                        }

                case 5 : {
                        if ( Lb2 == 0 ) {
                                if ( W == 0 ) sprintf(b1,"HLT");
                                        else  sprintf(b1,"CMC");
                                *b2 = NULL;
                                return(1);
                                }

                        operand = *(p+1) & 0x38;
                        operand = operand >> 3;
                        if ( operand == 0 ) {
                                sprintf(b1,"%s",oper3[operand]);
                                operand = *p;
                                *p = *p & 0xfd;
                                 a = f_reg_mem_imm(p,b2);
                                *p = operand;
                                return(a);
                                }

                        if ( operand == 1 ) return(f_db(p,b1,b2));

                        sprintf(b1,"%s",oper3[operand]);
                        return(f_reg8_16__mem8_16(p,b2));
                        }

                case 6 : {
                        sprintf(b1,"%s",oper4[Lb21]);
                        *b2 = NULL;
                        return(1);
                        }

                case 7 : {
                        if ( Lb2 == 0 ) {
                                if ( W == 0 ) sprintf(b1,"CLD");
                                        else  sprintf(b1,"STD");
                                *b2 = NULL;
                                return(1);
                                }

                        operand = *(p+1) & 0x38;
                        operand = operand >> 3;

                        if ( operand == 7 ) return(f_db(p,b1,b2));

                        sprintf(b1,"%s",oper5[operand]);

                        if ( operand == 3 || operand == 5 ) a=sprintf(b2,"FAR ");

			return(f_reg8_16__mem8_16(p,b2+a));
                        }

        }

}


/*=======================================================================*/
int     f_MOD(char *p,char *b2)
{
        unsigned char   Lb2,W,mod,reg,r_m;

        char    *p1,b[15];
        int     c1;

        Lb2 = 0x02 & *p;
        Lb2 = Lb2 >> 1;

        W = 0x01 & *p;

        mod = 0xc0 & *(p+1);
        mod = mod >> 6;

        reg = 0x38 & *(p+1);
        reg = reg >> 3;

        r_m = 0x07 & *(p+1);


        p1 = str[reg][1+W];

        switch ( mod ) {

                case    0 : if ( r_m == 6 ) {
                        sprintf(&b[0],"[%02X%02X]",*(p+3),*(p+2));
                        c1=4;
                        }
                        else {
                        sprintf(&b[0],"[%s]",str[r_m][0]);
                        c1=2;
                        }
                        break;

                case    1 :
                        sprintf(&b[0],"[%s+%02X]",str[r_m][0],*(p+2));
                        c1=3;
                        break;

                case    2 :
                        sprintf(&b[0],"[%s+%02X%02X]",str[r_m][0],*(p+3),*(p+2));
                        c1=4;
                        break;

                case    3 :
                        sprintf(&b[0],"%s",str[r_m][1+W]);
                        c1=2;
                        break;
                }


        if ( Lb2 == 1 ) {
                sprintf(b2,"%s,%s",p1,&b[0]);
                }
                else {
                sprintf(b2,"%s,%s",&b[0],p1);
                }


        return(c1);

}


int     f_acc_imm(char *p,char *b2)
{
        unsigned char   W;

        int     a,c1;

        W = *p & 0x01;

        a=sprintf(b2,"%s,",str[0][1+W]);

        if ( W == 1 ) {
                a=a+(sprintf(b2+a,"%02X%02X",*(p+2),*(p+1)));
                c1=3;
                }
                else {
                a=a+(sprintf(b2+a,"%02X",*(p+1)));
                c1=2;
                }

        return(c1);
}

int     f_reg_mem_imm(char *p,char *b2)
{
        unsigned char   S,W,mod,r_m;

        char    b[30];
        int     a,c1;

        S = 0x02 & *p;
        S = S >> 1;
        W = 0x01 & *p;

        mod = 0xc0 & *(p+1);
        mod = mod >> 6;

        r_m = 0x07 & *(p+1);

        if ( W == 1 ) {

        a=sprintf(&b[0],"WORD PTR ");

                switch ( mod ) {

                        case    0 :
                                if ( r_m == 6 ) {
                                a=a+sprintf(&b[0+a],"[%02X%02X]",*(p+3),*(p+2));
                                c1=6;
                                }
                                else {
                                a=a+sprintf(&b[0+a],"[%s]",str[r_m][0]);
                                c1=4;
                                }
                                break;

                        case    1 :
                                a=a+sprintf(&b[0+a],"[%s+%02X]",str[r_m][0],*(p+2));
                                c1=5;
                                break;

                        case    2 :
                                a=a+sprintf(&b[0+a],"[%s+%02X%02X]",str[r_m][0],*(p+3),*(p+2));
                                c1=6;
                                break;

                        case    3 :
                                a=sprintf(&b[0],"%s",str[r_m][1+W]);
                                c1=4;
                                break;
                        }
                c1=c1-S;
        }

        else {

        a=sprintf(&b[0],"BYTE PTR ");

                switch ( mod ) {

                        case    0 :
                                if ( r_m == 6 ) {
                                a=a+sprintf(&b[0+a],"[%02X%02X]",*(p+3),*(p+2));
                                c1=5;
                                }
                                else {
                                a=a+sprintf(&b[0+a],"[%s]",str[r_m][0]);
                                c1=3;
                                }
                                break;

                        case    1 :
                                a=a+sprintf(&b[0+a],"[%s+%02X]",str[r_m][0],*(p+2));
                                c1=4;
                                break;

                        case    2 :
                                a=a+sprintf(&b[0+a],"[%s+%02X%02X]",str[r_m][0],*(p+3),*(p+2));
                                c1=5;
                                break;

                        case    3 :
                                a=sprintf(&b[0],"%s",str[r_m][1+W]);
                                c1=3;
                                break;
                        }
        }

        if ( W == 1 ) {
                if ( S == 1 ) {
                sprintf(&b[0+a],",-%02X",(0x0100 - *(p+c1-1)));
                }
                else {
                sprintf(&b[0+a],",%02X%02X",*(p+c1-1),*(p+c1-2));
                }
        }

        else {
        sprintf(&b[0+a],",%02X",*(p+c1-1));
        }

        sprintf(b2,"%s",&b[0]);

        return(c1);
}


int     f_reg16_sreg(char *p,char *b2)
{
        unsigned char   Lb2,mod,sreg,r_m;

        char    *p1,b[15];
        int     a,c1;

        Lb2 = 0x02 & *p;
        Lb2 = Lb2 >> 1;

        mod = 0xc0 & *(p+1);
        mod = mod >> 6;

        sreg = 0x18 & *(p+1);
        sreg = sreg >> 3;

        r_m = 0x07 & *(p+1);


        p1 = str[sreg][3];

        switch ( mod ) {

                case    0 : if ( r_m == 6 ) {
                        sprintf(&b[0],"[%02X%02X]",*(p+3),*(p+2));
                        c1=4;
                        }
                        else {
                        sprintf(&b[0],"[%s]",str[r_m][0]);
                        c1=2;
                        }
                        break;

                case    1 :
                        sprintf(&b[0],"[%s+%02X]",str[r_m][0],*(p+2));
                        c1=3;
                        break;

                case    2 :
                        sprintf(&b[0],"[%s+%02X%02X]",str[r_m][0],*(p+3),*(p+2));
                        c1=4;
                        break;

                case    3 :
                        sprintf(&b[0],"%s",str[r_m][2]);
                        c1=2;
                        break;
                }



        if ( Lb2 == 1 ) {
                sprintf(b2,"%s,%s",p1,&b[0]);
                }
                else {
                sprintf(b2,"%s,%s",&b[0],p1);
                }


        return(c1);

}


int     f_reg_mem_cl(char *p,char *b2)
{
        unsigned char   Lb2,W,mod,r_m;

        char    b[30];
        int     a=0,c1;

        Lb2 = 0x02 & *p;
        Lb2 = Lb2 >> 1;

        W = 0x01 & *p;

        mod = 0xc0 & *(p+1);
        mod = mod >> 6;

        r_m = 0x07 & *(p+1);


        if ( mod != 3 ) {
                if ( W == 0 ) a=sprintf(&b[0],"BYTE PTR ");
                        else  a=sprintf(&b[0],"WORD PTR ");
                }


        switch ( mod ) {

                case    0 : if ( r_m == 6 ) {
                        sprintf(&b[a],"[%02X%02X]",*(p+3),*(p+2));
                        c1=4;
                        }
                        else {
                        sprintf(&b[a],"[%s]",str[r_m][0]);
                        c1=2;
                        }
                        break;

                case    1 :
                        sprintf(&b[a],"[%s+%02X]",str[r_m][0],*(p+2));
                        c1=3;
                        break;

                case    2 :
                        sprintf(&b[a],"[%s+%02X%02X]",str[r_m][0],*(p+3),*(p+2));
                        c1=4;
                        break;

                case    3 :
                        sprintf(&b[a],"%s",str[r_m][1+W]);
                        c1=2;
                        break;
                }


        if ( Lb2 == 0 ) {
                sprintf(b2,"%s,1",&b[0]);
                }
                else {
                sprintf(b2,"%s,CL",&b[0]);
                }

        return(c1);
}


int     f_reg8_16__mem8_16(char *p,char *b2)
{
        unsigned char   Lb2,W,mod,r_m;

        int     a=0,c1;

        Lb2 = 0x02 & *p;
        Lb2 = Lb2 >> 1;

        W = 0x01 & *p;

        mod = 0xc0 & *(p+1);
        mod = mod >> 6;

        r_m = 0x07 & *(p+1);


        if ( mod != 3 ) {
                if ( W == 0 ) a=sprintf(b2,"BYTE PTR ");
                        else  a=sprintf(b2,"WORD PTR ");
                }


        switch ( mod ) {

                case    0 : if ( r_m == 6 ) {
                        sprintf(b2+a,"[%02X%02X]",*(p+3),*(p+2));
                        c1=4;
                        }
                        else {
                        sprintf(b2+a,"[%s]",str[r_m][0]);
                        c1=2;
                        }
                        break;

                case    1 :
                        sprintf(b2+a,"[%s+%02X]",str[r_m][0],*(p+2));
                        c1=3;
                        break;

                case    2 :
                        sprintf(b2+a,"[%s+%02X%02X]",str[r_m][0],*(p+3),*(p+2));
                        c1=4;
                        break;

                case    3 :
                        sprintf(b2+a,"%s",str[r_m][1+W]);
                        c1=2;
                        break;
                }

        return(c1);
}

void   cursorON(void)
{
	union	REGS	r;

	r.h.ah = 1;
	r.h.ch = 8;
	r.h.cl = 9;
	int86 (0x10,&r,&r);
}

void cursorOFF(void)
{

	union	REGS	r;

	r.h.ah = 1;
	r.h.ch = 30;
	r.h.cl = 35;
	int86(0x10,&r,&r);
}





