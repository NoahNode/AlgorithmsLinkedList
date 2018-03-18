//================================================================================
// Name        : practical7.cpp
// Author      : your name
// Version     : 1.0
// Description : Develop understanding of List Node manipulation, LinkedList operation
//               and use in a larger data structure (a BlockChain)
//================================================================================

#include <iostream>
#include <string>
#include <cctype>
#include <exception>

#include "LinkedList.h"
#include "BlockChain.h"

using namespace std;

// ------------ Q1 Utility Functions -----------------------------------------
// Prints all nodes pointed to by dummy header node
void printNodes(const Node<string> *header)
{
	cout << "[ ";
	// dont print the header, rather the first node pointed to by header
	Node<string> *i = header->next;
	while (i != nullptr) {
		cout << i->data << " ";
		i = i->next;
	}
	cout << "]" << endl;
}

// Return a pointer to recreated list of nodes
Node<string> * createNodes()
{
	// create header
	Node<string> * header = new Node<string>();

	// now recreate new Nodes
	header->next = new Node<string>("they", header->next);
	header->next = new Node<string>("he", header->next);
	header->next = new Node<string>("we", header->next);

	return header;
}

// delete the nodes we created with 'new' operator
void destroyNodes(Node<string> * i)
{
	// loop through each node
	while (i != nullptr) {
		// set d to current node
		Node<string> *d = i;
		// move i to next node
		i = i->next;
		// delete node pointed to by d
		delete d;
	}
}

// ---------------------  COMPLETE THESE FUNCTIONS ------------------------------------
//  Q1a - this one is done for you as an example
void Q1a()
{
	Node<string> *header = createNodes();

	// create a new node containing element "XX" and set its next pointer equal to header->next 
	// i.e. the node containing "we"
	Node<string> *na = new Node<string>("XX", header->next);
	// set the header node to now refer to the new node "XX" rather than the original "we" node. 
	header->next = na;

	// print nodes to ensure it worked
	std::cout << "Q1a. "; printNodes(header);
	destroyNodes(header);
}

//  Q1b
void Q1b()
{
	Node<string> *header = createNodes();

	// complete function

	// print nodes to ensure it worked
	std::cout << "Q1b. "; printNodes(header);
	destroyNodes(header);
}

//  Q1c
void Q1c()
{
	Node<string> *header = createNodes();

	//complete function

	// print nodes to ensure it worked
	std::cout << "Q1c. "; printNodes(header);
	destroyNodes(header);
}


//  Q1d
void Q1d()
{
	Node<string> *header = createNodes();

	// complete function

	// print nodes to ensure it worked
	std::cout << "Q1d. "; printNodes(header);
	destroyNodes(header);
}



// --------------------------------QUESTIONs 2 3 & 4 ---------------------------------------- -

// PostCondition: Determine the intersection of List a and List b and 
//                return result in in List c.
//                i.e. elements contained in both Lists a and b
template <class T>
void listIntersection(const LinkedList<T> & a, const LinkedList<T> & b, LinkedList<T> & c)
{
	// complete
}


// PostCondition: Determine the union of List a and List b and 
//                return result in in List c.
//                i.e. all elements in both Lists (not repeated)
template <class T>
void listUnion(const LinkedList<T> & a, const LinkedList<T> & b, LinkedList<T> & c)
{
	// complete
}


// PostCondition: Populate list c with difference of List a and List b 
//                i.e. elements of List a, which do not belong to List b.
template <class T>
void listDifference(const LinkedList<T> & a, const LinkedList<T> & b, LinkedList<T> & c) {
	// complete
}


// ---------------------- Demo of the List Set Operations ---------------------------
void testListSetOperations()
{
	LinkedList<int> a, b, c;
	a.add(1); a.add(4); a.add(2); a.add(7); a.add(5);
	b.add(3); b.add(5); b.add(7); b.add(11);

	cout << " -- Q2,3,4 List Set Operations --\n";
	cout << "List a: " << a << "\n";
	cout << "List b: " << b << "\n";

	// Test Intersection
	listIntersection(a, b, c);
	cout << "Intersection:  " << c << "\n";
	c.clear();

	//Test Union
	listUnion(a, b, c);
	cout << "Union:  " << c << "\n";
	c.clear();

	// Test Difference
	listDifference(a, b, c);
	cout << "Difference : " << c << "\n";
	c.clear();

	cout << "\n";
}

// ------------------------------- Demo Of the BlockChain Class -----------------------------

void blockChainDemo() {

	// create chain with initial bank balance and proof of work difficulty
	BlockChain chain;
	chain.setDifficulty(1); // dont go above 3 otherwise mining blocks will take some time

	// add pending transactions to the chain
	chain.addTransaction(Transaction("bank", "aiden", 100.0));
	chain.addTransaction(Transaction("aiden", "frank", 50.0));

	// start a miner to process pending transactions
	chain.minerGenerateBlock("miner1");

	// add further pending transactions
	chain.addTransaction(Transaction("frank", "tom", 50.0));
	chain.addTransaction(Transaction("aiden", "emma", 10.0));

	// start another 2 miners
	chain.minerGenerateBlock("miner1");
	chain.minerGenerateBlock("miner2");

	// print the blockchain and some account balances
	std::cout << "\n" << chain << "\n";

	std::cout << "------- Balances -------\n";
	std::cout << "Balance of Miner1: " << chain.getBalanceOfAddress("miner1") << std::endl;
	std::cout << "Balance of Miner2: " << chain.getBalanceOfAddress("miner2") << std::endl;
	std::cout << "Balance of aiden: " << chain.getBalanceOfAddress("aiden") << std::endl;

}


// --------------- Main method --------------------
int main() {

	//Q1
	Q1a();
	Q1b();
	Q1c();
	Q1d();

	//Q2,3,4
	//testListSetOperations();

	// Optional Q5
	//blockChainDemo();

	// ---------------------------------------------------
	std::cout << std::endl << "Press enter to quit";
	std::cin.sync(); // flush input buffer
	std::cin.ignore(); // ignore what user enters

	return 0;
}