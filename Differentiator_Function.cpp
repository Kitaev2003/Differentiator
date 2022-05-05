#include "Differentiator.h"

Differentiator::Differentiator ()
{
	Head_ = new Block_Tree;
}

Differentiator::~Differentiator()
{
	 setFree(Head_);
}

void setFree(Block_Tree*  Tree)
{
 	if (Tree->left != nullptr)
	{
		setFree(Tree->left);
	}
	if (Tree->right != nullptr)
	{
		setFree(Tree->right);
	}
	if(Tree->type == TPCHAR)
	{
		delete (Tree->val.cp);
	}

	delete (Tree);
	
} 

void BodyDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF)
{
	assert(Tree_Next);

	if(Tree_Next->type != 0)//Для предупреждения перехода по нулевому указателю
	{	
		switch (Tree_Next->type)
		{
			case TPCHAR:
				*GRAF << "	" << Tree_Next->num  << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = char*| Value = " << Tree_Next->val.cp <<"}\"];" << std::endl;
				break;
			case TCHAR:
				*GRAF << "	" << Tree_Next->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = char| Value = " << Tree_Next->val.c <<"}\"];" << std::endl;
				break;
			case TDOUBLE:
				*GRAF << "	" << Tree_Next->num  <<"[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = double| Value = " << Tree_Next->val.d <<"}\"];" << std::endl;
				break;
		}
		
		*GRAF << "	"<< Tree->num << "->" << Tree_Next->num << ";"<< std::endl;
	}

	if(Tree_Next->left != nullptr && Tree_Next->right != nullptr)//Для предупреждения перехода по нулевому указателю
	{
		BodyDump(Tree_Next, Tree_Next->left, GRAF);// Запускаем рекурсию
		BodyDump(Tree_Next, Tree_Next->right, GRAF);//Запускаем рекурсию
	}
}

void Differentiator::getDump()
{
	std::ofstream GRAF;//Обьявляем переменную типа fstream
	GRAF.open("Differentiator.dot");

	GRAF << "digraph graphname \n {" << std::endl;// digtaph, так как нужно указывать конкретное направление перехода к блоку
	GRAF << "node [shape=record, color = \"black\"]; //Описание блоков графа"<< std::endl;

	switch (Head_->type)
	{	
		case TPCHAR:
				GRAF << "	" <<  Head_->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = char*| Value = " << Head_->val.cp <<"}\"];" << std::endl;
			break;
		case TCHAR:
				GRAF << "	" <<  Head_->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type =  char| Value = " << Head_->val.c <<"}\"];" << std::endl;
			break;
		case TDOUBLE:
				GRAF << "	" << Head_->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = float| Value = " << Head_->val.d <<"}\"];" << std::endl;
			break;
	}

	GRAF << "//Далее работа функции BodyDump" << std::endl;
	BodyDump(Head_, Head_->left, &GRAF);//Левое дерево описание блоков

	GRAF << "\n}" << std::endl;
	GRAF.close();//Закрываем файл

	system("dot -Tpng Differentiator.dot -o Differentiator");//Делаем PNG-Дерево
}

void Differentiator::Insert(Block_Tree* elem, size_t type, Value val)
{
	assert(elem); 

	elem->type = type;
	elem->val = val;

	size_++;
	elem->num = size_;//for graphviz 
}

void Differentiator::Copy(Block_Tree* old_, Block_Tree* new_)
{
	new_->type = old_->type;
	new_->val = old_->val;

	new_->left = old_->left;
	new_->right = old_->right;

	size_++;
	new_->num = size_;//for graphviz 
}


void Differentiator::EquactionRead()
{
	char* buffer = new char[100];
	std::ifstream READ;
	
	READ.open("Equaction.txt");

	READ.getline(buffer, 100, '\n');

	READ.close();

	setReadGlobal(buffer);

	std::cout << buffer << std::endl;

	delete(buffer);
}

void Differentiator::setReadGlobal(char* buffer) 
{
	size_t i = 0;
	Value val;

    val.cp = "Begin";
	Insert(Head_, TPCHAR, val);

	Head_->left =  setReadAddAndSub(buffer, &i); 

	size_ = i++;
}

Block_Tree* Differentiator::setReadAddAndSub(char* buffer, size_t* i)
{
	Block_Tree* left;
	Value val;

	left = setReadMulAndDiv(buffer, i);

	while(true)
	{
		if(buffer[*i] == '+' || buffer[*i] == '-')
		{
			Block_Tree* node = new Block_Tree;

			node->left = left;

			val.c = buffer[*i];
			Insert(node, TCHAR, val);

			*i = *i + 1;
 
			node->right = setReadAddAndSub(buffer, i);
			return node;
		} 
		else
		{
			return left;
		}
		*i = *i + 1;
	}
}

Block_Tree* Differentiator::setReadMulAndDiv(char* buffer, size_t* i)
{
	Block_Tree* left;
	Value val;

	left = setReadDegree(buffer, i);

	while(true)
	{
		if(buffer[*i] == '*' || buffer[*i] == '/')
		{
			Block_Tree* node = new Block_Tree;

			node->left = left;

			val.c = buffer[*i];
			Insert(node, TCHAR, val);

			*i = *i + 1; 

			node->right = setReadMulAndDiv(buffer, i);
			return node;
		} 
		else
		{
			return left;
		}
		*i = *i + 1;
	}
}

Block_Tree* Differentiator::setReadDegree(char* buffer, size_t* i)
{
	Block_Tree* left; 
	Value val;

	left = setReadSpecFunc(buffer, i);

	while(true)
	{
		if(buffer[*i] == '^')
		{
			Block_Tree* node = new Block_Tree;

			node->left = left;

			val.c = buffer[*i];
			Insert(node, TCHAR, val);

			*i = *i + 1; 

			node->right = setReadDegree(buffer, i);
			return node;
		} 
		else
		{
			return left;
		}
		*i = *i + 1;
	}
}

Block_Tree* Differentiator::setReadSpecFunc(char* buffer, size_t* i)
{
	Block_Tree* left; 
	Value val;

	size_t k = 0;
	size_t n = *i;

	left = setReadParenthes(buffer, i);

	while(true)
	{
		if('a' <= buffer[n] && 'z' >= buffer[n] && buffer[n] != 'x' && buffer[n] != 'e')
		{
			char* func = new char[5];

			while('a' <= buffer[*i] && 'z' >= buffer[*i] && buffer[n] != 'x' && buffer[n] != 'e')
			{
				func[k] = buffer[*i];

				k++;
				*i = *i + 1;
			}
			func[k] = '\0';

			Block_Tree* node = new Block_Tree;

			node->left = left;

			val.cp = func;
			Insert(node, TPCHAR, val);

			node->right = setReadSpecFunc(buffer, i);
			return node;	
		}
		else 
		{
			return left; 
		}
	}
}

Block_Tree* Differentiator::setReadParenthes(char* buffer, size_t* i)
{
	if(buffer[*i] == '(' || buffer[*i] == ')')
	{	
		while(true)
		{
			*i = *i + 1;
			Block* node = setReadAddAndSub(buffer, i);
			node->parenthes = true;
			*i = *i + 1;
			return node;
		}
	}
	else
	{
		return setReadValAndNum(buffer, i);
	}
}

Block_Tree* Differentiator::setReadValAndNum(char* buffer, size_t* i)
{
	Block_Tree* node = new Block_Tree;
	int k = -1;
	size_t n = *i;
    Value val;

	if(buffer[*i] == 'x')//Для переменной
	{
		*i = *i +1;
        val.c = 'x';

        Insert(node, TCHAR, val);
		return node;
	}

	if(buffer[*i] == 'e')//Для переменной
	{
		*i = *i +1;
        val.d = exp(1);

        Insert(node, TDOUBLE, val);
		return node;
	}

	while('0' <= buffer[n] && '9' >= buffer[n])// Считаем порядок числа 
	{
		k++;
		n++;
	}

	val.d = 0;
	
	while(k >= 0)// Считываем число
	{
		val.d = val.d + (buffer[*i] - '0')* pow (10, k);
		*i = *i + 1;
		k--;

		if(buffer[*i] == '.')//Если встретили точку, то начинаем считывать дробную часть
		{	
			*i = *i + 1;//Cдвигаем указатель с точки
			while('0' <= buffer[*i] && '9' >= buffer[*i])//Считываем дробную часть числа 
			{
				val.d = val.d + (buffer[*i] - '0')* pow (10, k);
				*i = *i + 1;
				k--;
			}
		}
	}

	if('0' <= buffer[n-1] && '9' >= buffer[n-1])
	{
		Insert(node, TDOUBLE, val);
	}

	return node; 
}

void Differentiator::Reduction()
{
	bool repeat = true;

	while(repeat)
	{
		repeat = false;
		setNumAndNum(Head_->left, &repeat);
		setSpecFuncAndNum(Head_->left, &repeat);
	}
}

void Differentiator::setNumAndNum(Block_Tree* node, bool* repeat)
{
	if(node->left != nullptr && node->right != nullptr && node->left->type == TDOUBLE &&  node->right->type == TDOUBLE)
	{
		switch (node->val.c)
		{
			case '*':
				node->val.d = node->left->val.d * node->right->val.d;
				break;
			case '/':
				node->val.d = node->left->val.d / node->right->val.d;
				break;
			case '+':
				node->val.d = node->left->val.d + node->right->val.d;
				break;
			case '-':
				node
				->val.d = node->left->val.d - node->right->val.d;
				break;
			case '^':
				node->val.d = pow(node->left->val.d, node->right->val.d); 
				break;
			default:
				exit(1);
				printf("Проблемы с сверткой констант");
		}
		*repeat = true;
		node->type = TDOUBLE;
		
		delete(node->left);
		delete(node->right);

		node->left = nullptr;
		node->right = nullptr;
	}

    else if(node->left != nullptr && node->right != nullptr)
    {
	    setNumAndNum(node->left, repeat);
	    setNumAndNum(node->right, repeat);
    }
}

void Differentiator::setSpecFuncAndNum(Block_Tree* node, bool* repeat)
{
	if(node->type == TPCHAR  && node->right->type == TDOUBLE)
	{	
		delete(node->val.cp);
		if (node->val.cp[0] == 'a')//Небольшое ускорение
		{
			if(strcmp(node->val.cp, "arcsin") == 0)
			{
				node->val.d = asin(node->right->val.d); 
			}
			else if(strcmp(node->val.cp, "arccos") == 0)
			{
				node->val.d = acos(node->right->val.d); 
			}
			else if(strcmp(node->val.cp, "arctg") == 0)
			{
				node->val.d = atan(node->right->val.d); 
			}
		}

		else
		{
			switch (node->val.cp[0])
			{
				case 's':
					node->val.d = sin(node->right->val.d);
					break;
				case 'c':
					node->val.d = cos(node->right->val.d);
					break;
				case 't':
					node->val.d = tan(node->right->val.d);
					break;
				case 'l':
					node->val.d = log(node->right->val.d);
					break;
				default:
					exit(1);
					printf("Проблемы с сверткой констант");
					break;
			}
		}

		*repeat = true;
		node->type = TDOUBLE;

		delete(node->right);
		node->right = nullptr;
	}
	else if(node->left != nullptr && node->right != nullptr)
    {
		setSpecFuncAndNum(node->left, repeat);
		setSpecFuncAndNum(node->right, repeat);
    }
}

Block_Tree* Differentiator::Derivative(Block_Tree* node)
{
	std::cout << "Derivative work" << std::endl;
	switch (node->type)
	{
	case TCHAR:
		DiffStandartFunc(node);
		break;
	case TDOUBLE:
		delete(node);
		break;
	default:
		std::cout << "Произошла ошибка" << std::endl;
		break;
	}
	return node;
}

Block_Tree* Differentiator::DiffStandartFunc(Block_Tree* node)
{
	Value val;

	switch(node->val.c)
	{
		case '*'://(uv)' = u'v + uv'
		{
			std::cout << "We in the case \"*\"" << std::endl;
			Block_Tree* old_left = new Block_Tree;
			Block_Tree* old_right = new Block_Tree;

			Copy( node->left, old_left);
			Copy(node->right, old_right);

			Block_Tree* next_mul1 = new Block_Tree;
			Block_Tree* next_mul2 = new Block_Tree;
			
			node->val.c = '+';//+

			val.c = '*';
			Insert(next_mul1, TCHAR, val);//*
			Insert(next_mul2, TCHAR, val);//*

			next_mul1->left = old_left;//*v
			next_mul2->right = old_right;//*u
			
			next_mul1->right = Derivative(node->left);//u'*v
			next_mul2->left = Derivative(node->right);//u*v'

			node->left = next_mul1;//u'*v+
			node->right= next_mul2;//u'*v+u*v'

			break;
		}

		case '/':  //(u/v)' =(u'*v - u*v')/(v^2) 
		{
			std::cout << "We in the case \"/\"" << std::endl;
			node->val.c = '/';
			
			Block_Tree* old_left = new Block_Tree;
			Block_Tree* old_right = new Block_Tree;

			Copy( node->left, old_left);
			Copy(node->right, old_right);

			Block_Tree* next_sub = new Block_Tree;
			Block_Tree* next_mul1 = new Block_Tree;
			Block_Tree* next_mul2 = new Block_Tree;
			Block_Tree* next_degree = new Block_Tree;
			Block_Tree* next_two = new Block_Tree;

			val.c = '-';
			Insert(next_sub, TCHAR, val);//-
			
			val.c = '*';
			Insert(next_mul1, TCHAR, val);//*
			Insert(next_mul2, TCHAR, val);//*

			next_sub->left = next_mul1;//*-
			next_sub->right = next_mul2;//*-*
			
			next_mul1->left = old_right;//*v-*
			next_mul2->right = old_left;//*v-u*

			val.c = '^';
			Insert(next_degree, TCHAR, val);//^

			val.d = 2;
			Insert(next_two, TDOUBLE, val);//2

			next_degree->right = next_two;//^2

			next_degree->left = old_right;//v^2

			next_mul1->right = Derivative(node->left);//u'*v-u*
			next_mul1->left = Derivative(node->right);//u'*v-u*v'

			node->left = next_sub;//(u'*v-u*v')
			node->right = next_degree;//(u'*v-u*v')/(v^2)
			break;
		}

		case '+'://(u+v) = u' + v'
		{
			std::cout << "We in the case \"+\"" << std::endl;
			node->left = Derivative(node->left);
			node->right = Derivative(node->right);
			break;
		}

		case '-'://(u-v) = u' - v'
		{
			std::cout << "We in the case \"-\"" << std::endl;
			node->left = Derivative(node->left);
			node->right = Derivative(node->right);
			break;
		}

		case '^'://(u^const)' = const*u'*u^(const-1)
		{
			if(node->left->type != TDOUBLE)
			{
				std::cout << "We in the case \"^\"" << std::endl;
				node->val.c = '*';

				Block_Tree* next_mul = new Block_Tree;
				Block_Tree* next_const = new Block_Tree;
				Block_Tree* old_left = new Block_Tree;
				Block_Tree* next_degree = new Block_Tree;
				Block_Tree* old_degree = new Block_Tree;
				
				Copy(node->left, old_left);
				Copy(node->right, next_const);
				next_const->val.d--;//const-1

				val.c = '*';
				Insert(next_mul, TCHAR, val);//*
				val.c = '^';
				Insert(next_degree, TCHAR, val);//^

				next_degree->left = old_left;//u^
				next_degree->right = next_const;//u^(const-1)

				Copy(node->right, old_degree);				

				next_mul->left = Derivative(node->left);//u'*
				next_mul->right = next_degree;//u'*u^(const-1)

				node->left = old_degree;	
				node->right = next_mul;//(u^const)' = const*u'*u^(const-1)
			}
			break;
		}

		case 'x':
		{
			std::cout << "We in the case \"x\"" << std::endl;
			node->type = TDOUBLE;
			node->val.d = 1;
			break;
		}

		default:
		{
			std::cout << "Такой функции нет" << std::endl;
		}
	}
	return node;
}

void DumpEquatiton(Block_Tree* node)
{	
	if(node->parenthes)
	{
		std::cout << "(";
		node->parenthes = false;
		DumpEquatiton(node);
		std::cout << ")";
	}
	else
	{
		if(node->left != nullptr)
		{
			DumpEquatiton(node->left);
		}
		switch(node->type)	
		{
			case TCHAR:
				std::cout <<node->val.c;
				break;
			case TDOUBLE:
				std::cout << node->val.d;		
				break;
			case TPCHAR:
				std::cout << node->val.cp;
				break;
		}
		if(node->right != nullptr)
		{
			DumpEquatiton(node->right);
		}
	}
}