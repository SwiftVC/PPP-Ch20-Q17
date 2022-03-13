/*
*		Author:	CS
*		13/03/2022
* 
*		Copied from Ch20 Q05 (std::cout related operator overloads removed)
*		The vector is converted to act as a vector of pointers:
*		- element's destructors called on close;
*		- the push_back operation creates a new pointer
*		- the array-of-pointers, elem, is handled by an allocator.
* 
*		There is no memory leak.
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
		*dbl_vect[dbl_vect.size() - 1] = testobj(5);	// dbl_vect.elem[1] is changed to testobj.val = 5
		// ^ memory assignment is fine too, the notation will be changed in the following question
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