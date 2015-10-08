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

Grammar grammar;

int main (int argc, char **argv)
{
  int ret = yyparse();

  // std::vector<Symbol*>::iterator it;
  // for (it = tokens.begin(); it != tokens.end(); it++){
  //   std::cout << (*it)->str << std::endl;
  // }

  // std::vector<Rule*>::iterator it;
  // for (it = rules.begin(); it != rules.end(); it++){
  //   std::cout << (*it)->head->str << " => ";
  //   std::vector<Symbol*>::iterator it2;
  //   for (it2 = (*it)->body.begin(); it2 != (*it)->body.end(); it2++){
  //     std::cout << (*it2)->str << " ";
  //     if ((*it2)->terminal){
  // 	std::cout << "t ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }

  std::cout << grammar << std::endl;
  
  return ret;
}
