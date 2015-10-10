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
  grammar = new Grammar();

  int ret = yyparse();

  std::cout << *grammar << std::endl;

  std::cout << std::endl << "First sets:" << std::endl;
  std::map<Symbol*,std::set<Symbol*> > firsts = grammar->first();
  for (auto& entry : firsts){
    std::cout << *(entry.first) << " -- ";
    for (Symbol *s : entry.second){
      std::cout << *s << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "Follow sets:" << std::endl;
  std::map<Symbol*,std::set<Symbol*> > follows = grammar->follow();
  for (auto& entry : follows){
    std::cout << *(entry.first) << " -- ";
    for (Symbol *s : entry.second){
      std::cout << *s << " ";
    }
    std::cout << std::endl;
  }
  
  delete grammar;
  grammar = NULL;
  return ret;
}
