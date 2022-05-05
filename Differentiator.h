#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstring>

#define TPCHAR 1
#define TCHAR 2
#define TDOUBLE 3

union Value
{
	const char* cp; 
	
	char  c;

	double d;
};

typedef struct Block
{	
	Value val;
		
	Block* left =  nullptr;
	
	Block* right = nullptr;

	int num = 0;
	
	size_t type = 0;

	bool parenthes = false;
}Block_Tree;

enum Function
{
 	Eadd = 1,
	Esub,
	Emul,
	Ediv,
	Edeg,
	Esin,
	Ecos,
	Etan,
	Earcsin,
	Earccos,
	Earctan,
};

class Differentiator{
	public:
	 
	Block_Tree* Head_ = nullptr;
	
	Differentiator();
	~Differentiator();
	
	void getDump();

	void Insert(Block_Tree* elem, size_t type, Value val);
	void Copy(Block_Tree* old_, Block_Tree* new_);

	//Read and make Tree
	void EquactionRead();	
	void setReadGlobal(char* buffer);
	Block_Tree* setReadAddAndSub(char* buffer, size_t* i);
	Block_Tree* setReadMulAndDiv(char* buffer, size_t* i);
	Block_Tree* setReadDegree(char* buffer, size_t* i);
	Block_Tree* setReadSpecFunc(char* buffer, size_t* i);
	Block_Tree* setReadParenthes(char* buffer, size_t* i);
	Block_Tree* setReadValAndNum(char* buffer, size_t* i);
	
	void Reduction();
	void setNumAndNum(Block_Tree* node, bool* repeat);
	void setSpecFuncAndNum(Block_Tree* node, bool* repeat);
	
	Block_Tree* Derivative(Block_Tree* node);
	Block_Tree* DiffStandartFunc(Block_Tree* node);

	private:
	
	size_t size_ = 0;
};

void DumpEquatiton(Block_Tree* node);
void BodyDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF, size_t* num, size_t* num_next);
void setFree(Block_Tree*  Tree);