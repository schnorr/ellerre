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
#ifndef __ITEM_H
#define __ITEM_H
#include <vector>
#include <tuple>
#include <iostream>
#include "symbol.h"
#include "rule.h"

class Item
{
public:
  Rule* rule;
  std::vector<Symbol*> lookahead;
  std::tuple<int, bool, Symbol*> dot;

public:
  Item();
  Item(Rule* rule, std::tuple<int, bool, Symbol*> dot);
  Item(Rule* rule, std::tuple<int, bool, Symbol*> dot, std::vector<Symbol*> lookahead);
  ~Item();
};

std::ostream &operator<< (std::ostream &output, const Item &item);
bool operator== (const Item &i1, const Item &i2 );

#endif