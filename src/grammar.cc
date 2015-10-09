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


void Grammar::first (void)
{
  std::map<Symbol*,std::set<Symbol*> > firsts;
 
  bool keep_going = true;
  
  while (keep_going){

    //count
    int count = count_map (firsts);
  
    //for each rule
    for (Rule *rule : rules){

      //for each first symbol of rule's body
      for (Symbol *s : rule->body){
	if (s->terminal){
	  firsts[rule->head].insert (s);
	}else{
	  std::set<Symbol*> nts = firsts[s];
	  firsts[rule->head].insert (nts.begin(), nts.end());
	}
	break;
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
