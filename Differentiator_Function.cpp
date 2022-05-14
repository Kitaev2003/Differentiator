#include "Differentiator.h"

Differentiator::Differentiator()
{
	HeadRead_ = new Block_Tree;
	HeadWrite_ = new Block_Tree;
}

Differentiator::~Differentiator()
{
	setFree(HeadRead_);
	setFree(HeadWrite_);
}

void setFree(Block_Tree *Tree)
{
	if (Tree->left != nullptr)
	{
		setFree(Tree->left);
	}
	if (Tree->right != nullptr)
	{
		setFree(Tree->right);
	}
	if (Tree->type == TPCHAR)
	{
		delete (Tree->val.cp);
	}

	delete (Tree);
}

void BodyDump(Block_Tree *Tree, Block_Tree *Tree_Next, std::ofstream *GRAF)
{
	assert(Tree_Next);

	if (Tree_Next->type != 0) //Для предупреждения перехода по нулевому указателю
	{
		switch (Tree_Next->type)
		{
		case TPCHAR:
			*GRAF << "	" << Tree_Next->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = char*| Value = " << Tree_Next->val.cp << "}\"];" << std::endl;
			break;
		case TCHAR:
			*GRAF << "	" << Tree_Next->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = char| Value = " << Tree_Next->val.c << "}\"];" << std::endl;
			break;
		case TDOUBLE:
			*GRAF << "	" << Tree_Next->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = double| Value = " << Tree_Next->val.d << "}\"];" << std::endl;
			break;
		}

		*GRAF << "	" << Tree->num << "->" << Tree_Next->num << ";" << std::endl;
	}

	if (Tree_Next->left != nullptr ) //Для предупреждения перехода по нулевому указателю
	{
		BodyDump(Tree_Next, Tree_Next->left, GRAF);	 // Запускаем рекурсию
	}
	if(Tree_Next->right != nullptr)
	{
		BodyDump(Tree_Next, Tree_Next->right, GRAF); //Запускаем рекурсию
	}
}

void getDump(Block_Tree* Head)
{
	std::ofstream GRAF; //Обьявляем переменную типа fstream
	GRAF.open("Differentiator.dot");

	GRAF << "digraph graphname \n {" << std::endl; // digtaph, так как нужно указывать конкретное направление перехода к блоку
	GRAF << "node [shape=record, color = \"black\"]; //Описание блоков графа" << std::endl;

	switch (Head->type)
	{
	case TPCHAR:
		GRAF << "	" << Head->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = char*| Value = " << Head->val.cp << "}\"];" << std::endl;
		break;
	case TCHAR:
		GRAF << "	" << Head->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type =  char| Value = " << Head->val.c << "}\"];" << std::endl;
		break;
	case TDOUBLE:
		GRAF << "	" << Head->num << "[fillcolor = \"#40e0d0\", style=\"rounded,filled\", label=\"{Type = float| Value = " << Head->val.d << "}\"];" << std::endl;
		break;
	}

	GRAF << "//Далее работа функции BodyDump" << std::endl;

	if (Head->left != nullptr ) //Для предупреждения перехода по нулевому указателю
	{
		BodyDump(Head, Head->left, &GRAF);	 // Запускаем рекурсию
	}
	if(Head->right != nullptr)
	{
		BodyDump(Head, Head->right, &GRAF); //Запускаем рекурсию
	}

	GRAF << "}" << std::endl;
	GRAF.close(); //Закрываем файл

	system("dot -Tpng Differentiator.dot -o Differentiator"); //Делаем PNG-Дерево
}

void DumpEquatiton(Block_Tree *node)
{
	if (node->parenthes)
	{
		std::cout << "(";
		node->parenthes = false;
		DumpEquatiton(node);
		std::cout << ")";
	}
	else
	{
		if (node->left != nullptr)
		{
			DumpEquatiton(node->left);
		}
		switch (node->type)
		{
		case TCHAR:
			std::cout << node->val.c;
			break;
		case TDOUBLE:
			std::cout << node->val.d;
			break;
		case TPCHAR:
			std::cout << node->val.cp;
			break;
		}
		if (node->right != nullptr)
		{
			DumpEquatiton(node->right);
		}
	}
}

void Differentiator::Insert(Block_Tree *elem, size_t type, Value val)
{
	assert(elem);

	elem->type = type;
	elem->val = val;

	size_++;
	elem->num = size_; // for graphviz
}

Block_Tree *Differentiator::Derivative(const Block_Tree *node)
{
	switch (node->type)
	{
		case TCHAR:
			return DiffStandartFunc(node);
			break;
		case TDOUBLE:
		{
			Block_Tree* new_node = new Block_Tree;
			new_node->type = TDOUBLE;
			new_node->val.d = 0;

			return new_node;
			break;
		}
		default:
			std::cout << "Произошла ошибка" << std::endl;
			break;
	}
	return 0;
}

Block_Tree *Differentiator::DiffStandartFunc(const Block_Tree *node)
{
	Value val;
	
	Block_Tree *new_node = new Block_Tree;

	Insert(new_node, node->type, node->val);

	switch (node->val.c)
	{
		case '*': //(uv)' = u'v + uv'
		{
			std::cout << "We in the case \"*\"" << std::endl;

			Block_Tree *old_left = new Block_Tree;
			Block_Tree *old_right = new Block_Tree;

			Block_Tree *next_mul1 = new Block_Tree;
			Block_Tree *next_mul2 = new Block_Tree;

			Insert(old_left, node->left->type, node->left->val);
			Insert(old_right, node->right->type, node->right->val);

			new_node->val.c = '+'; //+

			val.c = '*';
			Insert(next_mul1, TCHAR, val); //*
			Insert(next_mul2, TCHAR, val); //*

			next_mul1->left = old_left;	  //*v
			next_mul2->right = old_right; //*u

			next_mul1->right = Derivative(node->right); // u'*v
			next_mul2->left = Derivative(node->left); // u*v'

			new_node->left = next_mul1;	 // u'*v+
			new_node->right = next_mul2; // u'*v+u*v'

			break;
		}

		case '/': //(u/v)' =(u'*v - u*v')/(v^2)
		{
			std::cout << "We in the case \"/\"" << std::endl;
			new_node->val.c = '/';

			Block_Tree *old_left = new Block_Tree;
			Block_Tree *old_right = new Block_Tree;

			Insert(old_left, node->left->type, node->left->val);
			Insert(old_right, node->right->type, node->right->val);

			Block_Tree *next_sub = new Block_Tree;
			Block_Tree *next_mul1 = new Block_Tree;
			Block_Tree *next_mul2 = new Block_Tree;
			Block_Tree *next_degree = new Block_Tree;
			Block_Tree *next_two = new Block_Tree;

			val.c = '-';
			Insert(next_sub, TCHAR, val); //-

			val.c = '*';
			Insert(next_mul1, TCHAR, val); //*
			Insert(next_mul2, TCHAR, val); //*

			next_sub->left = next_mul1;	 //*-
			next_sub->right = next_mul2; //*-*

			next_mul1->left = old_right; //*v-*
			next_mul2->right = old_left; //*v-u*

			val.c = '^';
			Insert(next_degree, TCHAR, val); //^

			val.d = 2;
			Insert(next_two, TDOUBLE, val); // 2

			next_degree->right = next_two; //^2

			next_degree->left = old_right; // v^2

			next_mul1->right = Derivative(node->left); // u'*v-u*
			next_mul1->left = Derivative(node->right); // u'*v-u*v'

			new_node->left = next_sub;	   //(u'*v-u*v')
			new_node->right = next_degree; //(u'*v-u*v')/(v^2)
			break;
		}

		case '+': //(u+v) = u' + v'
		{
			new_node->left = Derivative(node->left);
			new_node->right = Derivative(node->right);
			break;
		}

		case '-': //(u-v) = u' - v'
		{
			new_node->left = Derivative(node->left);
			new_node->right = Derivative(node->right);
			break;
		}

		case '^': //(u^const)' = const*u'*u^(const-1)
		{
			if (node->left->type != TDOUBLE)
			{
				std::cout << "We in the case \"^\"" << std::endl;
				new_node->val.c = '*';

				Block_Tree *next_mul = new Block_Tree;
				Block_Tree *next_const = new Block_Tree;
				Block_Tree *old_left = new Block_Tree;
				Block_Tree *next_degree = new Block_Tree;
				Block_Tree *old_degree = new Block_Tree;

				Insert(old_left, node->left->type, node->left->val);
				Insert(next_const, node->right->type, node->right->val);
				next_const->val.d--; // const-1

				val.c = '*';
				Insert(next_mul, TCHAR, val); //*
				val.c = '^';
				Insert(next_degree, TCHAR, val); //^

				next_degree->left = old_left;	 // u^
				next_degree->right = next_const; // u^(const-1)

				Insert(old_degree, node->right->type, node->right->val );

				next_mul->left = Derivative(node->left); // u'*
				next_mul->right = next_degree;			 // u'*u^(const-1)

				new_node->left = old_degree;
				new_node->right = next_mul; //(u^const)' = const*u'*u^(const-1)
			}
			break;
		}

		case 'x':
		{
			std::cout << "We in the case \"x\"" << std::endl;
			val.d = 1;
			Insert(new_node, TDOUBLE, val);
			break;
		}
		
		default:
		{
			std::cout << "Такой функции нет" << std::endl;
		}
	}
	return new_node;
}