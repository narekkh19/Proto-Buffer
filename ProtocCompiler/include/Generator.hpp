#pragma once
#include "Parser.hpp"
#include <format>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

class Generator
{
	std::string path;
	std::string file_name;
	std::shared_ptr<AST> root;
public:


	Generator(const std::string& _file_name, const std::shared_ptr<AST>& _root, const std::string& _path = R"(C:\Users\narekkh19\source\repos\ProtoBuffer\ProtocCompiler\generated)");
	void GenerateHeader();
	void GenerateCpp();
};

class CodeGen {
	std::shared_ptr<AST> curr;

public:
	CodeGen(const std::shared_ptr<AST>& _curr) : curr{_curr} {}

	std::shared_ptr<AST>& GetCurr();
	virtual std::string GetDeclaration(std::string intent) = 0;
	virtual std::string GetMethodDefenition(std::string class_name) = 0;
	std::string GetCpp();
	std::string GetHeader();
};

class InstanceGen : public CodeGen
{
public:
	InstanceGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	virtual std::string GetMethods(std::string intent) = 0;
	std::string GetSerialization();
};


class NamespaceGen : public CodeGen
{
public:
	NamespaceGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	std::string GetDeclaration(std::string intent) override;
	std::string GetMethodDefenition(std::string class_name) override;
};


class ClassGen : public CodeGen
{
public:
	ClassGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	std::string GetDeclaration(std::string intent) override;
	std::string GetMethodDefenition(std::string class_name) override;
};

class EnumGen : public CodeGen
{
public:
	EnumGen(const std::shared_ptr<AST>& ptr) : CodeGen(ptr) {}
	std::string GetDeclaration(std::string intent) override;
	std::string GetMethodDefenition(std::string class_name) override;
};

class PrimitiveGen : public InstanceGen
{
public:
	PrimitiveGen(const std::shared_ptr<AST>& ptr) : InstanceGen(ptr) {}
	std::string GetDeclaration(std::string intent) override;
	std::string GetMethods(std::string intent) override;
	std::string GetMethodDefenition(std::string class_name) override;
	std::string SerializationForInt(int field_number);
	std::string SerializationForFloat(int field_number);
	std::string SerializationForDouble(int field_number);
};

class ContainerGen : public InstanceGen
{
public:
	ContainerGen(const std::shared_ptr<AST>& ptr) : InstanceGen(ptr) {}
	std::string GetDeclaration(std::string intent) override;
	std::string GetMethods(std::string intent) override;
	std::string GetMethodDefenition(std::string class_name) override;
	std::string SerializationForVector(int field_number);
};


class StringGen : public InstanceGen
{
public:
	StringGen(const std::shared_ptr<AST>& ptr) : InstanceGen(ptr) {}
	std::string GetDeclaration(std::string intent) override;
	std::string GetMethods(std::string intent) override;
	std::string GetMethodDefenition(std::string class_name) override;
	std::string SerializationForString(int field_number);
};


class GeneratorAdapter
{
public:
	static std::shared_ptr<CodeGen> Adapt(const std::shared_ptr<AST>& curr);
};