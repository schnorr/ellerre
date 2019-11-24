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
  // expand grammar
  this->grammar->expand_grammar();

  // calculate first and follow sets
  this->follow = this->grammar->follow();
  this->first = this->grammar->first();

  // create item set from input grammar
  create_item_set();
  create_automata();
}

LR1::~LR1()
{
}

void LR1::create_item_set(void) 
{
  // Produce the LR0 item set
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
      it_b--;
      Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_b)));
      addItem(i);
    }
  }
  std::set<Item*> items_lookahead;

  // For each item in the LR(0) item set, apply each element of the follow set as a lookahead
  for(Item *i:  this->items) {
    // for each entry <Head,follow> in the follow set
    for (auto& entry : this->follow) {
      
      // add the follow lookahead symbols for the head symbol of our items that matches entry.first 
      if(*(entry.first) == *i->rule->head) {
        if(i->rule->body.size() > 0) {
          // for each follow symbol for a given non terminal
          for (Symbol *s : entry.second) {
            std::vector<Symbol*> l{s};
            items_lookahead.insert(new Item(i->rule, i->dot, l)); 
          }   
        // if the rule produces the empty symbol, the valid lookahed is what follows it 
        } else {
          for (Symbol *s: getFollowOf(i->rule->head)) {
            std::vector<Symbol*> l{s};
            if(isLookaheadInFollow(i->rule->head, s))
              items_lookahead.insert(new Item(i->rule, i->dot, l));
          }
        }     
      } 
    }
  }
  // remove previous LR0 item set and assign the item set with the lookaheads to this->items
  this->clearItems();
  setItems(items_lookahead);
}

void LR1::create_automata(void)
{
  // This assumes that the first item in closure is the Starting item rule from the augmented grammar located at this->items.begin()
  std::vector<State*> states;
  bool change = true;
  int last_size = 0;
  Item* start = *this->items.begin(); 
  std::set<Item*> kernel = {start}; 
  std::set<Item*> items_set = closure(kernel);

  // Create the first state with its id, kernel and item_set
  State* starting_state = new State(0, kernel, items_set);
  createTransitionStates(starting_state);
  this->states.insert(starting_state);

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

void LR1::createTransitionStates(State* state)
{
  std::set<Symbol*> symbols;

  // add all possible transitions of the state to the symbols set (all symbols that the dot precedes)
  for(auto& i : state->all_items) {
    // if the dot precedes a symbol
    if(std::get<1>(i->dot))
      symbols.insert(std::get<2>(i->dot));
  }

  // for each transition symbol, look at the reachable states
  for(auto& s : symbols) {
    std::set<Item*> kernel;
    for(auto& i : state->all_items) {
      // if the current symbol s is found before the dot in an item
      // and its lookahead is in the state item first set
      if(std::get<1>(i->dot) && std::get<2>(i->dot) == s) {
        // find the item that have the dot in the next symbol 
        Item* nextItem = getNextItem(i);
        if(nextItem != NULL)
          kernel.insert(nextItem);
      }   
    }
    State* new_state = new State(kernel);
    new_state = createState(new_state);
    std::pair<Symbol*, State*> transition = std::make_pair(s, new_state);
    state->transitions.insert(transition);
  }
  
}

bool LR1::isLookaheadInFirst(Symbol* head, Symbol* lookahead)
{
  for(Symbol* s: this->first[head]) {
    if(lookahead == s)
      return true;
  }
  return false;
}

bool LR1::isLookaheadInFollow(Symbol* head, Symbol* lookahead)
{
  for(Symbol* s: this->follow[head]) {
    if(lookahead == s)
      return true;
  }
  return false;
}

bool LR1::isEmptyInFirst(Symbol* head)
{
    for(Symbol* s: this->first[head]) {
    if(s->empty){
      return true;
    }
  }
  return false;
}

std::set<Symbol*> LR1::getFollowOf(Symbol* s)
{
  return this->follow[s];
}

// The closure creates the item set of a given state
std::set<Item*> LR1::closure(std::set<Item*> kernel)
{
  std::set<Item*> items_set;
  std::set<Item*> new_set;
  std::tuple<int, bool, Symbol*> dot;
  Symbol* s;
  bool change = true;
  int last_size;

  // add the first productions from kernel items
  for(Item* item : kernel) {
      new_set = getProductionOfItem(item);
      set_union(items_set.begin(), items_set.end(), 
                new_set.begin(), new_set.end(), 
                std::inserter(items_set, items_set.begin()));
  }

  // while there are possible non-terminals items to expand
  while(change) {
    change = false;
    last_size = items_set.size(); 

    // for each item currently in the set
    for (Item* i: items_set) { 
      new_set = getProductionOfItem(i);
      set_union(items_set.begin(), items_set.end(), 
                new_set.begin(), new_set.end(), 
                std::inserter(items_set, items_set.begin()));          
    }

    // if a new item was added, reiterate the loop
    if(last_size != items_set.size())
      change = true;
  }
  
  return items_set;
}

State* LR1::createState(State* newState)
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
  newState->setItemSet(closure(newState->all_items));
  this->states.insert(newState);
  return newState;
}


std::set<Item*> LR1::getProductionOfItem(Item* item)
{
  std::set<Item*> items_set;
  std::tuple<int, bool, Symbol*> dot = item->dot;
  Symbol* s = std::get<2>(dot);

  // for each item of the parser
  for(Item* it: this->items) {
    
    // check if it starts with a dot and if the head is equal to the symbol that the dot precedes
    if(std::get<0>(it->dot) == 0 && s == it->rule->head) {
      bool isLookaheadEqual = false;
      for(int i=0; i<it->lookahead.size(); i++) {
        // check lookahead symbols
        if(it->lookahead[i] == item->lookahead[i])
          items_set.insert(it);
      }
    }
  }
  return items_set;
}

Item* LR1::getNextItem(Item* i)
{
  for(Item* it : this->items) {
    // if the item represents the same rule and the dot of i is greater by 1 having the same lookahead 
    if( it->rule == i->rule && 
        std::get<0>(i->dot)+1 == std::get<0>(it->dot) && 
        std::get<1>(i->dot) && 
        it->lookahead[0] == i->lookahead[0])
      return it;
  }

  return NULL;
}