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
#include "grammar.h"

Grammar::Grammar()
{
  empty = new Symbol(true);
}

Grammar::~Grammar()
{
  //delete all rules
  while (rules.size() != 0){
    Rule *r = rules.back();
    rules.pop_back();
    delete r;
  }

  //delete all symbols
  while (symbols.size() != 0){
    Symbol *s = (symbols.begin())->second;
    symbols.erase(symbols.begin());
    delete s;
  }

  delete empty;
}

void Grammar::addRule(Rule *rule)
{
  rule->head->terminal = false;
  rules.push_back(rule);
}

Symbol *Grammar::getSymbol(char *str)
{
  std::map<std::string,Symbol*>::iterator it = symbols.find(std::string(str));
  if (it != symbols.end()){
    return (*it).second;
  }else{
    Symbol *newSymbol = new Symbol(str, true);
    symbols[str] = newSymbol;
    return newSymbol;
  }
}

Symbol *Grammar::getEmptySymbol ()
{
  return empty;  
}

std::vector<Symbol*> Grammar::terminals (void)
{
  //find all non-terminals
  std::vector<Symbol*> terminals;
  std::map<std::string,Symbol*>::iterator itmap;
  for (itmap = symbols.begin(); itmap != symbols.end(); itmap++){
    if ((*itmap).second->terminal == true){
      terminals.push_back ((*itmap).second);
    }
  }
  return terminals;
}

std::vector<Symbol*> Grammar::nonterminals (void)
{
  std::vector<Symbol*> nonterminals;
  std::map<std::string,Symbol*>::iterator itmap;
  for (itmap = symbols.begin(); itmap != symbols.end(); itmap++){
    if ((*itmap).second->terminal == false){
      nonterminals.push_back ((*itmap).second);
    }
  }
  return nonterminals;
}

bool Grammar::deriveToEmpty (Symbol *symbol)
{
  if (symbol->terminal) return false;
  //TODO
}

std::vector<Rule*> Grammar::rulesForNonterminal (Symbol *symbol)
{ 
  std::vector<Rule*> ret;
  if (symbol->terminal){
    throw "Error";
  }
  
  for (Rule *r : rules){
    if (r->head == symbol){
      ret.push_back (r);
    }
  }
  return ret;
}

static int count_map (std::map<Symbol*,std::set<Symbol*> > map){
  int ret = map.size();
  for (auto &entry : map){
    ret += entry.second.size();
  }
  return ret;
}

static void print_set (std::set<Symbol*> set)
{
  std::cout << "Printing set " << std::endl;//set << std::endl;
  for (Symbol *s : set){
    std::cout << *s << std::endl;
  }
  std::cout << "End Printing set " << std::endl;//set << std::endl;

}


void Grammar::first (void)
{
  std::map<Symbol*,std::set<Symbol*> > firsts;
 
  bool keep_going = true;

  bool DEBUG = false;
  
  while (keep_going){
    if (DEBUG){
      std::cout << "====================" << std::endl;
    }
    //count
    int count = count_map (firsts);
  
    //for each rule
    for (Rule *rule : rules){

      if (DEBUG){
        std::cout << "=> Head: " << *(rule->head) << std::endl;
      }

      /* Let's check if all the rule's body derives to empty. It that
       * is the case, we can safely add the empty symbol to the first
       * set of the current rule's head symbol. */
      
      /* Other checks: for each first symbol of rule's body */
      bool body_derives_empty = true;
      for (Symbol *s : rule->body){
        
	if (s->terminal){
          /* Body member is a terminal */
          firsts[rule->head].insert (s);
          body_derives_empty = false;
	  break;
	}else{
          /* Body member is a non-terminal */
          
          /* I will add to the rule's head the first set of the
           * current body member, since it is part of the body that is
           * being currently considered. */

          if(DEBUG){
            std::cout << "==> Analyze body member: " << *s << std::endl;
          }

	  std::set<Symbol*> nts = firsts[s];

          //add everyone except empty symbol
          std::set<Symbol*> copy = nts;

          if(DEBUG){ //these are debug messages
            print_set (copy);
            std::set<Symbol*>::iterator emptyPresent = nts.find(getEmptySymbol());
            std::cout << "==> emptyPresent is " << (emptyPresent != nts.end()) << std::endl;
          }

          /* I have to add all the symbols of the first set of the
          current body member which is non-terminal, except the empty
          symbol (this last one should be added to the rule's head
          symbol only if the whole body derives to empty (checked
          above). */
          copy.erase (getEmptySymbol());

          if(DEBUG){ //these are debug messages
            std::cout << "==> Adding only:" << std::endl;
            print_set (copy);
          }

          /* Do the union */
	  firsts[rule->head].insert (copy.begin(), copy.end());

          /* Now, let's check wether the current body member derives
          to empty. I can very easily know that simply by looking to
          the current first set of this body member. */
          bool deriveToEmpty = nts.find(getEmptySymbol()) != nts.end();

          if(DEBUG){ //these are debug messages
            if (deriveToEmpty == true){
              std::cout << "==> Body member " << *s << " derive to empty, will continue." << std::endl;
            }else{
              std::cout << "==> Body member " << *s << " DOES NOT derive to empty, will NOT continue." << std::endl;
            }
          }
          
          if (deriveToEmpty == false){
            /* This body member which is a nonterminal does not derive
            to empty. That means that the rule's head symbol cannot
            derive to empty either. Mark this and stop analyzing this
            rule's body. */
            body_derives_empty = false;
            break; 
          }
	}
      }

      if (body_derives_empty == true){
        firsts[rule->head].insert (getEmptySymbol());
      }
    }

    if ((count_map (firsts) - count) == 0){
      keep_going = false;
    }
    
  }

  for (auto& entry : firsts){
    std::cout << *(entry.first) << " -- ";
    for (Symbol *s : entry.second){
      std::cout << *s << " ";
    }
    std::cout << std::endl;
  }
  return;
}

std::ostream &operator<< (std::ostream &output, const Grammar &grammar)
{
  //count number of non-terminals
  int ntc = 0;
  std::map<std::string,Symbol*>::iterator itmap;
  std::map<std::string,Symbol*> map = grammar.symbols;
  for (itmap = map.begin(); itmap != map.end(); itmap++){
    if (!((*itmap).second)->terminal){
      ntc++;
    }
  }

  output << "Grammar with " << grammar.rules.size() << " rules and " << grammar.symbols.size() << " symbols (" << ntc << " non-terminals):" << std::endl;
  std::vector<Rule*>::iterator it;
  std::vector<Rule*> r = grammar.rules;
  for (it = r.begin(); it != r.end(); it++){
    output << *(*it);
    if ((it+1) != r.end()){
      output << std::endl;
    }
  }
  return output;
}
