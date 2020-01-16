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
#include "LALR1.h"

LALR1::LALR1()
{
  this->type = "LALR1"; 
}

LALR1::LALR1(Grammar* grammar)
{
  this->type = "LALR1";
  this->grammar = grammar;
  // expand grammar
  this->grammar->expand_grammar();

  // calculate first and follow sets
  this->follow = this->grammar->follow();
  this->first = this->grammar->first();

  // create item set from input grammar
  LR1::create_item_set();
  create_automata();
}

LALR1::~LALR1()
{
}

void LALR1::create_automata(void)
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

  // Just to be sure that we are expanding all possible items on the states
  Parser::expandStates();
}

State* LALR1::createState(State* newState)
{
  // check if state already exists
  for(State* s : this->states) { 
    if (newState->haveSameKernelItems(s)) {
      // add the new lookaheads to this state
      for(Item* i: newState->kernel)
        s->addItemToKernel(i);  
      for(Item* i: newState->item_set)
        s->addItemToItemsSet(i);  
      return s;
    }
  }
  
  // the state doesn't exists, add new state with a new unique id
  int id = Parser::getNextStateId();
  newState->setId(id);
  newState->setItemSet(Parser::closure(newState->all_items));
  this->states.insert(newState);
  return newState;
}

void LALR1::createTransitionStates(State* state)
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
    new_state = LALR1::createState(new_state);
    new_state->setItemSet(closure(new_state->kernel));
    std::pair<Symbol*, State*> transition = std::make_pair(s, new_state);
    state->transitions.insert(transition);
  }
}

std::set<Item*> LALR1::getProductionOfItem(Item* item)
{
  std::set<Item*> items_set;
  std::tuple<int, bool, Symbol*> dot = item->dot;
  Symbol* s = std::get<2>(dot); // symbol that the dot precedes

  // if item is closed, the item can't be expanded
  if(!(std::get<1>(dot)) || s->terminal)
    return items_set;    

  // for each item of the parser
  for(Item* it: this->items) {

      // check if it starts with a dot AND if the head is equal to the symbol that the dot precedes
      if(std::get<0>(it->dot) == 0 && s == it->rule->head) {
        // Now we need to decide wich lookaheads are possible for this item in this state
        
        // If there is only one symbol after the dot, we add items that have the i's lookahead (i is the item in the state item set)
        if(item->rule->body_size - std::get<0>(dot) == 1) {
          if(it->lookahead[0] == item->lookahead[0])
            items_set.insert(it);
        // if there are more symbols in the rule body
        } else {
          // we add first of that symbol (if not terminal) or the symbol itself (if terminal)
          // Non terminal symbol
          if(!(item->rule->body[std::get<0>(dot)+1]->terminal)) {
            // if the lookahead of our item is in the first set of the symbol that is next to it (item->rule->body[std::get<0>(dot)+1)
            if(Parser::isLookaheadInFirst(item->rule->body[std::get<0>(dot)+1], it->lookahead[0]))
              items_set.insert(it);
          
            // need to check if the empty symbol is in first of the subsequent symbol
            if(Parser::isEmptyInFirst(item->rule->body[std::get<0>(dot)+1])){
              // if so, we consider all follow symbols as valid lookaheads
              if(Parser::isLookaheadInFollow(item->rule->body[std::get<0>(dot)+1], it->lookahead[0]))
                items_set.insert(it);
            }

          // Next symbol (i->rule->body[std::get<0>(dot)+1]) is terminal, it becomes the lookahead 
          } else {
            if(it->lookahead[0] == item->rule->body[std::get<0>(dot)+1])
              items_set.insert(it);
          }
        }  
      }
    }

  return items_set;
} 

/* 
  The method for printing the automata is different for LALR1, we want to reduce the
  number of items printed by grouping items lookahead symbols with same rule and dot
  in the same item:

    A => a, b
    A => a, $

  Is now printed by

    A => a , b $
*/
void LALR1::print_automata(void)
{
  std::cout << std::endl << this->type << " automata:" << std::endl;
  
  // maps the items with the same Rule and group their lookaheads for each state
  std::vector< std::pair<Item*, std::vector<Symbol*> > >item_map;
  std::vector< std::pair<Item*, std::vector<Symbol*> > >::iterator itmap;
  bool found;

  for(State* s : this->states) {
    item_map.clear();
    std::cout << "State " << s->id << ":" << std::endl;

    // check if kernel items have the same body
    for (Item* item: s->kernel) {
      // find out if the item in the item_map
      found = false;
      for (itmap = item_map.begin(); itmap != item_map.end(); itmap++){
        if ((*itmap).first->isBodyEqual(item)) {
          (*itmap).second.push_back(item->lookahead[0]);
          found = true;
          break;
        }
      }
      // if item was not seen before, add it to the vector
      if(!found){
        item_map.push_back(std::make_pair( item, item->lookahead));
      }
    } 

    // print kernel items
    for (itmap = item_map.begin(); itmap != item_map.end(); itmap++) {
      (*itmap).first->printItemWithoutLookahead();

      // now we print our grouped lookaheads
      std::cout << ", ";
      for(Symbol* symbol : (*itmap).second) 
        std::cout << *symbol << " ";
    }
    std::cout << std::endl << "---------------" << std::endl;

    item_map.clear();
    // now we do the same for the items in the closure of the state
    for (Item* item: s->item_set) {
      // find out if the item in the item_map
      found = false;
      for (itmap = item_map.begin(); itmap != item_map.end(); itmap++){
        if ((*itmap).first->isBodyEqual(item)) {
          (*itmap).second.push_back(item->lookahead[0]);
          found = true;
          break;
        }
      }
      // if item was not seen before, add it to the vector
      if(!found){
        item_map.push_back(std::make_pair( item, item->lookahead));
      }
    }

    // print state items
    for (itmap = item_map.begin(); itmap != item_map.end(); itmap++) {
      (*itmap).first->printItemWithoutLookahead();

      // now we print our grouped lookaheads
      std::cout << ", ";
      for(Symbol* symbol : (*itmap).second) 
        std::cout << *symbol << " ";
      std::cout << std::endl;  
    }
    s->printTransitions();
  }
}