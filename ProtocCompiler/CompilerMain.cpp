#include "Parser.hpp"

int main(int argc, char* argv[])
{
	Parser parser(argv[1]);
	auto x = parser.GetAbstractSyntaxTree();
}