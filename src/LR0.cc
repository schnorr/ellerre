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
  create_automata();
}

LR0::~LR0()
{
}

void LR0::create_item_set(void) 
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

void LR0::create_automata(void)
{ 
  // The first item in closure is the Starting item rule from the augmented grammar
  std::vector<State*> states;
  bool change = true;
  int last_size = 0;
  Item* start = *this->items.begin(); 
  std::set<Item*> kernel = {start}; 
  std::set<Item*> items_set = Parser::closure(kernel);

  // Create the first state with its id, kernel and item_set
  State* starting_state = new State(0, kernel, items_set);
  createTransitionStates(starting_state);
  this->states.insert(starting_state);
  
  // TODO: check if this while is really necessary
  // while new states appear in this->states
  while(change) {
    change = false;
    last_size = this->states.size();

    // for each state in the current states
    for(State* s : this->states) {
      // expand the state and create the transition states
      createTransitionStates(s);
      this->states.insert(s);
      
      // for each state in the transitions
      for(auto& ts : s->transitions) {
        // expand the state and create the transition states
        createTransitionStates(ts.second);
        this->states.insert(ts.second);
      }
    }
    if(last_size != this->states.size())
      change = true;
  }
}

void LR0::createTransitionStates(State* state)
{
  std::set<Symbol*> symbols;
  int last_size;
  // add all possible transitions of the state to the symbols set
  for(auto& i : state->all_items) {
    if(std::get<1>(i->dot))
      symbols.insert(std::get<2>(i->dot));
  }
  // for each transition symbol, look at the reachable states
  for(auto& s : symbols) {
    std::set<Item*> kernel;

    for(auto& i : state->all_items) {
      // if the current symbol s is found before the dot in an item
      if(std::get<1>(i->dot) && std::get<2>(i->dot) == s) {
        // find the item that have the dot in the next symbol 
        Item* nextItem = getNextItem(i);
        if(nextItem != NULL)
          kernel.insert(nextItem);
      }   
    }
    State* new_state = new State(kernel);
    new_state = createState(new_state);
    new_state->setItemSet(Parser::closure(new_state->kernel));
    std::pair<Symbol*, State*> transition = std::make_pair(s, new_state);
    state->transitions.insert(transition);
  }
}
