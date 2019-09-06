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
#ifndef __RULE_H
#define __RULE_H
#include <cstddef>
#include <ostream>
#include <vector>
#include "symbol.h"

class Rule
{
public:
  Symbol *head;
  std::vector<Symbol*> body;
  
public:
  Rule();
  Rule(Symbol *head);
  Rule(Symbol *head, std::vector<Symbol*> body);
  ~Rule();

  void addHead(Symbol *head);
  void addBody(std::vector<Symbol*> body);
  void addToBody(Symbol *symbol);
};

std::ostream &operator<< (std::ostream &output, const Rule &rule);
bool operator== (const Rule &r1, const Rule &r2);

#endif
