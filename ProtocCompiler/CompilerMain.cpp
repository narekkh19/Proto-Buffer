#include "Parser.hpp"
#include "Generator.hpp"

int u{};

void dfs(AST* curr)
{
	++u;
	if (u != 1)
	{
		curr->print();
	}

	if (Defenition* def_ptr = dynamic_cast<Defenition*>(curr))
	{
		const auto& children = def_ptr->GetChildren();

		for (auto& child : children)
		{
			dfs(child.get());
		}
	}
}

int main(int argc, char* argv[])
{	

	Parser parser(argv[1]);

	auto ASTroot = parser.GetAbstractSyntaxTree();
	
	Generator gen("Person", ASTroot);
	gen.GenerateHeader();

	return 0;
}
 
