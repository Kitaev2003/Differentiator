#include "Differentiator.h"

int main()
{
	std::cout << "----------Start work Differentiator---------" << std::endl;

	Differentiator play;

	std::cout << "----------Start work EquactionRead----------" << std::endl;

	play.EquactionRead(BufferFileRead());

	getDump(play.HeadRead_);
	DumpEquatiton(play.HeadRead_);

	play.Reduction(play.HeadRead_);

	std::cout << "----------End work EquactionRead------------" << std::endl;
	std::cout << "----------Start work Derivative-------------" << std::endl;
	play.HeadWrite_ = play.Derivative(play.HeadRead_);

	std::cout << "----------End work Derivative---------------" << std::endl;
	getDump(play.HeadWrite_);
	
	play.Reduction(play.HeadWrite_);

	getDump(play.HeadWrite_);
	std::cout << "END EQUACTION:";
	DumpEquatiton(play.HeadWrite_);

	std::cout << "\n----------End work Differentiator-----------" << std::endl;
	
	return 0;
}