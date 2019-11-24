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
#ifndef __PARSER_H
#define __PARSER_H
#include <algorithm>
#include "grammar.h"
#include "item.h"
#include "state.h"

class Parser
{
public:
  std::string type;
  Grammar* grammar;
  std::set<Item*> items;
  std::set<State*> states;
  std::map<Symbol*,std::set<Symbol*> > first;
  std::map<Symbol*,std::set<Symbol*> > follow;

public:
  Parser();
  Parser(Grammar* grammar);
  ~Parser();
  
  void setType(std::string type);
  void clearItems();
  void setItems(std::set<Item*> items);
  void clearStates();
  int getNextStateId(void);
  void addItem(Item *item);
  void print_automata(void);
  void print_item_set(void);
  void print_first_sets(void);
  void print_follow_sets(void);
  void createTransitionStates(State* state);
  std::set<Symbol*> getFollowOf(Symbol* s);
  State* createState(State* newState);
  std::set<Item*> closure(std::set<Item*> kernel);
  bool isLookaheadInFirst(Symbol* head, Symbol* lookahead);
  bool isLookaheadInFollow(Symbol* head, Symbol* lookahead);
  bool isEmptyInFirst(Symbol* head);
  void create_automata(void);
  void expandStates(void);
  // Abstract methods to be overridden
  virtual Item* getNextItem(Item* i) = 0;
  virtual std::set<Item*> getProductionOfItem(Item* item) = 0;
  virtual void create_item_set(void) = 0;
};

#endif