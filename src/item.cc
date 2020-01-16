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
#include "item.h"
#include "symbol.h"

Item::Item()
{
}

Item::Item(Rule* rule, std::tuple<int, bool, Symbol*> dot)
{
    this->rule = rule;
    this->dot = dot;
}

Item::Item(Rule* rule, std::tuple<int, bool, Symbol*> dot, std::vector<Symbol*> lookahead)
{
    this->dot = dot;
    this->rule = rule;
    this->lookahead = lookahead;
}

Item::~Item()
{
  lookahead.clear();
}

std::ostream &operator<< (std::ostream &output, const Item &item)
{
  int dot = 0;
  output << *(item.rule->head) << " \u21d2 "; 
  
  if (item.rule->body.size() > 0) {

    // print the item rule symbols with the dot
    for (auto& symbol : item.rule->body) {

      if(dot == std::get<0>(item.dot)) {
        output << "• "; 
      } 
      output << *symbol << " ";
      dot++;
    }

    if (dot == std::get<0>(item.dot)){
      output << "• "; 
    } 

    // print lookahead (if any)
    if (item.lookahead.size() > 0) {
      for (auto& symbol : item.lookahead) {
        output << ", " << *symbol;
      }
    }

    output << std::endl;
  } else {
    output << "• "; 
    // print lookahead (if any)
    if (item.lookahead.size() > 0) {
      for (auto& symbol : item.lookahead) {
        output << ", " << *symbol;
      }
    }
    output << std::endl;
  }
  return output;
}

bool operator==(const Item &i1, const Item &i2)
{
  // compare rules
  if(!(i1.rule == i2.rule))
    return false;

  // compare lookaheads
  for(int i=0; i < i1.lookahead.size(); i++) {
    if(!(i1.lookahead[i] == i2.lookahead[i]))
      return false;
  }
  
  // compare the dot position
  if(std::get<0>(i1.dot) != std::get<0>(i2.dot))
    return false;

  return true;
}

// here we ignore lookahead
bool Item::isBodyEqual(Item* i2)
{
  // compare rules
  if(!(this->rule == i2->rule))
    return false;

  // compare the dot position
  if(std::get<0>(this->dot) != std::get<0>(i2->dot))
    return false;

  return true;
}

void Item::printItemWithoutLookahead(void)
{
  int dot = 0;
  std::cout << *this->rule->head << " \u21d2 "; 
  
  if (this->rule->body.size() > 0) {

    // print the item rule symbols with the dot
    for (auto& symbol : this->rule->body) {

      if(dot == std::get<0>(this->dot)) {
        std::cout << "• "; 
      } 
      std::cout << *symbol << " ";
      dot++;
    }

    if (dot == std::get<0>(this->dot)){
      std::cout << "• "; 
    } 
  } else {
    std::cout << "• ";
  }
}