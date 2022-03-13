/*
The flow:
A change to the vectors allocated memory always goes though reserve.
resize and push_back both call reserve.
resize sets all elements to the default values.
*/

#ifndef cs_vector_
#define cs_vector_
#include "cs_allocator.h"
#include <iostream>
#include <exception>
#include <vector>
//template <typename T, typename A = cs_allocator<T>>

struct testobj {
	double val;
	testobj(double d) : val{ d } {};
	testobj() : val{ 0 } {}
	~testobj() {
		//std::cout << "Test object destructor called\n";
	};

};

class ownership_vector {
private:
	int sz;
	std::vector<bool> ownership;
	testobj** elem;
	int space;
	cs_allocator<testobj*> alloc;

public:
	ownership_vector() : sz{ 0 }, ownership(sz, true), elem{ nullptr }, space{ 0 } {}
	explicit ownership_vector(int s) : sz{ s }, ownership(sz, true), elem{ new testobj *[s] }, space{ s }
	{		// explicit prevents implicit conversion of int to vect in function arg
		for (int i = 0; i < sz; i++)
			elem[i] = new testobj(0);
	}
	ownership_vector(const ownership_vector& vect) : sz{ vect.size() }, elem{ new testobj *[sz] }, space{ sz }
	{
		for (int i = 0; i < sz; i++)
			elem[i] = new testobj(*vect.elem[i]);
	}

	~ownership_vector() {
		for (int i = 0; i < sz; i++)
		{
			if(ownership[i])
				delete elem[i];
		}

		alloc.deallocate(elem, space);
	}

	int capacity() const { return space; }
	int size() const { return sz; }
	void push_back(testobj d);
	ownership_vector& operator=(ownership_vector& vect);		// return type is convention so can chain assignment
	testobj& operator[](int i) { return *elem[i]; }
	testobj& operator*() { return *elem[0]; }
	testobj* operator+(int i) { return elem[i]; }
	void reserve(int newalloc);
	//void resize(int newsize, T val = T());
	void resize(int newsize, testobj* val = nullptr);
	testobj* relieve(int i) { ownership[i] = false; return elem[i]; }	// gain ownership of element's pointed-to object

};

//template <typename T, typename A> void ownership_vector<typename T, typename A>::reserve(int newalloc)
void ownership_vector::reserve(int newalloc){
	if (newalloc <= space) return;
	testobj** p = alloc.allocate(newalloc);
	for (int i = 0; i < sz; i++) alloc.construct(&p[i], elem[i]);
	for (int i = 0; i < sz; i++) alloc.destroy(&elem[i]);
	alloc.deallocate(elem, space);
	elem = p;
	space = newalloc;
}

//template <typename T, typename A> void ownership_vector<typename T, typename A>::resize(int newsize, T val) {
void ownership_vector::resize(int newsize, testobj* val) {
	reserve(newsize);
	for (int i = sz; i < space; i++) alloc.construct(&elem[i], val); // initialize new elements if sz<space
	for (int i = newsize; i < sz; i++) alloc.destroy(&elem[i]);		 // resets old
	sz = newsize;
	ownership = std::vector<bool>(sz, true);
}

//template <typename T, typename A> void ownership_vector<typename T, typename A>::push_back(T d) {
void ownership_vector::push_back(testobj d) {
	testobj* ptr = new testobj(d);
	if (space == 0)
		reserve(8);
	else if (sz == space)
		reserve(2 * space);
	alloc.construct(&elem[sz], ptr);
	++sz;
	ownership.push_back(true);
}

//template <typename T, typename A> ownership_vector<typename T, typename A>& ownership_vector<T,A>::operator=(ownership_vector& vect) {
ownership_vector& ownership_vector::operator=(ownership_vector & vect) {
	// self-assignment protection
	if (this == &vect) return *this;

	// enough space - no changes to allocation
	if (space >= vect.capacity()) {
		for (int i = 0; i < vect.size(); i++)
			*elem[i] = vect[i];
		sz = vect.size();
		ownership = std::vector<bool>(sz, true);
		return *this;
	}

	// else, increase allocation
	delete[] elem;
	sz = 0;
	ownership = std::vector<bool>(sz, true);
	space = 0;
	resize(vect.size());
	for (int i = 0; i < vect.size(); i++)
		*elem[i] = vect[i];

	return *this;
}

#endif