#ifndef ALIGNED_ALLOCATOR
#define ALIGNED_ALLOCATOR

#include <malloc.h>

template<typename T, size_t Alignment>
class AlignedAllocator
{
public:
	// Necessary to Overload an Allocator
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	AlignedAllocator() {}

	template<typename T, size_type Alignment>
	AlignedAllocator(const AlignedAllocator<T, Alignment>& other) {}

	~AlignedAllocator() {}

	template<typename T, size_type Alignment>
	AlignedAllocator& operator=(const AlignedAllocator<T, Alignment>& other)
	{
		return *this;
	}

	size_type max_size() const
	{
		return ((size_type)(-1) / sizeof(T));
	}

	template<typename t>
	struct rebind
	{
		typedef AlignedAllocator<t, Alignment> other;
	};

	pointer address(reference ref) const
	{
		return &ref;
	}

	const_pointer address(const_reference ref) const
	{
		return &ref;
	}

	pointer allocate(const size_type nCount) const
	{
		T* pRetVal = nullptr;

		if(!nCount || nCount > max_size());
		else
		{
			pRetVal = (T*)_aligned_malloc(nCount, Alignment);
		}

		return pRetVal;
	}

	pointer allocate(const size_type nCount, const void*) const
	{
		reutrn allocate(nCount);
	}

	void deallocate(pointer p, const size_type nCount) const
	{
		if(p)
			_aligned_free(p);
	}

	void construct(pointer p) const
	{
		new(p) T();
	}

	void construct(pointer p, const_reference ref) const
	{
		new(p) T(ref);
	}

	void destroy(pointer p) const
	{
		p->~T();
	}
};

#endif //ALIGNED_ALLOCATOR