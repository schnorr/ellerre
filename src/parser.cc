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

void Parser::setItems(std::set<Item*> items)
{
  this->items = items;
}

void Parser::clearStates()
{
  this->states.clear();
}

int Parser::getNextStateId(void)
{
  int maxid = 0;
  for(State *s: this->states)
    if(s->id > maxid)
      maxid = s->id;

  return maxid+1;
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

void Parser::print_first_sets(void)
{
  std::cout << std::endl << "First set: " << std::endl;
  this->grammar->print_first_sets();
}

void Parser::print_follow_sets(void)
{
  std::cout << std::endl << "Follow set: " << std::endl;
  this->grammar->print_follow_sets();
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

std::set<Symbol*> Parser::getFollowOf(Symbol* s)
{
  return this->follow[s];
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
  newState->setItemSet(closure(newState->all_items));
  this->states.insert(newState);
  return newState;
}

// The closure creates the item set of a given state
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

bool Parser::isLookaheadInFirst(Symbol* head, Symbol* lookahead)
{
  for(Symbol* s: this->first[head]) {
    if(lookahead == s)
      return true;
  }
  return false;
}

bool Parser::isLookaheadInFollow(Symbol* head, Symbol* lookahead)
{
  for(Symbol* s: this->follow[head]) {
    if(lookahead == s)
      return true;
  }
  return false;
}

bool Parser::isEmptyInFirst(Symbol* head)
{
    for(Symbol* s: this->first[head]) {
    if(s->empty){
      return true;
    }
  }
  return false;
}

void Parser::create_automata(void)
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

void Parser::expandStates(void)
{
  for(State* s: this->states) {
    s->setItemSet(closure(s->kernel));
    s->updateAllItems();
  }
}