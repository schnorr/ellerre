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

#include "state.h"

State::State()
{
}

State::State(std::set<Item*> kernel)
{
  this->kernel = kernel;
}

State::State(int id,
             std::set<Item*> kernel, 
             std::set<Item*> item_set)
{
  this->id = id;
  this->kernel = kernel;
  this->item_set = item_set;
  set_union(this->kernel.begin(), this->kernel.end(), 
            this->item_set.begin(), this->item_set.end(), 
            std::inserter(this->all_items, this->all_items.begin()));
}

State::~State()
{
  this->kernel.clear();
  this->item_set.clear();
  this->transitions.clear();
}
 
void State::setId(int id)
{
  this->id = id;
}

void State::setItemSet(std::set<Item*> item_set)
{
  this->item_set = item_set;
  set_union(this->kernel.begin(), this->kernel.end(), 
            this->item_set.begin(), this->item_set.end(), 
            std::inserter(this->all_items, this->all_items.begin()));
}

void State::setKernel(std::set<Item*> kernel)
{
  this->kernel = kernel;
  set_union(this->kernel.begin(), this->kernel.end(), 
            this->item_set.begin(), this->item_set.end(), 
            std::inserter(this->all_items, this->all_items.begin()));
}
