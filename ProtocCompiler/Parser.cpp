#include "Parser.hpp"
#include <stack>

std::unique_ptr<AST> Parser::GetAbstractSyntaxTree() 
{
	std::vector<std::unique_ptr<AST>> ast;
	std::ifstream file(file_name);
	std::string line;

	std::stack<Defenition*> st;
	auto root = std::make_unique<Namespace>();
	st.push(root.get()); // syntax tree root node 
	bool isEnumValue{};

	while (std::getline(file, line))
	{

		std::stringstream ss(line);
		std::vector<std::string> words;
		std::string word;

		while (ss >> word) words.push_back(std::move(word));
		
		if (!isEnumValue && !SyntaxAnalize(words)) continue;
		
		if (words.size() == 1 && words.front() == "}")
		{
			st.pop(); 
			isEnumValue = false;
			continue;
		}

		
		std::unique_ptr<AST> curr_ast = ASTFactory().createAST(words, isEnumValue);
		

		auto* new_top = curr_ast.get();
		st.top()->AddNested(std::move(curr_ast));

		if (auto* def_ptr = dynamic_cast<Defenition*>(new_top)) // if the dynamic type is Defenition
		{
			st.push(def_ptr);	
			if (!dynamic_cast<Namespace*>(def_ptr))
			{
				AddType(def_ptr->GetName()); //if it is class or enum add its name to types
				if (dynamic_cast<Enum*>(def_ptr)) isEnumValue = true;
			}
		}
	}

	while (st.size() != 1) st.pop(); // poping namespaces from stack because they dont have } end mark

	return root;
}


bool Parser::SyntaxAnalize(const std::vector<std::string>& words) const
{
	if (words.empty()) return false;

	std::string first_word = words.front();

	if (defenitions.contains(first_word)) return true;
	if (types.contains(first_word)) return true;

	return (first_word == "}");
}