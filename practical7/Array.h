/**
 * Array.h
 *
 * Generic Dynamic Array
 *
 * @author  Aiden McCaughey
 * @email   a.mccaughey@ulster.ac.uk
 * @version 1.3
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <exception>
#include <cassert>
#include <iostream>


template <class T>
class Array
{
public:
	explicit Array(int size=0);
	~Array();
	
	Array(const Array<T> & ap);
	Array(int size, const int *data);
	
	Array<T>& operator=(const Array<T> & right);
	
	T& operator[](int index);
	const T& operator[](int index) const;
	int length() const;
	
	void resize(int newSize);
	void initialise(T def=T());
	void print(std::ostream & os=std::cout) const;
	std::string toString() const;

private:
	T *elements;
	int capacity;

	inline void deepCopy(const Array<T> & original);
};


// ========================= IMPLEMENTATION Array.cpp ===================================

// PreCondition: size > 0
// PostCondition: creates a dynamic array of specified size
template <class T>
Array<T>::Array(int size) 
{
	if (size < 0) {
		size = 0;
	}
	capacity = size;
	elements = new T[capacity];
}

// PreCondition: None
// PostCondition: returns dynamically allocated memory used for elements to the heap and elements is set to null
template <class T>
Array<T>::~Array()
{
	delete [] elements;
	elements = nullptr;
	capacity = 0;
}

// PreCondition: ap is a valid Array
// PostCondition: Initalises new Array with deep copy of Array referenced by parameter ap 
template <class T>
Array<T>::Array(const Array<T> & ap)
{
	deepCopy(ap);
}

// PreCondition: a is a C array
// PostCondition: Initalises new Array with values contained in array data
template <class T>
Array<T>::Array(int size, const int *data)
{
	// create an Array of specified size
	if (size < 0) {
		size = 0;
	}
	capacity = size;
	elements = new T[capacity];

	// populate the Array with supplied data
	for (int i = 0; i < size; i++)
	{
		elements[i] = data[i];
	}
}


// PreCondition: None
// PostCondition: array referenced by right is copied into this
template <class T>
Array<T> & Array<T>::operator=(const Array<T> & right)
{
	if (this != &right) {
		delete [] elements;
		deepCopy(right);
	}
	return *this;
}

// PreCondition: None
// PostCondition: each position in array is initialised with def value
template <class T>
void Array<T>::initialise(T def) {
	for(int i=0; i<capacity; i++)
		elements[i] = def;
}


// PreCondition: memory allocated to elements has been released
// PostCondition: copy of original used to initialise this arrasy
template <class T>
void Array<T>::deepCopy(const Array<T> & original)
{
	capacity = original.capacity;

	elements = new T[capacity];
	for (int i=0; i<capacity; i++) {
		elements[i] = original.elements[i];
	}
}

// PreCondition: index is valid
// PostCondition: reference to element at index returned
template <class T>
T& Array<T>::operator[](int index)
{
	if (index < 0 || index >= capacity) {
		throw std::out_of_range("Array: index out of range " + std::to_string(index));
	}
	
	return elements[index];
}

// PreCondition: index is valid
// PostCondition: reference to element at index returned
template <class T>
const T& Array<T>::operator[](int index) const
{
	if (index < 0 || index >= capacity) {
		throw std::out_of_range("Array: index out of range " + std::to_string(index));
	}
	
	return elements[index];
}

// PreCondition: newSize is greater than 0
// PostCondition: array is resized
template <class T>
void Array<T>::resize(int newSize) 
{
	if (newSize > 0) {
		auto *newArray = new T[newSize];
		int limit = (newSize > capacity) ? capacity : newSize;

		// copy existing elements to new array
		for(int i=0; i<limit; i++) {
			newArray[i] = elements[i];
		}

		delete [] elements;
		elements = newArray;
		capacity = newSize;
	}	
}

// PreCondition: None
// PostCondition: Returns length of Array
template <class T>
inline int Array<T>::length() const
{
	return capacity;
} 

// PreCondition: None
// PostCondition: prints a copy of array to ostream
template <class T>
void Array<T>::print(std::ostream & os) const {
	os << "[ ";
	for(int i=0; i<length(); i++) {
		os << elements[i] << " ";
	}
	os << "]";
}

// PreCondition: None
// PostCondition: returns a string copy of array elements
template <class T>
std::string Array<T>::toString() const {
	std::string s = "[ ";
	for(int i=0; i<length(); i++) {
		s += std::to_string(elements[i]) + " ";
	}
	s += "]";
	return s;
}

// PreCondition: None
// PostCondition: overload << operator to output array on ostream
template <class T>
std::ostream& operator <<(std::ostream& output, const Array<T>& p) {
    p.print(output);
    return output;  // for multiple << operators.
}

#endif
