#include "Differentiator.h"

void Differentiator::EquactionRead()
{
	char *buffer = new char[100];
	size_t i = 0;

	std::ifstream READ;

	READ.open("Equaction.txt");
	READ.getline(buffer, 100, '\n');
	READ.close();

	HeadRead_ = setReadAddAndSub(buffer, &i);

	size_ = i++;

	std::cout << buffer << std::endl;

	delete [] buffer;
}

Block_Tree *Differentiator::setReadAddAndSub(char *buffer, size_t *i)
{
	Block_Tree* left;
	Value val;

	left = setReadMulAndDiv(buffer, i);

	while (true)
	{
		if (buffer[*i] == '+' || buffer[*i] == '-')
		{
			Block_Tree *node = new Block_Tree;

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

Block_Tree *Differentiator::setReadMulAndDiv(char *buffer, size_t *i)
{
	Block_Tree* left;
	Value val;

	left = setReadDegree(buffer, i);

	while (true)
	{
		if (buffer[*i] == '*' || buffer[*i] == '/')
		{
			Block_Tree *node = new Block_Tree;

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

Block_Tree *Differentiator::setReadDegree(char *buffer, size_t *i)
{
	Block_Tree *left;
	Value val;

	left = setReadSpecFunc(buffer, i);

	while (true)
	{
		if (buffer[*i] == '^')
		{
			Block_Tree *node = new Block_Tree;

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

Block_Tree *Differentiator::setReadSpecFunc(char *buffer, size_t *i)
{
	Block_Tree *left;
	Value val;

	size_t k = 0;
	size_t n = *i;

	left = setReadParenthes(buffer, i);

	while (true)
	{
		if ('a' <= buffer[n] && 'z' >= buffer[n] && buffer[n] != 'x' && buffer[n] != 'e')
		{
			char *func = new char[5];

			while ('a' <= buffer[*i] && 'z' >= buffer[*i] && buffer[n] != 'x' && buffer[n] != 'e')
			{
				func[k] = buffer[*i];

				k++;
				*i = *i + 1;
			}
			func[k] = '\0';

			Block_Tree *node = new Block_Tree;

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

Block_Tree *Differentiator::setReadParenthes(char *buffer, size_t *i)
{
	if (buffer[*i] == '(' || buffer[*i] == ')')
	{
		while (true)
		{
			*i = *i + 1;
			Block *node = setReadAddAndSub(buffer, i);
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

Block_Tree *Differentiator::setReadValAndNum(char *buffer, size_t *i)
{
	Block_Tree *node = new Block_Tree;
	int k = -1;
	size_t n = *i;
	Value val;

	if (buffer[*i] == 'x') //Для переменной
	{
		*i = *i + 1;
		val.c = 'x';

		Insert(node, TCHAR, val);
		return node;
	}

	if (buffer[*i] == 'e') //Для переменной
	{
		*i = *i + 1;
		val.d = exp(1);

		Insert(node, TDOUBLE, val);
		return node;
	}

	while ('0' <= buffer[n] && '9' >= buffer[n]) // Считаем порядок числа
	{
		k++;
		n++;
	}

	val.d = 0;

	while (k >= 0) // Считываем число
	{
		val.d = val.d + (buffer[*i] - '0') * pow(10, k);
		*i = *i + 1;
		k--;

		if (buffer[*i] == '.') //Если встретили точку, то начинаем считывать дробную часть
		{
			*i = *i + 1;								   // Cдвигаем указатель с точки
			while ('0' <= buffer[*i] && '9' >= buffer[*i]) //Считываем дробную часть числа
			{
				val.d = val.d + (buffer[*i] - '0') * pow(10, k);
				*i = *i + 1;
				k--;
			}
		}
	}

	if ('0' <= buffer[n - 1] && '9' >= buffer[n - 1])
	{
		Insert(node, TDOUBLE, val);
	}

	return node;
}