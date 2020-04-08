/******************************************************************************
                                                        Han Zhang
                                                        CSE 12, Winter 2020
                                                        February 27, 2020
                                                        cs12wi20kj
                                Assignment Eight
File Name:      Tree.c
Description:    Implement a Tree to allow variable assignment to the 
		store and find. Allow user to insert, write and lookup  
		and delete. Work together with Driver.h.
******************************************************************************/
#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;
// messages
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";
template <class Whatever>
int Tree<Whatever>::debug = 0;
static long tree_counter = 0;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define THRESHOLD 2
/*=============================================================================
Struct TNode
Description:		Allocate and deallocate TNodes for tree. Able to write,
			remove, insert, set height and balance, replace and 
			remove max, write_AllTNodes and delete_AllTNodes.
Data Fields:		data		- to store element for tnode
			*left		- left child of this tnode
			*right		- right child of this tnode
			&occupancy	- how many elements in this tree
			hasBeenDeleted	- flag for deleted
			balance		- balance of the branch
			height		- height of the node
			&tree_count	- counter for which tree this is
Functions:	TNode :: constructor1	- add occupancy
		TNode :: constructor2	- add occupancy
		TNode :: destructor	- minus occupancy
		TNode :: Insert		- let user insert element to the tree
		TNode :: ReplaceAndRemoveMax- use algorithm to fix the places
						of TNodes after remove
		TNode :: Remove		- let user remove element from tree
		TNode :: SetHeightAndBalance- fix height and balance for 
						each tnode
		TNode :: write_AllTNodes- write out the tree
		TNode :: delete_AllTNodes- remove all elements
=============================================================================*/
template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);
template <class Whatever>
struct TNode {
        long balance;
        Whatever data;
        long height;
        TNode<Whatever> * left;
        long & occupancy;
        TNode<Whatever> * right;
        unsigned long & tree_count;
        
/***************************************************************************
% Routine Name : TNode :: constructor 1
% File :         Tree.c
% 
% Description :  Initializes default values and add occupancy
***************************************************************************/
        TNode (const Whatever & element, Tree<Whatever> & theTree)
                : balance (0), data (element), height (0), left (0), 
                occupancy (theTree.occupancy), right (0), 
                tree_count (theTree.tree_count) {
		//add occupancy
		occupancy++;
        }
        
/***************************************************************************
% Routine Name : TNode :: constructor 2
% File :         Tree.c
% 
% Description :  Initializes default values and add occupancy
***************************************************************************/
        TNode (const Whatever & element, TNode<Whatever> & parentTNode)
        : balance (0), data (element), height (0), left (0), 
                occupancy (parentTNode.occupancy), right (0), 
                tree_count (parentTNode.tree_count) {
		//add occupancy
                occupancy++;
        }
/***************************************************************************
% Routine Name : TNode :: destructor 
% File :         Tree.c
% 
% Description :  decrease occupancy
***************************************************************************/
        ~TNode (void) {
		//decrease occupancy
		occupancy--;
        }
/***************************************************************************
% Routine Name : TNode :: delete_AllTNodes 
% File :         Tree.c
% 
% Description :  Recursive function to delete all TNodes
***************************************************************************/
        void delete_AllTNodes (void) {
		//recursive to check if have right or left
		if (left) {
			left->delete_AllTNodes();
		}
		if (right) {
			right->delete_AllTNodes();
		}
		//delete this node
		delete this;
        }
/***************************************************************************
% Routine Name : TNode :: insert
% File :         Tree.c
% 
% Description :  Allow user to insert element. Distinguish different
%		 situations and call recursively to insert or update
%		 element.
% Param :	 element	- the element user want to insert
%		 PointerInParent- the search path to track in recusion
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
        unsigned long Insert (const Whatever & element, 
                              TNode<Whatever> *& PointerInParent);
/***************************************************************************
% Routine Name : TNode :: Lookup
% File :         Tree.c
% 
% Description :  Allow user to search for element. Distinguish different 
		 situation and call recursively to find element.
% Param :	 element	- the element user want to insert
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
        // OPTIONAL TNode :: Lookup => uncomment if implementing recursively
        unsigned long Lookup(Whatever & element) const;
/***************************************************************************
% Routine Name : TNode :: ReplaceAndRemoveMax
% File :         Tree.c
% 
% Description :  Called when removing a TNode with 2 children, replaces that 
%		 TNode with the maximum TNode in its left subtree to maintain 
%		 the Tree structure.
% Param :	 targetTNode	- the TNode user want to remove
%		 PointerInParent- the search path to track in recusion
***************************************************************************/
        void ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
                TNode<Whatever> *& PointerInParent) {
		
		//then all the way right
		if (right) {
			right->ReplaceAndRemoveMax(*this, right);
			SetHeightAndBalance(PointerInParent);	
		//delete the biggest on the left brach
		} else {
			targetTNode.data = data;
			PointerInParent = left;
			delete this;
		}
        }
/***************************************************************************
% Routine Name : TNode :: Remove
% File :         Tree.c
% 
% Description :  Removes the matching data from the binary tree. Returns true 
%		 or false indicating success of removal.
% Param :	 elementTNode	- the element user want to remove
%		 PointerInParent- the search path to track in recusion
%		 fromSHB	- if this remove is called from SHB
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
        unsigned long Remove (TNode<Whatever> & elementTNode, 
                TNode<Whatever> *& PointerInParent,
                long fromSHB = FALSE) {
		/*debug msg*/
		if (Tree<Whatever>::debug == 1) {
			cerr <<TREE<<tree_count<<COMPARE<<
				(const char*)PointerInParent->data<<
				AND<<(const char*)data<<"]\n";
		}
		//if equals, check children
		if (data == elementTNode.data) {
			//save data in elementTNode
			elementTNode.data = data;
			//if no children
			if (left == NULL && right == NULL) {
				//set pip to null to prevent access
				PointerInParent = NULL;
				//delete this
				delete this;
			//if left children
			} else if (left == NULL) {
				//attach right to parent
				PointerInParent = right;
				//delete right
				delete this;
			//if right children
			} else if (right == NULL) {
				//attach left to parent
				PointerInParent = left;
				//delete left
				delete this;
			//if has two children, replace & remove min one
			} else {
				//create new node
				TNode<Whatever> working(data, *this);
				//call replace
				left->ReplaceAndRemoveMax(working, left);
				//change this value
				data = working.data;
				/*debug msg*/
				if (Tree<Whatever>::debug == true) {
					cerr <<TREE<<tree_count<<CHECK<<
					(const char*)PointerInParent->data
					<<"]\n";
					cerr <<TREE<<tree_count<<REPLACE<<
					(const char*)PointerInParent->data<<
					"]\n";
				}
				//set height and balance if not from SHB
				if (fromSHB == FALSE) {
					//update height & balance
					SetHeightAndBalance(PointerInParent);
				}
			}
			
			return 1;
	
		//if element greater than data, go right
		} else if (elementTNode.data > data) {
			//if cannot go right, failed
			if (!right) {
				return 0;
			//else, go right
			} else {
				//call recursion
				long result = right->Remove(elementTNode, 
						right, fromSHB);
				//if not from SHB, set height and balance
				if (fromSHB == FALSE) {	
					//update height & balance
					SetHeightAndBalance(PointerInParent);
				}
				return result;
			}
		//if element less than data, go left
		} else {
			//if cannot go left, failed
			if (!left) {
				return 0;
			//else, go left
			} else {
				//call recursion
				long result = left->Remove(elementTNode, 
						left, fromSHB);
				//if not from SHB, call set height and balance
				if (fromSHB == FALSE) {
					//update height & balance
					SetHeightAndBalance(PointerInParent);
				}
				return result;
			}
		}
        }
/***************************************************************************
% Routine Name : TNode :: SetHeightAndBalance
% File :         Tree.c
% 
% Description :  Updates the height and balance of the current TNode. 
% Param :	 PointerInParent- the search path to track in recusion
***************************************************************************/
        void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) {
		long leftchild;		//keep track leftchild's height
		long rightchild;	//keep track rightchild's height
		/*debug msg*/
		if (Tree<Whatever>::debug == 1) {
			cerr <<TREE<<tree_count<<UPDATE<<(const char*)data
				<<"]\n";
		}
		//set default height to -1
		leftchild = rightchild = -1;
		//if have left or right, assign childheight to var
		if(left) {leftchild = left->height;}
		if(right) {rightchild = right->height;}
		//height = tallest child + 1
		if(leftchild > rightchild) {height = 1 + leftchild;}
		else {height = 1 + rightchild;}
		//set balance
		balance = leftchild - rightchild;
		//if absolute balance > 2, remove and reinsert this node
		if(abs(balance) > THRESHOLD) {
			//create a working node
			TNode<Whatever> working(data, *this);
			//remove and reinsert
			Remove(working, PointerInParent, TRUE);
			PointerInParent->Insert(working.data, PointerInParent);
		}
        }
        ostream & Write_AllTNodes (ostream & stream) const {
                if (left)
                        left->Write_AllTNodes (stream);
                stream << *this;
                if (right)
                        right->Write_AllTNodes (stream);
                return stream;
        }
};
/***************************************************************************
% Routine Name : Tree :: Set_Debug_On
% File :         Tree.c
% 
% Description :  turns on debug in tree
***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On(void) {
	Tree<Whatever>::debug = 1;
}
/***************************************************************************
% Routine Name : Tree :: Set_Debug_Off
% File :         Tree.c
% 
% Description :  turns off debug in tree
***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off(void) {
	Tree<Whatever>::debug = 0;
}
/***************************************************************************
% Routine Name : TNode :: insert
% File :         Tree.c
% 
% Description :  Allow user to insert element. Distinguish different
%		 situations and call recursively to insert or update
%		 element.
% Param :	 element	- the element user want to insert
%		 PointerInParent- the search path to track in recusion
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
                                         TNode<Whatever> *& PointerInParent) {
	/*debug msg*/
	if (Tree<Whatever>::debug == 1) {
		cerr <<TREE<<tree_count<<COMPARE<<(const char*)element<<
			AND<<(const char*)data<<"]\n";
	}
	//if equals, update
	if (data == element) {
		//update element
		data = element;
		//debug msg
		if (Tree<Whatever>::debug == 1) {
			cerr <<TREE<<tree_count<<INSERT<<(const char*)element<<
				"]\n";
		}
		return 1;
	
	//if element greater than data, go right
	} else if (element > data) {
		//if cannot go right, element is new right
		if (!right) {
			/*debug msg*/
			if (Tree<Whatever>::debug == true) {
				cerr <<TREE<<tree_count<<INSERT<<
					(const char*)element<<"]\n";
			}
			//attach new node
			right = new TNode<Whatever>(element, *this);	
			//call function to set height and balance
			SetHeightAndBalance(PointerInParent);	
		//else, go right
		} else {
			long result = right->Insert(element, right);
			//call function to set height and balance
			SetHeightAndBalance(PointerInParent);
			return result;
		}
	//if element less than data, go left
	} else {
		//if cannot go left, element is new left
		if (!left) {
			/*debug msg*/
			if (Tree<Whatever>::debug == true) {
				cerr <<TREE<<tree_count<<INSERT<<
					(const char*)element<<"]\n";
			}
			//attach new node
			left = new TNode<Whatever>(element, *this);
			//call function to set height and balance
			SetHeightAndBalance(PointerInParent);
		//else, go left
		} else {
			long result = left->Insert(element, left);
			//call function to set height and balance
			SetHeightAndBalance(PointerInParent);
			return result;
		}
	}
	return 0;
}
/***************************************************************************
% Routine Name : TNode :: Lookup
% File :         Tree.c
% 
% Description :  Allow user to search for element. Distinguish different 
		 situation and call recursively to find element.
% Param :	 element	- the element user want to insert
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Lookup(Whatever & element) const {
	/*debug msg*/
	if (Tree<Whatever>::debug == 1) {
		cerr <<TREE<<TNode<Whatever>::tree_count<<COMPARE<<
			(const char*)element<<
			AND<<(const char*)data<<"]\n";
	}
	//if equals, update
	if (data == element) {
		//update element
		element = data;
		return 1;
	
	//if element greater than data, go right
	} else if (element > data) {
		//if cannot go right, failed
		if (!right) {
			return 0;
		//else, go right
		} else {
			//save and return result from right insert
			long result = right->Lookup(element);
			return result;
		}
	//if element less than data, go left
	} else {
		//if cannot go left, element is new left
		if (!left) {
			return 0;
		//else, go left
		} else {
			//save and return result from right insert
			long result = left->Lookup(element);
			return result;
		}
	}
}
template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}
/*=============================================================================
Class Tree
Description:		Allocate a Tree. Allow user to insert, write, delete 
			and lookup. Get isGreaterThan and equals functions from 
			Driver.c
Data Fields:		TNode :: occupancy	- number of TNodes so far
			Tree :: debug		- if user need debug msg
Functions:	Tree :: Set_Debug_On	- turn debug on
		Tree :: Set_Debug_Off	- turn debug off
		Tree :: isEmpty	- check if tree is empty
		Tree :: Tree	- allocate and initialize
		Tree :: ~Tree	- deallocate the tree
		Tree :: Insert	- allow user to insert element
		Tree :: Remove	- remove the element user want
		Tree :: Lookup	- find element in tree
		Tree :: Write	- return readable data to user
=============================================================================*/
/***************************************************************************
% Routine Name : Tree :: isEmpty
% File :         Tree.c
% 
% Description :  Check if the tree is empty
% Return :	 return 1 if true, 0 otherwise
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: IsEmpty () const {
	
	//if no root, empty
	if (root == NULL) {
		return 1;
	}
	return 0;
}
/***************************************************************************
% Routine Name : Tree :: insert
% File :         Tree.c
% 
% Description :  Inserts an element into the binary tree. Inserts at the root 
		 TNode if Tree is empty, otherwise delegates to TNode's Insert. 
		 Returns true or false indicating success of insertion.
% Param :	 element	- the element user want to insert
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) {
	//if no root, create new root
	if (Tree<Whatever> :: IsEmpty() == 1) {
		root = new TNode<Whatever>(element, *this);
		/*debug msg*/
		if (Tree<Whatever>::debug == true) {
			cerr <<TREE<<tree_count<<INSERT<<(const char*)element<<
				"]\n";
		}
		return 1;
	//else, insert goes down from root
	} else {
		root->Insert(element, root);	
		return 1;
	}
	return 0;
}
/***************************************************************************
% Routine Name : Tree :: Lookup
% File :         Tree.c
% 
% Description :  Searches for an element in the Tree. Delegates to TNode's 
		 Lookup when implemented recursively. Returns true or false 
		 indicating success of look up.
% Param :	 element	- the element user want to search
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {
	//if have root, recusion to find element
	if (root) {
		long output = root->Lookup(element);
		return output;
	}
	//else return 0
	return 0;
}
/***************************************************************************
% Routine Name : Tree :: Remove
% File :         Tree.c
% 
% Description :  Removes an element from the Tree. Delegates to TNode's Remove 
		 when Tree is not empty. Returns true or false indicating 
		 success of removal. 
% Param :	 element	- the element user want to search
% Return :	 return 1 if success, 0 otherwise
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {
	//if no root, return false
	if (root == NULL) {
		return 0;
	//if so, call tnode remove
	} else {
		//create a working TNode
		TNode<Whatever> working(element, *this);
		//call tnode's remove
		long output = root->Remove(working, root, FALSE);
		//if tnode remove success, gives back data
		if (output == 1) {
			element = working.data;
		}
		return output;
	}
}
                                
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
% 
% Description :  Guarantee initialization of occupancy and root. It also 
%                initializes the tree_count using a static counter.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (void): occupancy (0), root (NULL) 
{            
	//set tree counter	
	tree_count = ++tree_counter;
	/*debug msg*/
	if (Tree<Whatever>::debug == true) {
		cerr << TREE << tree_count << ALLOCATE;
	}
}
template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/
{
	//debug msg
	if (Tree<Whatever>::debug == true) {
		cerr << TREE << tree_count << DEALLOCATE;
	}
	//free memory
	if (root != NULL) {
		root->delete_AllTNodes();
	}
}
template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/
{
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";
        return (root) ? root->Write_AllTNodes (stream) : stream;
}