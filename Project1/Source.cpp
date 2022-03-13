/*
*		Author:	CS
*		13/03/2022
* 
*		Copied from Ch20 Q16
*		The vector acts as a vector of pointers:
*		- element's destructors called on close
*		- the push_back operation creates a new pointer
*		- the array-of-pointers, elem, is handled by an allocator.
*		
*		This allows the user to decide which objects are owned by the vector.
*		(The final destructor won't delete the desired object when the ownership_vector goes out of scope)
*		An object of the standard library std::vector<bool> is used to keep track of which not to delete,
*		this resizes with the sz ownership_vector member, the number of constructed objects.
* 
*		An additional function is provided to return the pointer to an element who's ownership has changed:
*		testobj* ownership::relieve(int i) { ownership[i] = false; return elem[i]; }
* 
*		Heap checks out.
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
			testobj* pointer{ nullptr };
			{
				ownership_vector dbl_vect;
				dbl_vect.push_back(testobj(3));
				dbl_vect.push_back(testobj(3));			// successful deallocation

				dbl_vect[dbl_vect.size() - 1] = testobj(5);	// dbl_vect.elem[1] is changed to testobj.val = 5
				pointer = dbl_vect.relieve(dbl_vect.size() - 1);
			}
			delete pointer;
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