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
  #include "symbol.h"
  #include "rule.h"
  #include "parser.hh"

  extern "C" int yylex(void);
%}

%option yylineno

SPACE  [\ \t\r]
LETTER [^\ \t\r\n;]
DIGIT  [0-9]

%%

"=>"             return TK_FLECHA;
"|"              return TK_DIVISOR;
";"              return TK_SEP;

{LETTER}*        return TK_LIT_STRING;
\'[^']\'         return TK_LIT_STRING;
\"[^"]*\"        return TK_LIT_STRING;

\n
.


%%
