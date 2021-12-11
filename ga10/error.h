char	*warnings[] =
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
};

char	*errors[] =
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
	"file too many open",                    		    /* number is 10 */
	"source file need",
	"out of memory for sourcefile buffer",
	"out of memory for label buffer",
	"out of memory for equ buffer",
	"working file not open",                              /* number is 15 */
	"you must rename ga10.exe",
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
	"this 2operand do not have &labelname",
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
};

