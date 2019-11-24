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
  Parser::create_automata();
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
          for (Symbol *s: Parser::getFollowOf(i->rule->head)) {
            std::vector<Symbol*> l{s};
            if(Parser::isLookaheadInFollow(i->rule->head, s))
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

std::set<Item*> LR1::getProductionOfItem(Item* item)
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