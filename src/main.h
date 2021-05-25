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
#ifndef __MAIN_H
#define __MAIN_H
#include <iostream>
#include <vector>
#include "symbol.h"
#include "rule.h"
#include "grammar.h"
#include "parser.h"
#include "LR0.h"
#include "LR1.h"
#include "LALR1.h"

extern int yyparse(void);
extern Grammar *grammar;
inline void show_usage(std::string name){
  std::cerr << "Usage: " << name << " < input_file" << std::endl;
} 
#endif
