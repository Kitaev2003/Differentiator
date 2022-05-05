#include "Differentiator.h"

int main()
{
	std::cout << "----------Start work Differentiator---------" << std::endl;
	Differentiator play;
	play.EquactionRead();
	play.Reduction();
	DumpEquatiton(play.Head_->left);
	//play.Derivative(play.Head_->left);
	play.getDump();
	return 0;
}