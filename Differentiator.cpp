#include "Differentiator.h"

int main()
{
	std::cout << "----------Start work Differentiator---------" << std::endl;

	Differentiator play;

	play.EquactionRead();

	play.Reduction(play.HeadRead_);

	play.HeadWrite_ = play.Derivative(play.HeadRead_);

	play.Reduction(play.HeadWrite_);

	DumpEquatiton(play.HeadRead_);

	getDump(play.HeadWrite_);

	std::cout << "\n----------End work Differentiator---------" << std::endl;
	
	return 0;
}