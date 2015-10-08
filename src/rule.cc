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
#include "rule.h"

Rule::Rule()
{
  head = NULL;
  body.clear();
}

Rule::Rule(Symbol *head)
{
  this->head = head;
  body.clear();
}

Rule::Rule(Symbol *head, std::vector<Symbol*> body)
{
  this->head = head;
  this->body = body;
}

Rule::~Rule()
{
  body.clear();
}

void Rule::addHead(Symbol *head)
{
  this->head = head;
}

void Rule::addBody(std::vector<Symbol*> body)
{
  this->body = body;
}

void Rule::addToBody(Symbol *symbol)
{
  body.push_back(symbol);
}

std::ostream &operator<< (std::ostream &output, const Rule &rule)
{
  output << *(rule.head) << " => ";
  std::vector<Symbol*>::iterator it;
  std::vector<Symbol*> r = rule.body;
  for (it = r.begin(); it != r.end(); it++){
    output << *(*it) << " ";
  }
  return output;
}
