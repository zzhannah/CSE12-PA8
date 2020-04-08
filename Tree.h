/*=============================================================================
 Tree.h
 @author Gary Gillespie
 @version 02/17/2020
=============================================================================*/
#ifndef TREE_H
#define TREE_H
#include <iostream>
using namespace std;
// debug message
static const char ALLOCATE[] = " - Allocating]\n";
static const char TREE[] = "[Tree ";
template <class Whatever>
struct TNode;
template <class Whatever>
class Tree {
        friend struct TNode<Whatever>;
        long occupancy;
        TNode<Whatever> * root;
        unsigned long tree_count;
        static int debug;
public:
        
        Tree (void);
        ~Tree (void);
        
        static void Set_Debug_On (void);
        static void Set_Debug_Off (void);
        unsigned long IsEmpty (void) const;
        unsigned long Insert (const Whatever &);
        unsigned long Lookup (Whatever &) const;
        unsigned long Remove (Whatever &);
        ostream & Write (ostream &) const;
};
#include "Tree.c"
#endif