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
#include "parser.h"

Parser::Parser()
{
}

Parser::Parser(std::string type, Grammar* grammar)
{
  std::cout << "parser class\n";
  this->type = type;
  this->grammar = grammar;
}
  
Parser::~Parser()
{
  clearItems();
  clearStates();
}

void Parser::setType(std::string type)
{
  this->type = type;
}

void Parser::clearItems()
{
  this->items.clear();
}

void Parser::clearStates()
{
  this->states.clear();
}

State* Parser::createState(State* newState)
{
  // check if state already exists
  for(State* s : this->states) { 
    if (newState->haveSameKernel(s)) {
      return s;
    }
  }
  
  // the state doesn't exists, add new state with a new unique id
  int id = getNextStateId();
  newState->setId(id);
  newState->setItemSet(closure(newState->kernel));
  this->states.insert(newState);
  return newState;
}

int Parser::getNextStateId(void)
{
  int maxid = 0;
  for(State *s: this->states)
    if(s->id > maxid)
      maxid = s->id;

  return maxid+1;
}

Item* Parser::getNextItem(Item* i)
{
  for(Item* it : this->items) {
    // if the item represents the same rule and the dot of i is greater by 1 
    if(it->rule == i->rule && std::get<0>(i->dot)+1 == std::get<0>(it->dot) && std::get<1>(i->dot))
      return it;
  }

  return NULL;
}

void Parser::addItem(Item *item)
{
  this->items.insert(item);
}

void Parser::print_item_set (void)
{
  std::cout << std::endl << this->type << " item set:" << std::endl;
  for(auto& item : this->items){
    std::cout << *item;
  }
  
}
    
void Parser::print_automata(void)
{
  std::cout << std::endl << this->type << " automata:" << std::endl;
  for(State* s : this->states)
    std::cout << *s;
}


