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
private:
  std::string type;
  Grammar* grammar;
  std::set<Item*> items;
  std::set<State*> states;

public:
  Parser();
  Parser(Grammar* grammar);
  ~Parser();
  
  void setType(std::string type);
  void clearItems();
  void clearStates();
  State* createState(State* state);
  int getMaxStateId(void);
  Item* getNextItem(Item* i);
  State* getTransitionState(State state, Symbol* s);
  void createTransitionStates(State* state);
  void addItem(Item *item);
  void print_item_set(void);
  void print_automata(void);
  void LR0_item_set(void);
  std::set<Item*> getProductionOfItem(Item* item);
  std::set<Item*> closure(std::set<Item*> kernel);
  void LR0(void);
  void LR1(void);
  void closure(void);
};

#endif