
/*=============================================================================
 SymTab.h
 @author Gary Gillespie
 @version 02/17/2020
=============================================================================*/
#ifndef SYMTAB_H
#define SYMTAB_H
#include <iostream>
#include "Tree.h"
using namespace std;
template <class Whatever>
class SymTab : private Tree<Whatever> {
public:
       	using Tree<Whatever> :: Insert;
        using Tree<Whatever> :: Lookup;
        using Tree<Whatever> :: Remove;
        using Tree<Whatever> :: Write;
	using Tree<Whatever> :: IsEmpty;
        using Tree<Whatever> :: Set_Debug_On;
        using Tree<Whatever> :: Set_Debug_Off;
};
#endif