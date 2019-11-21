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
  std::cout << "first: \n";
  grammar->print_first_sets();
  std::cout << "follow: \n";
  grammar->print_follow_sets();
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

  // Get the follow set to determine LR1 lookaheads in the items
  std::map<Symbol*,std::set<Symbol*> > follows = this->grammar->follow();
  std::set<Item*> items_lookahead;

  // For each item in the LR(0) item set, apply each element of the follow set as a lookahead
  for(Item *i:  this->items){
    // for each entry <Head,follow> in the follow set
    for (auto& entry : follows) {
      // add the follow lookahead symbols for the head symbol of our items that matches entry.first 
      if(*(entry.first) == *i->rule->head) {
        // for each follow symbol for a given non terminal
        for (Symbol *s : entry.second) {
          std::vector<Symbol*> l{s};
          items_lookahead.insert(new Item(i->rule, i->dot, l)); 
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
  // MARCELO: for the first state it os ok

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

void LR1::createTransitionStates(State* state)
{
  std::set<Symbol*> symbols;

  // add all possible transitions of the state to the symbols set
  for(auto& i : state->all_items) {
    if(std::get<1>(i->dot))
      symbols.insert(std::get<2>(i->dot));
  }
  // for each transition symbol, look at the reachable states
  for(auto& s : symbols) {
    std::set<Item*> kernel;
      // if(isLookaheadInFirst(i->rule->head, i->lookahead[0])) {
        std::cout << "transition symbol " << *s << "\n";
      // }
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
    new_state->setItemSet(closure(new_state->kernel));
    std::pair<Symbol*, State*> transition = std::make_pair(s, new_state);
    state->transitions.insert(transition);
  }
  std::cout << "\n\n";
}


// && isLookaheadInFirst(i->rule->head, i->lookahead[0])

bool LR1::isLookaheadInFirst(Symbol* head, Symbol* lookahead)
{
  for(Symbol* s: this->first[head]) {
    if(lookahead == s)
      return true;
  }
  return false;
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
    dot = item->dot;
    s = std::get<2>(dot);

    // if the kernel item is not closed, and the dot precedes any symbol that is non-terminal
    if(std::get<1>(dot) && !(s->terminal)) {
      new_set = getProductionOfItem(item, this->first);
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
        // and the lookahead is in the first set of that head symbol
        if(std::get<0>(it->dot) == 0 && std::get<2>(dot) == it->rule->head && isLookaheadInFirst(it->rule->head, it->lookahead[0])) {
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
  newState->setItemSet(closure(newState->kernel));
  this->states.insert(newState);
  return newState;
}
}