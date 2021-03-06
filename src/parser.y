/*
    This file is part of Ellerre

    Ellerre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ellerre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Ellerre. If not, see <http://www.gnu.org/licenses/>.
*/
%{
  #include<stdio.h>
  #include "symbol.h"
  #include "rule.h"
  #include "main.h"

  extern "C" int yylex (void);
  extern int yylineno;
  extern char *yytext;
  void yyerror (char const *msg);

  Symbol *current_head;
  std::vector<Symbol*> current_corpo;
%}

%token TK_LIT_STRING
%token TK_LIT_LETTER
%token TK_FLECHA
%token TK_BREAK
%token TK_SEP
%token TK_DIVISOR
%define parse.error verbose
%start gr
%%

gr: regras | ;

regras: lista_regras;
lista_regras: regra | lista_regras regra;
regra: cabeca TK_FLECHA lista_corpos TK_SEP { current_head = NULL; };
cabeca: TK_LIT_STRING {
                       current_head = grammar->getSymbol(yytext);
                       current_corpo.clear();
                      };
lista_corpos: corpo;
lista_corpos: lista_corpos TK_DIVISOR corpo;
corpo: /*empty*/ {
                   current_corpo.push_back(grammar->getEmptySymbol());
                   Rule *rule = new Rule(current_head, current_corpo);
                   grammar->addRule (rule);
                   rule = NULL;
                   current_corpo.clear();
                 };
corpo: lista_palavras {
                       Rule *rule = new Rule(current_head, current_corpo);
                       grammar->addRule(rule);
                       rule = NULL;
                       current_corpo.clear();
                      };
lista_palavras: palavra | lista_palavras palavra;
palavra: TK_LIT_STRING { current_corpo.push_back(grammar->getSymbol(yytext)); };

%%

void yyerror (char const *msg)
{
  fprintf (stderr, "%s on line %d\n", msg, yylineno);
}
