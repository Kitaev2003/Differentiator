#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstring>

#define TPCHAR 1
#define TCHAR 2
#define TDOUBLE 3
#define SIZEBLOCK 48
#define DOUBLEPTR 0.001

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

	size_t num = 0;
	
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
	 
	Block_Tree* HeadRead_ = nullptr;
	Block_Tree* HeadWrite_ = nullptr;
	
	Differentiator();
	~Differentiator();

	void Insert(Block_Tree* elem, size_t type, Value val);

	//Read and make Tree
	void EquactionRead();	
	Block_Tree* setReadAddAndSub(char* buffer, size_t* i);
	Block_Tree* setReadMulAndDiv(char* buffer, size_t* i);
	Block_Tree* setReadDegree(char* buffer, size_t* i);
	Block_Tree* setReadSpecFunc(char* buffer, size_t* i);
	Block_Tree* setReadParenthes(char* buffer, size_t* i);
	Block_Tree* setReadValAndNum(char* buffer, size_t* i);
	
	void Reduction(Block_Tree* Head);
	void setNulAndFunc(Block_Tree* node, bool* repeat);
	void setNumAndNum(Block_Tree* node, bool* repeat);
	void setAddAndSub(Block_Tree* node, bool* repeat);
	void setSpecFuncAndNum(Block_Tree* node, bool* repeat);
	
	Block_Tree* Derivative(const Block_Tree* node);
	Block_Tree* DiffStandartFunc(const Block_Tree* node);

	private:
	
	size_t size_ = 0;
};

void DumpEquatiton(Block_Tree* node);
void getDump(Block_Tree* Head);
void BodyDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF, size_t* num, size_t* num_next);
void setFree(Block_Tree*  Tree);