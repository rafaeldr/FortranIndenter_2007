/*
 *	TRABALHO DE COMPILADORES: IDENTADOR DE CÓDIGO FORTRAN 77
 *
 *	ALUNOS:
 *	RAFAEL DELALIBERA RODRIGUES
 *	PAULO HENRIQUE ARANTES URGAL
 *
 *  Arquivo: Init_Fortran.c
 *
 */

#include "defaults.h"
#include "tokens.h"

extern void lexer_start(void);

main (int argc, char *argv[])
{
  dest = fopen("destino.c", "w");
  lexer_start ();
  main_program ();
  exit (0);
}
