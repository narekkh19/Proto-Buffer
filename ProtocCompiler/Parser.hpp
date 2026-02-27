#pragma once
#include "AST.hpp"
#include <sstream>
#include <fstream>
#include <unordered_set>

class Parser
{
	std::string file_name;

public: 
	Parser() = default;
	Parser(std::string _file_name) : file_name{ _file_name } {}

	std::shared_ptr<AST> GetAbstractSyntaxTree();
	bool SyntaxAnalize(const std::vector<std::string>& words) const;

	inline static std::unordered_set<std::string> defenitions{ "namespace", "class", "enum" };
	inline static std::unordered_set<std::string> types
	{ 
		"bool", "short", "int", "long", 
		"float", "double", 
		"string", "vector"
	};

	void AddType(const std::string& type) { types.insert(type); }
};

