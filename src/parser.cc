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
  int id = getMaxStateId();
  newState->setId(id);
  newState->setItemSet(closure(newState->kernel));
  this->states.insert(newState);
  return newState;
}

int Parser::getMaxStateId(void)
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

void Parser::createTransitionStates(State* state)
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
    new_state->setItemSet(closure(new_state->kernel));
    std::pair<Symbol*, State*> transition = std::make_pair(s, new_state);
    state->transitions.insert(transition);
  }
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

void Parser::LR0_item_set(void) 
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
      Rule* r = new Rule((*it_r)->head);
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

void Parser::LR0(void)
{ 
  // create item set from input grammar
  LR0_item_set();

  // The first item in closure is the Starting item rule from the augmented grammar
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