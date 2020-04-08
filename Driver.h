/******************************************************************************
							Han Zhang
							CSE12, Winter 2020
							February 27, 2020
							cs12wi20kj
			Assignment Eight
File Name:	Driver.h
Description:	Contains the member functions for the UCSDStudent class as part
		of the Driver.h file used to test Tree. Work together with
		Tree.c.
******************************************************************************/
#ifndef DRIVER_H
#define DRIVER_H
#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;
//=============================================================================
//Class UCSDStduent
//
//Description:		initialize incoming values, provide isgreaterthan,  
//			equals, and write functions for Tree
//
//Data Fields:		name		- holds the name inserted from user
//			stduentnum	- hold the student num inserted from 
//					  user
//
//Public Fucntions:	write		- print out strings
//			constructor	- takes incoming value and initialize
//			deconstructor	- free the memory
//			operator ==	- compare if name are equal
//			operator >	- compare if name is greater
//			const char *	- return name of the element
//=============================================================================
class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[20];
        long studentnum;
public:
	/*constructor1*/
	UCSDStudent (char * nm, long stunm) : studentnum (stunm) {
		strcpy (name, nm);
	}	
	/*overload to return name*/
	operator const char * (void) const {
		return name;
	}
	/*overload to compare name if equal*/
	long operator == (const UCSDStudent & stu) const {
		return ! strcmp (name, stu.name);
	}
	/*overload to compare name if greater*/
	long operator > (const UCSDStudent & stu) const {
		return (strcmp (name, stu.name)>0) ? 1:0;
	}
	/*write out name and studentnum for user*/
	ostream & Write (ostream & stream) const {
		return stream << "name:  " << name
			<< "  studentnum:  " << studentnum;
	}	
};
#endif