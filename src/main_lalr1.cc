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
#include "main.h"

Grammar *grammar = NULL;

int main (int argc, char **argv)
{

  if ( argc != 1 ) {
    show_usage(argv[0]);
    return 1;
  }

  grammar = new Grammar();
  Parser* parser;

  int ret = yyparse();

  parser = new LALR1(grammar);
  
  std::cout << *grammar << std::endl;
  parser->print_first_sets();
  parser->print_follow_sets();
  
  parser->print_item_set();
  parser->print_automata();
  parser->generateDotFile("LALR1.dot");

  delete parser;
  delete grammar;
  grammar = NULL;
  return ret;
}
