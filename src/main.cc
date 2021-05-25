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

  int ret = yyparse();

  std::cout << *grammar << std::endl;

  std::cout << std::endl << "First sets:" << std::endl;
  grammar->print_first_sets();

  std::cout << std::endl << "Follow sets:" << std::endl;
  grammar->print_follow_sets();
  
  delete grammar;
  grammar = NULL;
  return ret;
}
