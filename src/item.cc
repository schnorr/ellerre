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

Item::Item(Rule* rule, std::tuple<int, Symbol*> dot)
{
    this->rule = rule;
    this->dot = dot;
}

Item::Item(Rule* rule, std::tuple<int, Symbol*> dot, std::vector<Symbol*> lookahed)
{
    this->dot = dot;
    this->rule = rule;
    this->lookahed = lookahed;
}

Item::~Item()
{
    lookahed.clear();
}

void Item::print_item() 
{
  std::cout << this->rule->body[0]->str << " => ";
  
  for (auto& symbol : this->rule->body){
    std::cout << symbol->str << " ";
  }
  std::cout << std::endl;
}


std::ostream &operator<< (std::ostream &output, const Item &item){
  int dot = 0;
  output << *(item.rule->head) << " => "; 
  
  for (auto& symbol : item.rule->body){
    if(dot == std::get<0>(item.dot)){
      output << "\u26ab "; 
    } 
    output << symbol->str << " ";
    dot++;
  }
  if(dot == std::get<0>(item.dot)){
    output << "\u26ab "; 
  } 
  output << std::endl;
}
