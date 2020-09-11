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
#include "LR0.h"

LR0::LR0()
{
  this->type = "LR0"; 
}

LR0::LR0(Grammar* grammar)
{
  this->type = "LR0";
  this->grammar = grammar;
  // create item set from input grammar
  create_item_set();
  Parser::create_automata();
}

LR0::~LR0()
{
}

void LR0::create_item_set(void) 
{
  std::vector<Rule*> r = this->grammar->rules; 
  std::vector<Rule*>::iterator it_r;
  std::vector<Symbol*> body;
  std::vector<Symbol*>::iterator it_b;
  int dot;
  bool hasNext;

  // for each rule in grammar
  for (it_r = r.begin(); it_r != r.end(); it_r++) {
    hasNext = true;
    body = (*it_r)->body;
    dot = 0;

    // if the rule produces the empty symbol
    if(body.size() == 1 && body[0]->empty) {
      hasNext = false;
      Rule *r = new Rule((*it_r)->head);
      Item *i = new Item(r, std::make_tuple(0, hasNext, body[0]));
      addItem(i);
    } else {

      // for each symbol in rule add a dot
      for (it_b = body.begin(); it_b != body.end(); it_b++) {
        Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_b)));
        addItem(i);
        dot++;
      }

      // the last dot does not precede any symbol
      hasNext = false;
      it_b--; // get the symbol before the dot
      Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_b)));
      addItem(i);
    }
  }
}

std::set<Item*> LR0::getProductionOfItem(Item* item)
{
  std::set<Item*> items_set;
  std::tuple<int, bool, Symbol*> dot = item->dot;
  Symbol* s = std::get<2>(dot);
  // for each item of the parser
  for(Item* it: this->items) {
    // check if the dot of item hasNext
    if(std::get<1>(item->dot)) {
      // check if it starts with a dot and if the head is equal to the symbol that the dot precedes
      if(std::get<0>(it->dot) == 0 && s == it->rule->head) {
        int last_size = items_set.size();
        items_set.insert(it);
      }
    }
  }
  return items_set;
}

Item* LR0::getNextItem(Item* i)
{
  for(Item* it : this->items) {
    // if the item represents the same rule and the dot of i is greater by 1 
    if(it->rule == i->rule && std::get<0>(i->dot)+1 == std::get<0>(it->dot) && std::get<1>(i->dot))
      return it;
  }

  return NULL;
}