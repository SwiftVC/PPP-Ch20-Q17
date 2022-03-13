#ifndef CS_allocator_
#define CS_allocator_
//	allocator usage will be used by a vector to reserve memory
template<typename T>
class cs_allocator {
public:
	cs_allocator() : _begin{ nullptr } {};
	T* allocate(int n);					// allocate space for n*T objects WITHOUT initialising
	void deallocate(T* p, int n);		// deallocate n*T objects POTENTIALLY unitialized, starting at address p

	void construct(T* p, const T& v);	// construct a T object in allocated BUT uninitialized space
	void destroy(T* p);					// destroy the T in p (Destroy means return to the uninitialized state BUT still allocated!)

	T* _begin;

};

template<typename T>
T* cs_allocator<T>::allocate(int n) {
	_begin = (T*)malloc(sizeof(T) * n);

	return _begin;
}

template<typename T>
void cs_allocator<typename T>::deallocate(T* p, int n) {
	free(p);	// free() is used on a pointer returned by malloc
}

template<typename T>
void cs_allocator<typename T>::construct(T* p, const T& v) {
	new(p)T(v);		// initializes a copy of v to p
}

template<typename T>
void cs_allocator<typename T>::destroy(T* p)
{
	p->~T();
}

#endif