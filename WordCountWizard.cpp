
//============================================================================
// Name         : lab12.cpp
// Author       : Khalid Mengal
// Version      : 1.0
// Date Created : 25-04-2020
// Date Modified: 22-11-2020 
// Copyright    : All rights are reserved
// Description  : HashTable using seperate chaining
//============================================================================

#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include<string.h>
#include<string>
#include <ctype.h>
#include<ctime>
#include<stdlib.h>
#include<stdio.h>


using namespace std;


class HashNode								//our basic hashnode unit
{
	private:
		string key;							//stores the word
		int value;							//stores frequency of the word
		HashNode* left;
		HashNode* right;

	public:
		HashNode(string key)				//constructor
		{
			this->key = key;
			this->value = 1;
			this->left = nullptr;
			this->right=nullptr;
		}
		string getKey() { return this->key;}		//getter functions
		int getVal() {return this->value;}
		friend class HashTable;
		friend class OrderMap;

};

class Heap 										//heap class is used to store data to find out the word with the maximum frequency
												//Here a max hap is used
{
	private:
		HashNode** array;						//array stores pointers of pointers
		int capacity;
	
	public:
		int size;
		Heap(int capacity);
		Heap()
		{	capacity=5;
			size=0;
			array=new HashNode*[capacity];}
		~Heap();
		void insertNode(HashNode* key);
		HashNode* getMax();
		int parent(int k);
		int left(int k);
		int right(int k);
		void siftup(int k);
	private:
		void adjustSize();
};

// Constructor
Heap::Heap(int capacity)
{
	//todo
	this->capacity=capacity;
	size=0;
	array=new HashNode*[capacity];
}
//Destructor
Heap::~Heap()
{
	delete []array;
}
// Insert an element in Heap keeping the Heap property intact 
void Heap::insertNode(HashNode* key)
{

	if (size<capacity)
	{	array[size]=key;
		siftup(size);
		size+=1;}
	else
	{	adjustSize();
		insertNode(key);}
}

// Return (but don't remove) the maximum value from the Heap
HashNode* Heap::getMax()
{

	if (size>0)
		return array[0];
}
// Returns the index of the parent of the node i
int Heap::parent(int k)
{

	return floor((k-1)/2);
}
// Returns the index of the left child of the node i
int Heap::left(int k)
{

	return (2*k)+1;
}
// Returns the index of the right child of the node i
int Heap::right(int k)
{

	return (2*k)+2;
}

// Sift-up an element at index k
void Heap::siftup(int k)
{
	
	HashNode* temp;
	while(1)
	{	if (k==0)
			break;
		else if (array[parent(k)]->getVal()<array[k]->getVal())
		{	temp=array[k];
			array[k]=array[parent(k)];
			array[parent(k)]=temp;
			k=parent(k);}
		else
			break;}
}

//=============================================
void Heap::adjustSize()
{
	HashNode** newArray = new HashNode*[capacity*2];
	for(int i=0; i<capacity; i++)
		newArray[i] = array[i];
	delete[] array;
	array = newArray;
	capacity = capacity*2;	
}
//=============================================


class OrderMap										//implementation of ordered map through an AVL tree
{
	private:
		HashNode *root;
	public:
		Heap heapmaxi;
		OrderMap();									//constructor
		~OrderMap();									//destructor
		HashNode* getRoot();						//returns the root of the Tree
		HashNode* insert(HashNode* ptr,string key);		//Insert key into tree/subtree with root ptr
		int height(HashNode *ptr);				    //Find the height(MaxDepth) of a tree/subtree with root ptr
		int size(HashNode *ptr);					//Find total number of elements in tree rooted at ptr
		int sum_values(HashNode* ptr);				//Find sum of values of all elements in tree rooted at ptr
		HashNode* findMin(HashNode* ptr);         		//Find and return the Node with Min key value from a tree/subtree with root ptr	
		HashNode* findMax(HashNode *ptr);				//Find and return the Node with Max key value from a tree/subtree with root ptr		
		HashNode* findKey(HashNode *ptr,string key);		//Find and returns the node with key
		HashNode* remove(HashNode *ptr,string key);		//Remove a node with key from the tree/subtree with root
		HashNode* rotateLeft(HashNode *z);
		HashNode* rotateRight(HashNode *z);
		void heapinserter(HashNode* ptr);
	public:	//helper methods
		void insert_helper(string key);			//Helper method for insert method
		void remove_helper(string key);			//Helper method for remove method
	private:
		int balanceFactor(HashNode *ptr);
		HashNode* rebalance(HashNode *ptr);    			// rebalance a node
};

OrderMap::OrderMap()
{
	this->root=nullptr;
}
OrderMap::~OrderMap()
{
	//Keep removing the Node with Min value util entire tree becomes empty
	while(root!=nullptr)
		remove_helper(findMin(root)->key);
}

int OrderMap::size(HashNode* ptr)
{
	if (ptr!=nullptr)
		return 1+size(ptr->left)+size(ptr->right);
	else
		return 0;
}

int OrderMap::sum_values(HashNode* ptr)										//used to find the sum total of all values ie. total number of words
{
	if (ptr!=nullptr)
		return ptr->value+sum_values(ptr->left)+sum_values(ptr->right);
	else
		return 0;
}

void OrderMap::heapinserter(HashNode* ptr)									//used to insert into the heap
{
	if (ptr!=nullptr)
	{	heapmaxi.insertNode(ptr);
		heapinserter(ptr->left);
		heapinserter(ptr->right);}
}
// Return the root of the tree
HashNode* OrderMap::getRoot()
{
	return root;
}

//Find the Height (MaxDepth) of the tree/subtree rooted at ptr
int OrderMap::height(HashNode* ptr)
{
	if(ptr==nullptr) return -1;
	return max(height(ptr->left),height(ptr->right))+1;
}

// Find recursively (or iteratively ) the key with minimum in the tree/subtree rooted at ptr
HashNode* OrderMap::findMin(HashNode *ptr)
{

	if(ptr==nullptr) 		return nullptr; //if Tree is empty
	else if(ptr->left)		//recursive approach to find the node with minimum key in the tree
		return findMin(ptr->left);
	else								//if reached to the left most node
		return ptr;
}

// Find recursively (or iteratively ) the key with Maximum in the tree/subtree rooted at ptr
HashNode* OrderMap::findMax(HashNode *ptr)
{

	if(ptr==nullptr) 		return nullptr; //if Tree is empty
	else if(ptr->right)		//recursive approach to find the node with Maximum key in the tree
		return findMax(ptr->right);
	else								//if reached to the right most node
		return ptr;
}
// Find recursively the node with key value in the tree/subtree rooted at ptr
HashNode* OrderMap::findKey(HashNode *ptr,string key)
{
	if(ptr==nullptr) 			return nullptr;
	else if(key == ptr->key) 	return ptr;
	else if(strcmp(key.c_str(),ptr->key.c_str())<0) 	return findKey(ptr->left,key);
	else						return findKey(ptr->right,key);
}
// Calculate and returns the Balance Factor of a Node
int OrderMap::balanceFactor(HashNode *ptr)
{
	//todo
	int leftheight=height(ptr->left);
	int rightheight=height(ptr->right);
	return leftheight-rightheight;
}
//Helper method for insert method
void OrderMap::insert_helper(string key)
{	
	root=insert(root,key);
}
//Insert recursively the key in the tree/subtree rooted at ptr
HashNode* OrderMap::insert(HashNode *ptr, string key)
{
	if (ptr ==nullptr) return (new HashNode(key)); 		//if tree is empty or if we found the right place for new node 

	if(strcmp(key.c_str(),ptr->key.c_str())<0)							// if the key is smaller
			ptr->left = insert(ptr->left,key);   	// recursively insert new node in left subtree
	else if(strcmp(key.c_str(),ptr->key.c_str())>0)						// if the key is greater
			ptr->right = insert(ptr->right,key);	// recursively insert new node into right subtree
	else	//if key already present, then we increase the value associated wiht it by 1
	{
		ptr->value+=1;
	}
	ptr = rebalance(ptr);
	return ptr;
}
//==========================================
// Rotate a node to its right
HashNode* OrderMap::rotateRight(HashNode *z)
{  
	//todo
	HashNode* y=z->left;
	HashNode* T1=y->right;
	y->right=z;
	z->left=T1;
	return y;
}  
//==========================================
// Rotate a node to its left  
HashNode* OrderMap::rotateLeft(HashNode *z)
{  
	//todo
	HashNode* y=z->right;
	HashNode* T1=y->left;
	y->left=z;
	z->right=T1;
	return y;
}
//============================================
// Rebalane a node (ptr)
HashNode* OrderMap::rebalance(HashNode *ptr)
{	int factor=balanceFactor(ptr);
	// case 0: Already balanced
	 
	if(factor==-2)
	{	int factorright=balanceFactor(ptr->right);
		// case 1: right right heavey
		if (factorright==-1)
			ptr=rotateLeft(ptr);
		// case 3: right left heavey 
		else
		{	ptr->right=rotateRight(ptr->right);
			ptr=rotateLeft(ptr);}
	}
	if(factor==2)
	{	int factorleft=balanceFactor(ptr->left);
		// case 2: left left heavey 
		if (factorleft==1)
			ptr=rotateRight(ptr);
		// case 4: left right heavey
		else
		{	ptr->left=rotateLeft(ptr->left);
			ptr=rotateRight(ptr);}
	}
	
	return ptr;
}

//=================================================
//Helper method for remove 

void OrderMap::remove_helper(string key)
{
	if(findKey(root,key)==nullptr)
		cout<<"Key not found..."<<endl;
	else	
		root = remove(root, key);
}

//Remove recursively the node with key value from the tree/subtree rooted at ptr
HashNode* OrderMap::remove(HashNode *ptr,string key)
{
	if(strcmp(key.c_str(),ptr->key.c_str())<0)						//if key is smaller than the key at root of the tree/subtree
		ptr->left = remove(ptr->left, key);		//then search in left subtree
	else if(strcmp(key.c_str(),ptr->key.c_str())>0)
		ptr->right = remove(ptr->right, key);	//else search in right subtree
	else		//The node to be deleted has been found		
	{
		if(ptr->left == nullptr && ptr->right == nullptr)		//if the node to be deleted is a Leaf node then remove it
		{
			delete ptr;
			ptr=nullptr;
			return ptr;
		}
		else if(ptr->right == nullptr)		//if the node to be deleted has no right child 
		{
			HashNode* tmp = ptr;				//then remove it and connect its left child/subtree to its parent
			ptr = ptr->left;
			delete tmp;
		}
		else if(ptr->left == nullptr)		//if the node to be deleted has no left child
		{
			HashNode* tmp = ptr;				//then remove it and connect its right child/subtree to its parent
			ptr = ptr->right;
			delete tmp;
		}
		else										//if the node to be deleted has both left and right children
		{
			HashNode *min = findMin(ptr->right);		//find the node with minimum key in its right subtree 
			swap(ptr->key,min->key);				//and replace it with the minimum node found in right subtree, 
													//We swapped instead of copying so the correct deleted message will be printed
			ptr->right = remove(ptr->right, min->key);	//recursivly remove the minimum node from right subtree
		}
	}
	ptr=rebalance(ptr);
	return ptr;
}



class HashTable
{
	private:
		OrderMap *buckets;				// Array of lists of type Hash Nodes
		int size;					    //Current Size of HashTable
		int capacity;				    // Total Capacity of HashTable
		int collisions;					// Total Number of Collisions(I only consider a collision to have occured when a unique word has the same hashcode)
		int A=rand();
		int B=rand();
		Heap heap_max;
	public:
		int hashtype=0;					//to toggle type of hashcode
		HashTable(int capacity);
		unsigned long hashCode(const string key);
		void insert(const string key);
		int find_freq(const string key);	//finds frequency of a given word
		int getSize();
		int count_collisions();
		string find_max();					//finds and returns words with highest frequency
		int count_words();					//counts total words
		int count_unique_words();			//counts total unique words
		void switch_hash_fun(int i);		//switches hash function
		~HashTable();

};
HashTable::HashTable(int capacity)
{
	buckets = new OrderMap[capacity];			//Create an array of size capacity. Each locaiton of Array is a list/chain
	this->capacity = capacity;
	this->size = 0;
	this->collisions = 0;
}

void HashTable::switch_hash_fun(int i)
{	if (i==0||i==1||i==2||i==3)
		hashtype=i;
	else
	{	cout<<"Invalid choice, default hashing will be used"<<endl;}
}

unsigned long HashTable::hashCode(const string key)
{
	unsigned long code=0;
	while(A%capacity==0)
		A=rand();
	while(B%capacity==0)
		B=rand();

	if (hashtype==0)	
	{	
		int i=0;
		int x=1;
		while(i<key.length())
		{	code+=pow(x,10)*int(key[i]);
			i++;
			x++;}
		return code%capacity;
	}
	else if (hashtype==1)
	{	
		for (int i = 0; i < key.length(); i++) {
			code = (code << 5) | (code >> 27);
			code += (unsigned int) key[i];
		}
		return code%capacity;
	}

	else if (hashtype==2)
	{
		int i=0;
		int x=1;
		while(i<key.length())
		{	code+=pow(x,10)*int(key[i]);
			i++;
			x++;}

		return ((A*code)+B)%capacity;
	}
	else if (hashtype==3)
	{
		for (int i = 0; i < key.length(); i++) {
			code = (code << 5) | (code >> 27);
			code += (unsigned int) key[i];
		}
		return ((A*code)+B)%capacity;
	}
	

}
//================================================
void HashTable::insert(const string key)							//calculates hashcode, then inserts in the following way:
{
	//TODO
	unsigned long code=hashCode(key);

	if(buckets[code].getRoot()==nullptr)							//if at the given position, no element is present, a node is simply inserted
	{	buckets[code].insert_helper(key);
		size+=1;}
	else if(buckets[code].findKey(buckets[code].getRoot(),key)==nullptr)	//if there are elements and none of the elements has the same key as the key to be inserted, then the key is added to the tree, and a collision is ocunted
		{collisions+=1;
		buckets[code].insert_helper(key);
		}
	else
		buckets[code].insert_helper(key);

}
//===================================================
int HashTable::find_freq(const string key)								//calculates hashcode of a given string, and reaches appropriate bucket
{
	//TODO
	unsigned long code=hashCode(key);
	HashNode* x=buckets[code].findKey(buckets[code].getRoot(), key);
	if (x==nullptr)
		return 0;
	else
		return x->value;
}

//=====================================================
int HashTable::getSize()
{
	return this->size;
}
//======================================================
int HashTable::count_collisions()
{
	return this->collisions;
}
//======================================================
int HashTable::count_words()											//iterates thrugh all AVL trees in hash table and adds up the values of every HashNode
{	
	int totalwords=0;
	for(int i=0;i<capacity;i++)
		totalwords+=buckets[i].sum_values(buckets[i].getRoot());
	return totalwords;
}
//======================================================
int HashTable::count_unique_words()										//iterates through every AVL tree in the hashtable and basically adds up the sizes of all of them
{
	int uniquewords=0;
	for(int i=0;i<capacity;i++)
		uniquewords+=buckets[i].size(buckets[i].getRoot());
	return uniquewords;
}
//======================================================
string HashTable::find_max()
{	
	if(heap_max.size==0)
	{for(int i=0;i<capacity;i++)
	{	if(buckets[i].size(buckets[i].getRoot())==0)
			continue;
		if(buckets[i].heapmaxi.size==0)
			buckets[i].heapinserter(buckets[i].getRoot());
		heap_max.insertNode(buckets[i].heapmaxi.getMax());}}

	return heap_max.getMax()->getKey();
}
//======================================================
HashTable::~HashTable()
{
	for(int i=0;i<capacity;i++)
	{	if(buckets[i].getRoot()!=nullptr)
			buckets[i].~OrderMap();}
	delete[] this->buckets;
}


//======================================================
bool isprime(int num)									//checks to see if a number is prime
{	for(int i=2;i<sqrt(num);i++)
	{	if (num%i==0)
			return 0;}
	return 1;}
int nextprime(int num)									//calculates the next prime number from a number
{	
	while(!isprime(num))
		num+=1;
	return num;
}
bool check_int(string a)								//checks if a string is an integer
{	for (int i = 0; i < a.length(); i++)
  		if (isdigit(a[i]) == 0)
    		return 0;
	return 1;}
int main(void)
{
	ifstream fin;
	string file;
	string hashtype;
	string wordcount;
	int words=0;
	string line;
	cout<<"Welcome to Word Count Wizard!!"<<endl;
	cout<<"Enter File name/path"<<endl;
	cin>>file;
	fin.open(file);
	while(!fin){
		string option;
		cout<<"No such file exists!"<<endl;
		cout<<"Enter 1 to try another file, enter any other key to exit"<<endl;
		cin>>option;
		if(option=="1")
		{	cout<<"Enter File name/path"<<endl;
			cin>>file;
			fin.open(file);}
		else
			exit(-1);}
	

	cout<<endl<<endl<<"Enter hash function to use: (Enter d for default)\nEnter 0 for Polynomial with Division\nEnter 1 for Polynomial with MAD\nEnter 2 for 5-Cyclic Shift with division\nEnter 3 for 5-Cyclic Shift with MAD"<<endl<<endl;
	cin>>hashtype;
	if(hashtype=="d")
		hashtype="1";
	else
	{while(!check_int(hashtype)||!(0<=stoi(hashtype)<4))
	{	cout<<"Invalid input"<<endl;
		cin>>hashtype;}}
	while(!fin.eof())
	{	getline(fin,wordcount,' ');
		words+=1;}
	fin.clear();
	fin.seekg(0);
	int hashsize=nextprime(words*1.25);	//calculates prime number greater than 1.25* total count of words. we consider total number of words as it will be the worst case where a words are unique
	HashTable myHashTable(hashsize);
	myHashTable.switch_hash_fun(stoi(hashtype));
	while(!fin.eof())
	{
		string key="";								//word identifying part of the code, basically runs some checks before adding a word
		string key1="";
		while(key.length()==0)
			getline(fin,key,' ');
		if(key.find("\n")!=string::npos)
		{	key1=key.substr(0,key.find("\n"));
			key=key.substr(key.find("\n"));
			}
		int beg=0;
  	 	int end=key.length()-1;
   		while(end>=0 && !isalnum(key[end]))		//sets beg to position where alphanumeric characters start(useful when extracting words in quotations)
   		    end-=1;
    	while(beg<=key.length()-1 && !isalnum(key[beg]))		//sets end to position where alphanumeric characters end
   	    	beg+=1;
    	if(end-beg<0)					//case where a word only has special characteristics
			continue;
    	key=key.substr(beg,end-beg+1);
		for (int i=0;i<key.length();i++)		//converts the word to lowercase
       		key[i]=tolower(key[i]);
		myHashTable.insert(key);			//inserts word into the hashtable
		if(key1!="")
		{	int beg=0;
	  	 	int end=key1.length()-1;
	   		while(end>=0 && !isalnum(key1[end]))		//sets beg to position where alphanumeric characters start(useful when extracting words in quotations)
	   		    end-=1;
	    	while(beg<=key1.length()-1 && !isalnum(key1[beg]))		//sets end to position where alphanumeric characters end
	   	    	beg+=1;
	    	if(end-beg<0)					//case where a word only has special characteristics
				continue;
	    	key1=key1.substr(beg,end-beg+1);
			for (int i=0;i<key1.length();i++)		//converts the word to lowercase
	       		key1[i]=tolower(key1[i]);
			myHashTable.insert(key1);			//inserts word into the hashtable
		}

	}
	fin.close();
	cout<<endl<<endl<<endl;
	cout<<"Analysis"<<endl;													//outputs the analysis screen with the required commands
	cout<<"==================================================="<<endl;
	cout<<"Total words = "<<myHashTable.count_words()<<endl;
	cout<<"Total Number of Collisions = "<<myHashTable.count_collisions()<<endl;
	cout<<"Number of unique words = "<<myHashTable.count_unique_words()<<endl;
	cout<<"==================================================="<<endl<<endl<<endl;

	string user_input="";										//menu
	string command="";
	string argument1="";
	cout<<"Commands available: \nFind the frquency of a given word: freq <word>\nFind word with highest frequency: maxfreq\nQuit: exit"<<endl<<endl;
	while(true)
	{
		cout<<">";
		getline(cin,user_input);

		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,argument1);

			if(command == "freq") cout<<"Frequency of "<<argument1<<" is: "<<myHashTable.find_freq(argument1)<<endl;
		else if(command == "maxfreq") cout<<"The most frequently occuring word is: "<<myHashTable.find_max()<<" : Count = "<<myHashTable.find_freq(myHashTable.find_max())<<endl;
		else if(command == "help")	  cout<<"Commands available: \nFind the frquency of a given word: freq <word>\nFind word with highest frequency: maxfreq\nQuit: exit"<<endl;
		else if(command == "exit")	  break;
		else if(command=="")		continue;
		else cout<<"Invalid command !!"<<endl;
	}
	return 0;
}



