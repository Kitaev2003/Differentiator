#include "Differentiator.h"

void Differentiator::Reduction(Block_Tree *Head)
{
    bool repeat = true;

    while (repeat)
    {
        repeat = false;

        setNulAndFunc(Head, &repeat);
        setNumAndNum(Head, &repeat);
        setSpecFuncAndNum(Head, &repeat);
        setAddAndSub(Head, &repeat);
    }
}

void Differentiator::setAddAndSub(Block_Tree *node, bool *repeat)
{
    if (node->right != nullptr && node->type == TCHAR && node->right->type == TCHAR)
    {
        if (node->val.c == '+' && node->right->val.c == '-')
        {
            Block_Tree *address_delete = node->right;
            node->val = node->right->val;
            node->right = node->right->right;
            setFree (address_delete);
            *repeat = true;
        }

        else if (node->val.c == '-' && node->right->val.c == '+')
        {
            Block_Tree *address_delete = node->right;
            node->right = node->right->right;
            setFree(address_delete);
            *repeat = true;
        }
    }
    if (node->left != nullptr)
    {
        setAddAndSub(node->left, repeat);
    }
    if (node->right != nullptr)
    {
        setAddAndSub(node->right, repeat);
    }
}

void Differentiator::setNumAndNum(Block_Tree *node, bool *repeat)
{
    if (node->left != nullptr && node->right != nullptr && node->left->type == TDOUBLE && node->right->type == TDOUBLE)
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
            std::cout << "Ошибка свертки констант в \"setNumAndNum\"" << std::endl;
            exit(1);
        }
        *repeat = true;
        node->type = TDOUBLE;

        setFree(node->left);
        setFree(node->right);

        node->left = nullptr;
        node->right = nullptr;
    }
    if (node->left != nullptr)
    {
        setNumAndNum(node->left, repeat);
    }
    if (node->right != nullptr)
    {
        setNumAndNum(node->right, repeat);
    }
}

void Differentiator::setSpecFuncAndNum(Block_Tree *node, bool *repeat)
{
    const char *strdelete = node->val.cp;

    if (node->type == TPCHAR && node->right->type == TDOUBLE)
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
            std::cout << "Ошибка свертки констант в \"setSpecFuncAndNum\"" << std::endl;
            exit(1);
            break;
        }

        *repeat = true;
        node->type = TDOUBLE;

        delete [] strdelete;
        setFree(node->right);
        node->right = nullptr;
        node->left = nullptr;
    }

    if (node->left != nullptr)
    {
        setSpecFuncAndNum(node->left, repeat);
    }

    if (node->right != nullptr)
    {
        setSpecFuncAndNum(node->right, repeat);
    }
}

void Differentiator::setNulAndFunc(Block_Tree *node, bool *repeat)
{
    if (node->left != nullptr && node->right != nullptr && node->type == TCHAR && node->val.c != 'x')
    {
        if (node->left->type == TDOUBLE && (abs(node->left->val.d) < DOUBLEPTR))
        {
            Block_Tree *address_delete = node->right;
            switch (node->val.c)
            {
            case '*':
                setFree(node->right);
                setFree(node->left);
                node->left = nullptr;
                node->right = nullptr;

                node->type = TDOUBLE;
                node->val.d = 0;
                break;
            case '/':
                setFree(node->right);
                setFree(node->left);
                node->left = nullptr;
                node->right = nullptr;

                node->type = TDOUBLE;
                node->val.d = 0;
                break;
            case '+':
                setFree(node->left);
                node->left = nullptr;

                memcpy(node, node->right, SIZEBLOCK);
                delete (address_delete);
                break;
            case '-':
                setFree(node->left);
                node->left = nullptr;
                break;
            case '^':
                setFree(node->right);
                setFree(node->left);
                node->left = nullptr;
                node->right = nullptr;

                node->type = TDOUBLE;
                node->val.d = 0;
                break;
            default:
                std::cout << "Ошибка свертки констант в \"setNulAndFunc\":left" << std::endl;
                exit(1);
                break;
            }
            *repeat = true;
        }

        else if (node->right->type == TDOUBLE && (abs(node->right->val.d) < DOUBLEPTR))
        {
            Block_Tree *address_delete = node->left;

            switch (node->val.c)
            {
            case '*':
                setFree(node->right);
                setFree(node->left);
                node->left = nullptr;
                node->right = nullptr;

                node->type = TDOUBLE;
                node->val.d = 0;
                break;
            case '/':
                std::cout << "Вы делите на ноль" << std::endl;
                exit(1);
                break;
            case '+':
                setFree(node->right);
                node->right = nullptr;

                memcpy(node, node->left, SIZEBLOCK);
                setFree(address_delete);
                break;
            case '-':
                setFree(node->right);
                node->right = nullptr;

                memcpy(node, node->left, SIZEBLOCK);
                setFree(address_delete);
                break;
            case '^':
                setFree(node->right);
                setFree(node->left);
                node->left = nullptr;
                node->right = nullptr;

                node->type = TDOUBLE;
                node->val.d = 1;
                break;
            default:
                std::cout << "Ошибка свертки констант в \"setNulAndFunc\":right" << std::endl;
                exit(1);
                break;
            }
            *repeat = true;
        }
    }

    if (node->left != nullptr)
    {
        setNulAndFunc(node->left, repeat);
    }

    if (node->right != nullptr)
    {
        setNulAndFunc(node->right, repeat);
    }
}