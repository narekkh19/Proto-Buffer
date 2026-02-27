#pragma once
#include "Parser.hpp"

class Generator
{
	std::string path;
	std::string file_name;
	std::shared_ptr<AST> root;
public:
	Generator(const std::string& _file_name,
			  const std::shared_ptr<AST>& _root,
			  const std::string& _path = R"(C:\Users\narekkh19\source\repos\ProtoBuffer\ProtocCompiler)")
			  : path{ _path }, file_name{ _file_name }, root{ _root } {}
	
	void GenerateHeader();
};

class CodeGen {
	std::shared_ptr<AST> curr;

public:
	CodeGen(const std::shared_ptr<AST>& _curr) : curr{_curr} {}

	std::shared_ptr<AST>& GetCurr();
	virtual std::string GetDeclaration() = 0;

	std::string GetHeader();
};

class InstanceGen : public CodeGen
{
public:
	InstanceGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	virtual std::string GetMethods() = 0;
};


class NamespaceGen : public CodeGen
{
public:
	NamespaceGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	std::string GetDeclaration() override;
};


class ClassGen : public CodeGen
{
public:
	ClassGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	std::string GetDeclaration() override;
	
};

class EnumGen : public CodeGen
{
public:
	EnumGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	std::string GetDeclaration() override;
};

class PrimitiveGen : public InstanceGen
{
public:
	PrimitiveGen(const std::shared_ptr<AST>& ptr) : InstanceGen(ptr) {}
	std::string GetDeclaration() override;
	std::string GetMethods() override;
};

class ContainerGen : public InstanceGen
{
public:
	ContainerGen(const std::shared_ptr<AST>& ptr) : InstanceGen(ptr) {}
	std::string GetDeclaration() override;
	std::string GetMethods() override;

};


class StringGen : public InstanceGen
{
public:
	StringGen(const std::shared_ptr<AST>& ptr) : InstanceGen(ptr) {}
	std::string GetDeclaration() override;
	std::string GetMethods() override;
};


class GeneratorAdapter
{
public:
	static std::shared_ptr<CodeGen> Adapt(std::shared_ptr<AST>& curr);
};