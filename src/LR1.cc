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
#include "LR1.h"

LR1::LR1()
{
  this->type = "LR1"; 
}

LR1::LR1(Grammar* grammar)
{
  this->type = "LR1";
  this->grammar = grammar;
  // create item set from input grammar
  create_item_set();
  create_automata();
}

LR1::~LR1()
{
}

void LR1::create_item_set(void)
{
  std::vector<Rule*> r = this->grammar->rules; 
  std::vector<Rule*>::iterator it_r;
  std::vector<Symbol*> s;
  std::vector<Symbol*>::iterator it_s;
  int dot;
  bool hasNext;

  // for each rule in grammar
  for (it_r = r.begin(); it_r != r.end(); it_r++) {
    hasNext = true;
    s = (*it_r)->body;
    dot = 0;

    // if the rule produces the empty symbol
    if(s.size() == 1 && s[0]->empty) {
      hasNext = false;
      Rule *r = new Rule((*it_r)->head);
      Item *i = new Item(r, std::make_tuple(0, hasNext, s[0]));
      addItem(i);
    } else {
      // for each symbol in rule add a dot
      for (it_s = s.begin(); it_s != s.end(); it_s++) {
        Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_s)));
        addItem(i);
        dot++;
      }

      // the last dot does not precede any symbol
      hasNext = false;
      it_s--;
      Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_s)));
      addItem(i);
    }
  }
}

void LR1::create_automata(void)
{

}

std::set<Item*> LR1::closure(std::set<Item*> kernel)
{

}

State* LR1::getTransitionState(State state, Symbol* s)
{

}


void LR1::createTransitionStates(State* state)
{

}

std::set<Item*> LR1::getProductionOfItem(Item* item)
{
    
}