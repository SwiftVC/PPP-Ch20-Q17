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
//template <typename T, typename A = cs_allocator<T>>

struct testobj {
	double val;
	testobj(double d) : val{ d } {};
	testobj() : val{ 0 } {}
	~testobj() {
		//std::cout << "Test object destructor called\n";
	};

};

class cs_vector {
private:
	int sz;
	testobj** elem;
	int space;
	cs_allocator<testobj*> alloc;

public:
	cs_vector() : sz{ 0 }, elem{ nullptr }, space{ 0 } {}
	explicit cs_vector(int s) : sz{ s }, elem{ new testobj *[s] }, space{ s }
	{		// explicit prevents implicit conversion of int to vect in function arg
		for (int i = 0; i < sz; i++)
			elem[i] = new testobj(0);
	}
	cs_vector(const cs_vector& vect) : sz{ vect.size() }, elem{ new testobj *[sz] }, space{ sz }
	{
		for (int i = 0; i < sz; i++)
			elem[i] = new testobj(*vect.elem[i]);
	}

	~cs_vector() {
		for (int i = 0; i < sz; i++)
			delete elem[i];

		alloc.deallocate(elem, space);
	}

	int capacity() const { return space; }
	int size() const { return sz; }
	void push_back(testobj d);
	cs_vector& operator=(cs_vector& vect);		// return type is convention so can chain assignment
	testobj* operator[](int i) { return elem[i]; }
	
	void reserve(int newalloc);
	//void resize(int newsize, T val = T());
	void resize(int newsize, testobj* val = nullptr);
};

//template <typename T, typename A> void cs_vector<typename T, typename A>::reserve(int newalloc)
void cs_vector::reserve(int newalloc){
	if (newalloc <= space) return;
	testobj** p = alloc.allocate(newalloc);
	for (int i = 0; i < sz; i++) alloc.construct(&p[i], elem[i]);
	for (int i = 0; i < sz; i++) alloc.destroy(&elem[i]);
	alloc.deallocate(elem, space);
	elem = p;
	space = newalloc;
}

//template <typename T, typename A> void cs_vector<typename T, typename A>::resize(int newsize, T val) {
void cs_vector::resize(int newsize, testobj* val) {
	reserve(newsize);
	for (int i = sz; i < space; i++) alloc.construct(&elem[i], val); // initialize new elements if sz<space
	for (int i = newsize; i < sz; i++) alloc.destroy(&elem[i]);		 // resets old
	sz = newsize;
}

//template <typename T, typename A> void cs_vector<typename T, typename A>::push_back(T d) {
void cs_vector::push_back(testobj d) {
	testobj* ptr = new testobj(d);
	if (space == 0)
		reserve(8);
	else if (sz == space)
		reserve(2 * space);
	alloc.construct(&elem[sz], ptr);
	++sz;
}

//template <typename T, typename A> cs_vector<typename T, typename A>& cs_vector<T,A>::operator=(cs_vector& vect) {
cs_vector& cs_vector::operator=(cs_vector & vect) {
	// self-assignment protection
	if (this == &vect) return *this;

	// enough space - no changes to allocation
	if (space >= vect.capacity()) {
		for (int i = 0; i < vect.size(); i++)
			elem[i] = vect[i];
		sz = vect.size();
		return *this;
	}

	// else, increase allocation
	delete[] elem;
	sz = 0;
	space = 0;
	resize(vect.size());
	for (int i = 0; i < vect.size(); i++)
		elem[i] = vect[i];

	return *this;
}

#endif