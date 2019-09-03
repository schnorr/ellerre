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
#ifndef __GRAMAR_H
#define __GRAMAR_H
#include <map>
#include <set>
#include <iostream>
#include <ostream>
#include <vector>
#include "symbol.h"
#include "rule.h"
#include "item.h"

class Grammar
{
private:
  Symbol *empty;
  Symbol *dollar;
  
public:
  std::vector<Rule*> rules;
  std::map<std::string,Symbol*> symbols;
  std::vector<Item*> items;
  //std::map<std::string, std::vector<Item*> items> states;

public:
  Grammar();
  ~Grammar();
  void addRule(Rule *rule);
  void addItem(Item *item);
  Symbol *getSymbol(char *str);
  Symbol *getEmptySymbol ();
  Symbol *getDollarSymbol ();
  std::map<Symbol*,std::set<Symbol*> > first (void);
  std::map<Symbol*,std::set<Symbol*> > follow (void);
  
  void print_item_set (void);
  void print_first_sets (void);
  void print_follow_sets (void);
  void LR0_item_set (void);
  std::set<Item*> closure(Item* item);

private:
  std::vector<Rule*> rulesForNonterminal(Symbol*);
  bool deriveToEmpty(Symbol*);
  std::vector<Symbol*> terminals (void);
  std::vector<Symbol*> nonterminals (void);
  void print_set (std::map<Symbol*,std::set<Symbol*> > map);
  void augmented_grammar (Rule* start);
};

std::ostream &operator<< (std::ostream &output, const Grammar &grammar);

#endif
