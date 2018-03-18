/**
* BlockChain.h
*
* Simple Implementation of a BlockChain
*
* Used to demonstrate use of:
* Dynamic List class (LinkedList) used to implement the Chain
* Static  List class (ArrayList)  used to implement a Block

* @author  Aiden McCaughey
* @email   a.mccaughey@ulster.ac.uk
* @version 1.0
*/

#include "ArrayList.h"	// Static List
#include "LinkedList.h"	// Dynamic List
#include "picosha2.h"	// SHA256 hash algorithm

#include <sstream>		// std::stringstream
#include <iomanip>      // std::setprecision
#include <ctime>		// std::time

//  -------- A Transaction recording transfer of money from sender to recipient ------ //
struct Transaction {
	Transaction(std::string from = "", std::string to = "", float amt = 0) :
		fromAddress{ from }, toAddress{ to }, amount{ amt } {}

	// return a transaction as a string
	std::string toString() const {
		std::stringstream ss;
		ss << "(" << fromAddress << "->" << toAddress << " : " << std::fixed
			<< std::setprecision(2) << amount << ")";
		return ss.str();
	}

	// public member properties
	std::string fromAddress; // record of person sending funds
	std::string toAddress;	 // record of person receiving funds
	float amount;			 // amount transfered
};


// ----------------------------- A Block -----------------------------------------//
// Contains fixed number of transactions (2 in this example) along with a creation 
// timestamp, a hash of the block and a copy of the hash of the previous block
// (ideally would be declared as a private member of the BlockChain class)        
// -------------------------------------------------------------------------------//
struct Block {
	Block() : transactions{}, timestamp{ "" }, previousHash{ "" }, hash{ "" } {
		// set timestamp of block creation as current time		
		timestamp = std::to_string(std::time(0));
		hash = calculateHash();
	};

	Block(const ArrayList<Transaction> & trans, std::string prevHash) :
		transactions{ trans }, timestamp{ "" }, previousHash{ prevHash }, hash{ "" } {

		// set timestamp of block creation as current time		
		timestamp = std::to_string(std::time(0));
		hash = calculateHash();
	};

	std::string hash;						// hash of this block
	std::string previousHash;				// hash of previous block
	std::string timestamp;					// time of block creation
	ArrayList< Transaction > transactions;	// transactions stored in block

	int nonce;		// used to generate new hash as part of proof of work

					// PostCondition: return hex string hash of block
	std::string calculateHash() const {
		// convert transactions to stringstream representation
		// then to extract a string from the stream call ss.str()
		std::stringstream ss;
		ss << transactions;

		// create string from transaction stream, previous hash, timestamp and nonce
		std::string blockStr = ss.str() + previousHash + timestamp + std::to_string(nonce);

		// return hex string hash of the string representation of the block
		return picosha2::hash256_hex_string(blockStr);
	}

	// PostCondition: proof of work carried out to mine a new block
	// setting difficulty level above 4 will cause mining to take a long time
	// in real world it takes at least 10 mins to mine a single block
	void mineBlock(int difficulty) {
		std::cout << "Mining block.. ";

		// while the hash does not begin with difficulty 0's
		while (hash.substr(0, difficulty) != std::string().append(difficulty, '0')) {
			nonce++;				// increment nonce to cause hash change  
			hash = calculateHash(); // generate the new hash
		}
		std::cout << hash << "\n";
	}

	// PostCondition: return string representation of a Block
	std::string toString() const {
		std::stringstream ss;	// create stringsteam to construct a string
		ss << transactions;		// output transactions to stream to construct a string
		return ss.str();		// return string from stream
	}

};


// ------------------------  The BlockChain Class ----------------------------
class BlockChain {
public:
	BlockChain(int difficulty = 1, float reward = 0.05) : chain{}, pendingTransactions{},
		difficulty{ difficulty }, miningReward{ reward } {
		// create initial chain genesis block
		createGenesisBlock();
	}

	// PreCondition: dif >=1 && dif <=MAXDIFFICULTY
	// PostCondition: new difficulty level set for proof of work when mining a block
	void setDifficulty(int dif) {
		if (dif >= 1 && dif <= MAXDIFFICULTY) {
			difficulty = dif;
		}
	}

	// PostCondition: new mining reward set for future mined blocks
	void setReward(float reward) {
		miningReward = reward;
	}

	// PostCondition: return true if chain is valid, otherwise false
	bool isChainValid() const {
		// verify that all block (excluding genesis) are valid
		for (int i = 1; i < chain.size(); i++) {
			Block currentBlock = chain.get(i);
			Block previousBlock = chain.get(i - 1);
			if (currentBlock.hash != currentBlock.calculateHash()) {
				return false;
			}
			if (currentBlock.previousHash != previousBlock.hash) {
				return false;
			}
		}
		return true;
	}

	// PostCondition: add a new pending transaction
	void addTransaction(const Transaction & t) {
		pendingTransactions.add(t);
	}

	// PostCondition: a miner creates a new block (carrying out proof of work) and adds to the chain
	bool minerGenerateBlock(std::string minerAccount) {
		bool blockMined = false; // was a block mined successfully 

								 // ensure enough pending transactions available to create a Block
		if (pendingTransactions.size() >= BLOCKSIZE) {
			// create fixed size transaction list for addition to block
			ArrayList<Transaction> trans(BLOCKSIZE);

			// add pending transactions to Fixed size block transaction List
			for (int i = 1; i <= BLOCKSIZE; i++) {
				trans.add(pendingTransactions.get(0));
				pendingTransactions.remove(0);
			}

			// create a new block with mined transactions and hash of last block
			Block block(trans, getLatestBlock().hash);

			// carry out the proof of work
			block.mineBlock(difficulty);

			// add mined block to the chain
			chain.add(block);

			// send miner their playment from the bank

			pendingTransactions.add(Transaction("bank", minerAccount, miningReward));

			// block successfully mined
			blockMined = true;
		}
		return blockMined;
	}

	// PostCondition: searches chain for transactions containing address and 
	//				  calculates and returns the balance of the address
	float getBalanceOfAddress(std::string address) const {
		float balance = 0;

		// COMPLETE THIS FUNCTION
		// loop through each block 
		// loop though each transaction within a block
		// if the fromAddress of transaction is equal to address then reduce balance by transaction amount
		// if the toAddress of transaction is equal to address then increase balance by transaction amount

		return balance;
	}

	std::string toString() const {
		std::stringstream ss;
		if (isChainValid()) {
			ss << "--------------------Block Chain-----------------\n";
		}
		else {
			ss << "------------***INVALID*** Block Chain-----------\n";
		}

		// exclude the genesis block so start at 1
		for (int i = 1; i < chain.size(); i++) {
			Block b = chain.get(i);
			ss << "Block " << i << ": " << b.toString() << "\n";
		}

		// output pending transactions
		ss << "\nPending: " << pendingTransactions << "\n";

		ss << "------------------- End ------------------\n";
		return ss.str();
	}

private:
	const static int BLOCKSIZE = 2; // each block will only contain a fixed number of transactions
	const static int MAXDIFFICULTY = 5; // maximum difficulty level  

	LinkedList< Block > chain;
	ArrayList<Transaction> pendingTransactions;
	int difficulty;
	float miningReward;
	float bankBalance;

	// private member function to create genesis block - called in constructor
	void createGenesisBlock() {

		// genesis block to start the chain
		Block genesisBlock;
		genesisBlock.hash = genesisBlock.calculateHash();

		// add block to the chain
		chain.add(genesisBlock);
	}

	// PostCondition: last block in chain returned
	Block getLatestBlock() const {
		return chain.get(chain.size() - 1);
	}

};


// -----------Block/BlockChain/Transaction overloaded output operators ------
std::ostream& operator <<(std::ostream& output, const Block & b) {
	output << b.toString();
	return output;  // for multiple << operators.
}

std::ostream& operator <<(std::ostream& output, const BlockChain & c) {
	output << c.toString();
	return output;  // for multiple << operators.
}

std::ostream& operator <<(std::ostream& output, const Transaction & t) {
	output << t.toString();
	return output;  // for multiple << operators.
}
