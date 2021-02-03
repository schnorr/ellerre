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

/*! Creates all the possible transitions given a State and add
    them in the state item passed as parameter
    \param State* state, the state to create transitions
    \return void
*/
void Parser::createTransitionStates(State* state)
{
  std::set<Symbol*> symbols;
  
  // add all possible transitions of the state to the symbols set
  for(auto& i : state->all_items) {
    // check if the dot precedes a symbol (get<1>)
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


/*! Computes the closure of a state given its kernel
    \param std::set<Item*> kernel, the state kernel items
    \return std::set<Item*>  the complete item set of the state
*/
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


/*! Handles the automata creation for any type of automata as the different parsers
    override the methods getNextItem, getProductionOfItem, and create_item_set according
    to their specifications
*/
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

/*! Generates a .dot file representing the whole automata, this file can be
    processed by GraphViz to generate a visual representation of the automata
*/
void Parser::generateDotFile(std::string outFile)
{
  int port = 0;
  std::ofstream dotFile;
  dotFile.open (outFile);
  dotFile << "digraph g { graph [fontsize=30 labelloc=\"t\" label=\"\" splines=true overlap=false rankdir = \"LR\"]; ratio = auto;\n\n";

  // create the automata states
  for(State *s: this->states){
    port = 0; // port for node connections
    // opent the table tag
    dotFile << "\t\"state" << s->id << "\" [ style = \"filled\" penwidth = 1 fillcolor = \"white\" fontname = \"Courier New\" shape = \"Mrecord\" label = <<table border=\"0\" cellborder=\"0\" cellpadding=\"3\" bgcolor=\"white\">\n";

    // set the state identificator
    dotFile << "\t\t<tr><td bgcolor=\"black\" align=\"center\" colspan=\"2\"><font color=\"white\">State #" << s->id << "</font></td></tr>\n";

    // add kernel items of the state
    for(Item* i : s->kernel) {
      dotFile << "\t\t<tr><td align=\"left\" port=\"r" << port << "\"><font face=\"bold\">" << *i << "</font></td></tr>\n";
      port++;
    }
    // add production items of the state
    for(Item* i : s->item_set) {
      dotFile << "\t\t<tr><td align=\"left\" port=\"r" << port << "\"><font color=\"gray25\" face=\"bold\">" << *i << "</font></td></tr>\n";
      // dotFile << "\t\t<tr><td align=\"left\" port=\"r" << port << "\">" << *i << "</td></tr>\n";
      port++;
    }
    // close table tag
    dotFile << "\t</table>>];\n\n";
  }


  std::set<State*>::iterator it = this->states.begin();

  // Insert initial state mark:
  dotFile << "nowhere [style=invis,shape=point]\n";
  dotFile << "nowhere -> state" << (*it)->id << "[ penwidth = 3 fontsize = 22 fontcolor = \"black\"];\n";

  //Print other states:
  for(; it != this->states.end(); it++){
    for(std::pair<Symbol*, State* > t : (*it)->transitions){
      // t.first, t.second.
      dotFile << "state" << (*it)->id << " -> state" << t.second->id << "[ penwidth = 3 fontsize = 22 fontcolor = \"black\" label = \"" << t.first->str << "\" ];\n";
    }
  }

  dotFile << "}";
  dotFile.close();
}