/*
*		Author:	CS
*		13/03/2022
* 
*		Copied from Ch20 Q15
*		The vector acts as a vector of pointers:
*		- element's destructors called on close
*		- the push_back operation creates a new pointer
*		- the array-of-pointers, elem, is handled by an allocator.
*		The changes from Ch20 Q15 are in the [] and * operators
*		(and + operator, to support the * operator for pointer arithmetic)
* 
*		Note:
*		An iterator could've been added but pointer arithetic was opted for.
*/

#include <iostream>
#include <exception>
#include <vector>
#include <memory>
#include "cs_allocator.h"
#include "cs_vector.h"
#include <concepts>
#include <string>

int main() {
	try {
		{
		cs_vector dbl_vect;
		dbl_vect.push_back(testobj(3));
		dbl_vect.push_back(testobj(3));			// successful deallocation
		cs_vector copy_test = dbl_vect;			// successful deep copy
		cs_vector cons_test(5);					// successful deallocation


		// The following assignments are affected by Ch20 Q15-Q16 notational update.
		dbl_vect[dbl_vect.size() - 1] = testobj(5);	// dbl_vect.elem[1] is changed to testobj.val = 5
		// ^ memory assignment is fine too, the notation will be changed in the following question

		*dbl_vect = testobj(90);		// dbl_vect.elem[0] is changed to testobj.val = 90
		*(dbl_vect + 1) = testobj(100);	// dbl_vect.elem[1] is changed to testobj.val = 100
		}
	return 0;
	}
	catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "exception\n";
		return 2;
	}
}