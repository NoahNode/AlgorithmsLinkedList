/***********************************************************************
 * Name        : LinkedList.h
 * Author      : a.mccaughey@ulster.ac.uk
 * Version     : 1.3.2
 * Description : Advanced Head/Tail Dynamic Linked LinkedList class with
 *               exception handling and public Node and Iterator classes
 ***********************************************************************/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstdlib> // Defines NULL
#include <iostream>
#include <exception>

// =============================== LIST NODE ==============================================
// Node Class Used as Building Blocks of a LinkedList
template <class T>
struct Node {
    Node(const T& d = T(), Node<T>* n = nullptr) : data(d), next(n) { }

    T data;
    Node<T>* next;
};

// ============================= LIST ITERATOR ============================================

// List Iterator Class , used to efficiently traverse through a List
template <class T>
class ListIterator {
	public:
		ListIterator(Node<T> *start=NULL) : current(start)	{}
		ListIterator & operator=(const ListIterator & o)	{ current = o.current; return (*this); }
		T & operator*()										{ return current->data; }
		ListIterator & operator++()							{ current = current->next; return *(this);}
		ListIterator operator++(int)						{ ListIterator tmp(*this); ++(*this); return tmp; }

		bool operator!=(const ListIterator & o)				{ return current != o.current; }
		bool operator==(const ListIterator & o)				{ return current == o.current; }
	private:
		Node<T> *current;
};

// ============================== LINKED LinkedList ===========================================
template <class T>
class LinkedList {
public:
    LinkedList();
    virtual ~LinkedList();
    LinkedList(const LinkedList<T> & other);
	void operator=(const LinkedList<T> & other);
	bool operator==(const LinkedList<T> & other) const;
	
    void clear();
	void add(const T & value);
    void add(int pos, const T & value);
    void remove(int pos);
    void set(int pos, const T & value);
    T    get(int pos) const;
    int  size() const;
    bool isEmpty() const;

    void print(std::ostream & os) const;
    int  find(const T & value) const;

	// Iterators
	ListIterator<T> begin()			{ return ListIterator<T>(header->next); }
    ListIterator<T> end()			{ return ListIterator<T>(nullptr); }
    ListIterator<T> begin() const	{ return ListIterator<T>(header->next); }
    ListIterator<T> end()   const	{ return ListIterator<T>(nullptr); }

private:
	Node<T>* nodeAt(int pos) const;
	void deepCopy(const LinkedList<T> & c);

    Node<T> *header, *tail;
    int count;
};

// ============================== LinkedList Implementation ======================

// Default Constructor
template <class T>
LinkedList<T>::LinkedList() {
	header = new Node<T>();
	tail = header;			// TAIL POINTS TO HEADER
	count = 0;
}

// Destructor
template <class T>
LinkedList<T>::~LinkedList() {
    clear();
	delete header; // delete dummy header node
}

// PostCondition: construct LinkedList as a duplicate of c
template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> & other) {
	header = new Node<T>;	// create dummy header
    deepCopy(other);			// create a deep copy of other
}

// PreCondition: c refers to a LinkedLinkedList
// PostCondition: perform a deep copy of c
template <class T>
void LinkedList<T>::deepCopy(const LinkedList<T> & c) {
	Node<T>* cc = c.header->next;
    Node<T>* prev = header;
    Node<T>* n;			// new Node reference
    while (cc != NULL) {
        n = new Node<T>(cc->data, prev->next);
        prev->next = n;	// set last to refer to n
        prev = n;		// set last to n
        cc = cc->next;	// move to next Node in c
    }
	tail = prev;		// SET TAIL TO LAST NODE
	count = c.count;	// copy number of elements
}


// PostCondition: assign c to LinkedList
template<class T>
void LinkedList<T>::operator=(const LinkedList<T> & other) {
	clear();		// clear existing LinkedList
	deepCopy(other);	// perform a deep copy
}

// PostCondition: test LinkedLists for equality
template<class T>
bool LinkedList<T>::operator==(const LinkedList<T> & other) const {
	bool same=true;
	if (size() != other.size()) {
		same = false;
	}

	// Non iterator traversal - less efficient
	/*for(int i=0; i<size() && same; i++) {
		if (get(i) != o.get(i)) {
			same = false;
		}
	}*/
	
	for(ListIterator<T> itr = begin(), oitr=other.begin(); itr != end() && same; itr++, oitr++)  {
	    if ( (*itr) != (*oitr)) {
		   same = false;
		}
	}
	return same;
}

// PostCondition: return number of elements in LinkedList
template<class T>
int LinkedList<T>::size() const {
    return count;
}


// PreCondition: pos is a valid insertion LinkedList position
// PostCondition: inserts element value at specified position in linked LinkedList
template<class T>
void LinkedList<T>::add(int pos, const T & value) {
	if (pos < 0 || pos > size()) {
		throw std::out_of_range("LinkedList invalid position: " + std::to_string(pos));
	}
 	Node<T>* prev = nodeAt(pos - 1);
	Node<T>* n = new Node<T> (value, prev->next);
	prev->next = n;
	if (pos == count) {tail = n;}           // INSERTED AT END SO UPDATE TAIL
	count++;	
}

// PostCondition: inserts element value at end of linked LinkedList
template<class T>
void LinkedList<T>::add(const T & value) {
	add(size(),value);
}


// PreCondition: pos is a valid LinkedList position
// PostCondition: deletes Node at specified position in linked LinkedList
template<class T>
void LinkedList<T>::remove(int pos) {
	if (pos < 0 || pos >= size()) {
		throw std::out_of_range("LinkedList invalid position: " + std::to_string(pos));
	}
 	Node<T>* prev = nodeAt(pos - 1);		// obtain ref to previous Node
	Node<T>* curr = prev->next;				// obtain ref to Node being deleted
	prev->next = curr->next;				// set prev to refer to next node
	if (pos == count - 1) { tail = prev; }	// IF LAST NODE DELETED UPDATE TAIL
	count--;								// reduce number of elements in LinkedList
	delete curr;							// delete Node referred to by curr
}

// PreCondition: pos is a valid LinkedList position
// PostCondition: deletes Node at specified position in linked LinkedList
template<class T>
T LinkedList<T>::get(int pos) const  {
	if (pos < 0 || pos >= size()) {
		throw std::out_of_range("LinkedList invalid position: " + std::to_string(pos));
	}
  	Node<T>* curr = nodeAt(pos);
	return curr->data;
}


// PreCondition: pos is a valid LinkedList position
// PostCondition: updates value part of Node at specified position in linked LinkedList
template<class T>
void LinkedList<T>::set(int pos, const T & value) {
	if (pos < 0 || pos >= size()) {
		throw std::out_of_range("LinkedList invalid position: " + std::to_string(pos));
	}
	Node<T>* curr = nodeAt(pos);
	curr->data = value;	
}

// PostCondition: returns index of LinkedList item if found or -1 if not found
template<class T>
int LinkedList<T>::find(const T & value) const {
    /* Using LinkedList node traversal - efficent
	Node<T>* tmp = header->next;
    for (int p = 0; (tmp != NULL); p++) {
        if (tmp->data == value) {
            return p;
        }
        tmp = tmp->next;
    }
    return -1;
	*/

	/* Using LinkedList functions - but less efficient
	for(int i = 0; i < size(); i++) {
		if (get(i) == value) {
			return i;
		}
	}
	return -1;
	*/
	// Using an iterator
	int i=0;
	for (ListIterator<T> itr = begin(); itr != end(); itr++, i++) {
		if ((*itr) == value) {
			return i;
		}
	}
	return -1;
}

// PostCondition: prints contents of LinkedList to standard output
template<class T>
void LinkedList<T>::print(std::ostream & os) const {

    ListIterator<T> itr = begin();
	os <<  "[ ";
    while (itr != end()) {
        os << (*itr) << " ";
        ++itr;
    }
    os << "]";
}

// PreCondition:  pos is valid 
// PostCondition: returns reference to Node at specified position
template<class T>
Node<T>* LinkedList<T>::nodeAt(int pos) const {   
	if (pos == count-1) {	// LAST NODE SO RETURN tail
		return tail;
	} else {				// Locate node
		Node<T>* p = header;
	    for (int i = 0; i <= pos; i++) {
		    p = p->next;
		}
		return p;
	}
}

// PostCondition: LinkedList is emptied count == 0;
template<class T>
void LinkedList<T>::clear() {
	for (int i = size() - 1; i >= 0; i--) {
		remove(i);
	}
}

//PostCondition: returns countgth of LinkedList
template<class T>
bool LinkedList<T>::isEmpty() const {
    return (count == 0);
}


// PreCondition: None
// PostCondition: overload << operator to output LinkedList on ostream
template <class T>
std::ostream& operator <<(std::ostream& output, const LinkedList<T>& l) {
    l.print(output);
    return output;  // for multiple << operators.
}

#endif /*LINKEDLIST_H*/
