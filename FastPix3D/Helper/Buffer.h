#pragma once 
#include "FastPix3D.h" 

template<class T>
class Buffer
{
private:
	int32 _Size;
	T *_Data;

public:
	readonly_property(int32, Size)
	{
		return _Size;
	}
	readonly_property(T*, Data)
	{
		return _Data;
	}

	explicit Buffer(int32 size) :
		_Size(size),
		_Data((T*)_aligned_malloc(size * sizeof(T), 32))
	{
	}
	Buffer(const Buffer& other) :
		_Size(other._Size),
		_Data((T*)_aligned_malloc(other._Size * sizeof(T), 32))
	{
		memcpy(_Data, other._Data, _Size * sizeof(T));
	}
	~Buffer()
	{
		_aligned_free(_Data);
	}

	Buffer& operator =(const Buffer<T> &other)
	{
		if (this != &other)
		{
			if (_Size != other._Size)
			{
				_aligned_free(_Data);

				_Size = other._Size;
				_Data = (T*)_aligned_malloc(_Size * sizeof(T), 32);
			}

			memcpy(_Data, other._Data, _Size * sizeof(T));
		}
		return *this;
	}
};