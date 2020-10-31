/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#include "LinkedList.h"
#include "StringIndexed.h"

enum BASICBlock {
 BASIC_WHILE_WEND,
 BASIC_IF_THEN,
 BASIC_DEFFN_FEND,
 BASIC_DO_LOOP,
 BASIC_GOSUB_RETURN,
 BASIC_FOR_NEXT
};

enum BASICDirectives {
 BASIC_INCLUDE_,
 BASIC_CHAIN_//,
// BASIC_LIST_,
// BASIC_NOLIST_,
// BASIC_PAGE_,
// BASIC_EJECT_
};

enum BASICStatements {
 BASIC_LABEL,
 BASIC_REM,
// BASIC_CONSOLE
// BASIC_USR,  // ASM
// BASIC_TRON, // ML or ASM
 BASIC_STOP,
 BASIC_END,
 BASIC_PRINT,
 BASIC_PRINT_USING,
 BASIC_LPRINT,
 BASIC_LPRINT_USING,
 BASIC_INPUT,
 BASIC_INPUT_,
 BASIC_INPUT_POUND,
 BASIC_READ,
 BASIC_OPEN,
 BASIC_LET,
 BASIC_DIM,
 BASIC_ERASE,
 BASIC_DATA,
 BASIC_FIELD,
 BASIC_GET, // File Get, Graphics GET
 BASIC_PUT,
 BASIC_MAT,
 BASIC_RETURN,
 BASIC_DEF,
 BASIC_DEF_FN,
 BASIC_DEFINT,
 BASIC_DEFDBL,
 BASIC_DEFSNG,
 BASIC_DEFSTR,
 BASIC_ON,
 BASIC_GOTO,
 BASIC_GO,
 BASIC_LOOP_WHILE,
 BASIC_LOOP_WEND,
 BASIC_BEEP,
 BASIC_TIMER,
 BASIC_SCREEN,
 BASIC_CHAIN,
 BASIC_COMMON,
 BASIC_RANDOMIZE,
 BASIC_COLOR,
 BASIC_CIRCLE,
 BASIC_CLOSE,
 BASIC_CLS,
// BASIC_IOCTL,
 BASIC_KEY,
 BASIC_KEY_n,
 BASIC_DRAW,
 BASIC_LINE,
 BASIC_LINE_INPUT,
 BASIC_LINE_INPUT_POUND,
 BASIC_LSET,
 BASIC_RSET,
 BASIC_MID_,
 BASIC_ON_COM,
 BASIC_ON_PEN,
 BASIC_ON_PLAY,
 BASIC_ON_STRIG,
 BASIC_ON_TIMER,
 BASIC_ON_ERROR,
 BASIC_ON_GOSUB,
 BASIC_ON_GOTO,
 BASIC_OPEN_COM,
 BASIC_OPTION_BASE,
 BASIC_OUT,
 BASIC_PAINT,
 BASIC_PALETTE,
 BASIC_PALETTE_USING,
 BASIC_SPEED,
 BASIC_PEN,
 BASIC_POKE,
 BASIC_PRESET,
 BASIC_PSET,
 BASIC_PRINT_POUND,
 BASIC_RESTORE,
 BASIC_RESUME,
 BASIC_SOUND,
 BASIC_STRIG,
 BASIC_STRIG_N,
 BASIC_SWAP,
 BASIC_TIME_,
 BASIC_VIEW,
 BASIC_VIEW_PRINT,
 BASIC_WAIT,
 BASIC_WIDTH,
 BASIC_WINDOW,
 BASIC_WRITE,
 BASIC_WRITE_POUND,
 BASIC_SAVEMEM,
 BASIC_BLANK,
 BASIC_ERROR
};

enum BASICPredefinedFunctions {
 BASIC_ABS,
 BASIC_ATN,
 BASIC_TAN,
 BASIC_EXP,
 BASIC_RND,
 BASIC_LOG,
 BASIC_COS,
 BASIC_SIN,
 BASIC_SQR,
 BASIC_NUM,
 BASIC_INT,
 BASIC_FIX,
 BASIC_CINT,
 BASIC_ASC,
 BASIC_CSNG,
 BASIC_FN_SCREEN,
 BASIC_POS,
 BASIC_CVI, BASIC_CVS, BASIC_CVD,
// BASIC_COM, BASIC_ENVIRON, BASIC_ENVIRON_, BASIC_ERDEV_, 
 BASIC_EOF,
 BASIC_FRE,
 BASIC_HEX_,
 BASIC_INP,
 BASIC_INSTR,
 BASIC_LEFT_,
 BASIC_LEN,
 BASIC_LOC,
 BASIC_LOF,
 BASIC_LPOS,
// BASIC_IOCTL_
 BASIC_FN_MID_,
 BASIC_OCT_,
 BASIC_PEEK,
 BASIC_FN_PEN,
 BASIC_PLAY,
 BASIC_PMAP,
 BASIC_POINT,
 BASIC_RIGHT,
 BASIC_SGN,
 BASIC_SPACE_,
 BASIC_SPC,
 BASIC_STICK,
 BASIC_STR_,
 BASIC_STRING_,
 BASIC_TAB,
 BASIC_AT,
 BASIC_VAL,
 BASIC_VARPTR,
 BASIC_VARPTR_,
 BASIC_SEC,
 BASIC_CSC,
 BASIC_COT,
 BASIC_ARCSIN,
 BASIC_ARCCOS,
 BASIC_ARCSEC,
 BASIC_ARCCSC,
 BASIC_ARCCOT,
 BASIC_SINH,
 BASIC_COSH,
 BASIC_TANH,
 BASIC_SECH,
 BASIC_CSCH,
 BASIC_COTH,
 BASIC_ARCSINH,
 BASIC_ARCCOSH,
 BASIC_ARCTANH,
 BASIC_ARCCSCH,
 BASIC_ARCSECH,
 BASIC_ARCCOTH,
 BASIC_RENAME,
 BASIC_SIZE,
 BASIC_SADD,
 BASIC_MATCH,
 BASIC_UCASE_,
 BASIC_FN_CHR_,
 BASIC_FLOAT,
 BASIC_INT_PERCENT,
 BASIC_CONCHAR,
 BASIC_CONSTAT
};

enum BASICReservedRegisters {
 BASIC_CHR_,
 BASIC_CSRLIN,
 BASIC_MKI_, BASIC_MKS_, BASIC_MKD_,
 BASIC_DATE_,
 BASIC_ERR,
 BASIC_ERL
};

ONE(BASICVariable,{
})
 Zstring value;
 Zdouble d;
 Zint i;
 Zbool isInt,isNumber;
MANY(BASICVariable,BASICVariableHandle,BASICVariableHandles,"BASICVariable",BASICVariables,{})
DONE(BASICVariable);

ONE(BASICExpression,{
 type=BASIC_BLANK;
})
 BASICStatements type;
 Zstring label;
 Zstring expression;
 Zp<BASICExpression> from,to;
MANY(BASICExpression,BASICExpressionHandle,BASICExpressionHandles,"BASICExpression",BASICExpressions,{})
DONE(BASICExpression);

ONE(BASICRoutine,{})
 Zp<BASICExpressions> lines;
MANY(BASICRoutine,BASICRoutineHandle,BASICRoutineHandles,"BASICRoutine",BASICRoutines,{})
DONE(BASICRoutine);

ONE(BASICFile,{})
 Zstring bytes;
MANY(BASICFile,BASICFileHandle,BASICFileHandles,"BASICFile",BASICFiles,{})
DONE(BASICFile);