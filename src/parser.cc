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

Parser::Parser(Grammar* grammar)
{
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

std::set<Item*> Parser::closure(std::set<Item*> kernel)
{
  std::set<Item*> items_set;
  std::set<Item*> new_set;
  std::tuple<int, bool, Symbol*> dot;
  Symbol* s;
  bool change = true;
  int last_size;

  // add the first productions from kernel items
  for(Item* item : kernel) {
    dot = item->dot;
    s = std::get<2>(dot);

    // if the kernel item is not closed: the dot precedes any symbol that is non-terminal
    if(std::get<1>(dot) && !(s->terminal)) {
      new_set = getProductionOfItem(item);
      set_union(items_set.begin(), items_set.end(), 
            new_set.begin(), new_set.end(), 
            std::inserter(items_set, items_set.begin()));
    }
  }

  // while there are possible non-terminals items to expand
  while(change) {
    change = false;
    last_size = items_set.size(); 

    // for each item already in the set   
    for (Item* i: items_set) {                
      dot = i->dot;
      s = std::get<2>(dot);
     
      // item is closed, continue to next item
      if(!(std::get<1>(dot)) || s->terminal)
        continue;                             
     
      // for each item of the parser 
      for(Item* it: this->items) {             
        // check if it starts with a dot and if the head is equal to the symbol that the dot precedes
        if(std::get<0>(it->dot) == 0 && std::get<2>(dot) == it->rule->head) {
          items_set.insert(it);
        }
      }
    }
    // if a new item was added, reiterate the loop
    if(last_size != items_set.size())
      change = true;
  }
  return items_set;
}

std::set<Item*> Parser::getProductionOfItem(Item* item)
{
  std::set<Item*> items_set;
  std::tuple<int, bool, Symbol*> dot = item->dot;
  Symbol* s = std::get<2>(dot);
  // for each item of the parser
  for(Item* it: this->items) {
    // check if it starts with a dot and if the head is equal to the symbol that the dot precedes
    if(std::get<0>(it->dot) == 0 && s == it->rule->head) {
      int last_size = items_set.size();
      items_set.insert(it);
    }
  }
  return items_set;
}