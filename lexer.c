/*
 *	TRABALHO DE COMPILADORES: IDENTADOR DE CÓDIGO FORTRAN 77
 *
 *	ALUNOS:
 *	RAFAEL DELALIBERA RODRIGUES
 *	PAULO HENRIQUE ARANTES URGAL
 *
 *  Arquivo: Lexer.c
 *
 */

#include "defaults.h"
#include "keywords.h"

/*
 *       Public Members:
 */
 
token_t lookahead;
char lexeme[256];
void match (token_t);
void lexer_start (void);
long int lines = 1; 
long int column = 0;
int lexvalue;

/*
 *       Lexer Functions
 */
 
token_t gettoken (FILE * src) {
        token_t c;

        c=getc(src);
        column++;

/*
 *      'Next Line' Recognition (Column 7-72)
 */

        while (c=='\n'){
              c=getc(src);              
              column=1;
              lexeme[0] = c;
        }

/*
 *      Comment Recognition (Column 1)
 */

        if (column==1){
           if (c=='c'||c=='C'||c=='*'){
              fprintf(dest, "%c",c);
              while (c=getc(src)!='\n'){
                    fprintf(dest, "%c",c);
                    column=1;
              }
           }
        }          

/*
 *      'End of Line' Recognition / With EOF Case (Line > 72 Columns)
 */

        if (column==73){
           while (c!='\n'){
                 c=getc(src);
                 column++;
                 if (c==0) return EOL;
           }
           c=getc(src);
           column=1;
        }

/*
 *      Blank Space Recognition (Column 1-*)
 */

        while (c==' '){
              c=getc(src);
              column++;
        }
        lexeme[0]=c;

/*
 *      Label Recognition (Column 1-5)
 */
      
       if (column<6){        
        if (isdigit(c)){
           int i = 1;
           while (column < 5){
                 c=getc(src);
                 column++;
                 if (isdigit(c)){          
                    lexeme[i] = c;
                    i++;
                 }
                 else{
                      while ((column<6)&&(c==' ')){
                            c=getc(src);
                            column++;
                      }
                      if (column!=6){
                         lexeme[i]=' ';
                         i++;
                         lexeme[i]=c;
                         i++;
                         lexeme[i]=0;
                         fprintf (stderr, "error: unexpected label: %s", lexeme);
                         exit(-1);
                      }
                 } 
           }
           lexeme[i]=0;
           return LABEL;
        } 
        else{
             fprintf (stderr, "error: unexpected label: %s", lexeme);
             exit (-1);
        }
       }
/*
 *      Continuation Line Recognition (Column 6)
 */

        if (column==6){
           if (c!='0'){
              lexeme[1]=0;
              column++;
              return CONTLINE;
           }
           else column++;
        }

/* 
 *      Special Characters + - * / = ( ) : . , ' $ **
 */
// ALTERAR LEXEME[1]
  
        if (c == '+') {
           lexeme[1]=0;
           return '+';
        }
        if (c == '-') {
           lexeme[1]=0;
           return '-';
        }
        if (c == '*') {
           if (c=getc(src)=='*'){
              lexeme[1]=c;
              lexeme[2]=0;
              ungetc(c,src);
              return ASPAS2;
           }
           else{
                lexeme[1]=0;
                return '*';
           }
        }
        if (c == '/') {
           lexeme[1]=0;
           return '/';
        }
        if (c == '=') {
           lexeme[1]=0;
           return '=';
        } 
        if (c == '(') {
           lexeme[1]=0;
           return '(';
        } 
        if (c == ')') {
           lexeme[1]=0;
           return ')';
        } 
        if (c == ':') {
           lexeme[1]=0;
           return ':';
        } 
        if (c == '.') {
           lexeme[1]=0;
           return '.';
        }                                                        
        if (c == ',') {
           lexeme[1]=0;
           return ',';
        } 
        if (c == '$') {
           lexeme[1]=0;
           return '$';
        }                 
           
/*
 *         ID and Keyword Recognition
 */
 
  if (isalpha (c))
     {
      token_t keyword;
      extern iskeyword (char const *);
      int i = 1;
      
      while (isalnum (c = getc (src)))
            {
             lexeme[i] = c;
             i++;
            }
      lexeme[i] = 0;
      unget(c,src);
      if (keyword = iskeyword(lexeme))
         return keyword;
      return ID;
      }
      
/*
 *           Numeric Constant Recognition
 */
   
  if ('1' <= c && c <= '9')
    {
      int i = 1;
      lexeme[i] = c;
      while (isdigit (c = getc (src)))
	{
	  lexeme[i] = c;
	  i++;
	}
      lexeme[i] = 0;
      lexvalue = atoi (lexeme);
      ungetc (c, src);
      return CT;
    }

/*
 *      'Next Line' Recognition (Column 7-72)
 */

        while (c=='\n'){
              c=getc(src);              
              column=1;
              lexeme[0] = c;
        }

/*
 *      Return to Parser an Unknown Token 
 */
  lexeme[1] = 0;
  return c;
}

/*
 *       MATCH Function (The Connection Between Lexer and Parser)
 */

void match (token_t expected_token) {
     if (expected_token == lookahead)
        lookahead = gettoken (stdin);
     else {
          fprintf (stderr, "match: unexpected token: %s", lexeme);
          exit (-1);
          }
}


void
lexer_start (void)
{
  lookahead = gettoken (stdin);
}
 
