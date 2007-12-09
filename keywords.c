/*
 *	TRABALHO DE COMPILADORES: IDENTADOR DE CÓDIGO FORTRAN 77
 *
 *	ALUNOS:
 *	RAFAEL DELALIBERA RODRIGUES
 *	PAULO HENRIQUE ARANTES URGAL
 *
 *  Arquivo: Keywords.c
 *
 */


#include "defaults.h"
#include "keywords.h"

char *keyword[] =
  { 
  "PROGRAM",  "FUNCTION",  "SUBROUTINE",
  "BLOCKDATA",  "IMPLICIT",  "PARAMETER",
  "FORMAT",  "ENTRY",  "INTEGER",
  "REAL",  "DOUBLEPRECISION",  "COMPLEX",
  "LOGICAL",  "CHARACTER",  "COMMON",
  "DATA",  "DIMENSION",  "EQUIVALENCE",
  "EXTERNAL",  "INTRINSIC",  "SAVE",
  "IMPLICIT",  "PARAMETER",  "FORMAT",
  "ENTRY",  "DO",  "IF",  "ASSIGN",
  "BACKSPACE",  "CALL",  "CLOSE",  "CONTINUE",
  "ENDFILE",  "INQUIRE",  "OPEN",  "PAUSE",
  "PRINT",  "READ",  "RETURN",  "REWIND",
  "STOP",  "WRITE",  "GOTO",  "ENDIF",  "ELSE",
  "ELSEIF",  "THEN",  "END"};              


iskeyword (char const *symbol)
{
  token_t token;
  for (token = PROGRAM; token <= END; token++)
    {
      if (strcmp (keyword[token -  PROGRAM], symbol) == 0)
	return token;
    }
  return 0;
}
