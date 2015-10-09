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
#ifndef __GRAMAR_H
#define __GRAMAR_H
#include <map>
#include <iostream>
#include <ostream>
#include <vector>
#include "symbol.h"
#include "rule.h"

class Grammar
{
private:
  Symbol *empty;
  
public:
  std::vector<Rule*> rules;
  std::map<std::string,Symbol*> symbols;
  
public:
  Grammar();
  ~Grammar();
  void addRule(Rule *rule);
  Symbol *getSymbol(char *str);
  Symbol *getEmptySymbol ();
  std::vector<Symbol*> terminals (void);
  std::vector<Symbol*> nonterminals (void);
};

std::ostream &operator<< (std::ostream &output, const Grammar &grammar);

#endif
