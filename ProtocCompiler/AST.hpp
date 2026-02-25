#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

class AST
{
	std::string name;
public:
	std::string GetName() const { return name; }
	void SetName(const std::string& _name) { name = _name; }

	virtual std::string GetType() const = 0;
	virtual void print() const = 0;
	virtual ~AST() = default;
};


class Defenition : public AST
{
	std::vector<std::unique_ptr<AST>> next;

public:
	void AddNested(std::unique_ptr<AST>&& u)
	{
		next.push_back(std::move(u));
	}

	const std::vector<std::unique_ptr<AST>>& GetChildren()  
	{
		return next;
	}
};


class Namespace : public Defenition
{
public:
	Namespace() = default;
	Namespace(const std::vector<std::string>& words)
	{
		if (words.size() != 2) throw std::logic_error("Invalid schema format for namespace.");
		std::string NamespaceName =	words.back();

		NamespaceName.pop_back(); // poping back the last ; character 
		SetName(NamespaceName);
	}

	std::string GetType() const override
	{
		return "namespace";
	}

	void print() const override
	{
		std::cout << "Namespace: " << GetName() << std::endl;
	}
};


class Class : public Defenition
{
public:
	Class() = default;
	Class(const std::vector<std::string>& words)
	{
		if (words.size() != 3) throw std::logic_error("Invalid schema format for class.");

		SetName(words[1]);
	}

	std::string GetType() const override
	{
		return "class";
	}

	void print() const override
	{
		std::cout << "Class: " << GetName() << std::endl;
	}
};


class Enum : public Defenition
{
public:
	Enum() = default;
	Enum(const std::vector<std::string>& words)
	{
		if (words.size() != 3) throw std::logic_error("Invalid schema format for enum.");

		SetName(words[1]);
	}

	std::string GetType() const override
	{
		return "enum";
	}

	void print() const override
	{
		std::cout << "Enum: " << GetName() << std::endl;
	}
};


class Instance : public AST
{
	std::string type;
	int id;

public: 
	std::string GetType() const { return type; }
	void SetType(const std::string& _type) { type = _type; }

	int GetId() const { return id; }
	void SetId(int _id) { id = _id; }
	
	Instance() = default;

	Instance(const std::vector<std::string>& words)
	{
		if (words.size() != 4) throw std::logic_error("Invalid schema format for instance.");
		
		SetType(words[0]);
		SetName(words[1]);

		std::string ID = words.back();
		ID.pop_back();//poping back the last ; (ex. int id = 1;)

		int id = std::atoi(ID.c_str());
		SetId(id);
	}

	void print() const override
	{
		std::cout << GetType() << ": " << GetName() << " === " << GetId() << std::endl;
	}
};

class EnumValue : public AST
{
	int id;
public:
	virtual std::string GetType() const { return "EnumValues"; }

	int GetId() const { return id; }
	void SetId(int _id) { id = _id; }

	EnumValue(const std::vector<std::string>& words)
	{
		SetName(words[0]);
		
		std::string ID = words.back();
		ID.pop_back();//poping back the last ; (ex. Phone_Type_Mobile = 1;)

		int id = std::atoi(ID.c_str());
		SetId(id);
	}

	void print() const override
	{
		std::cout << "EnumValue: " << GetName()  << " === " << GetId() << std::endl;
	}
};

class Container : public Instance
{
	std::string inner_type;
public:
	std::string GetInnerType() const { return inner_type; }
	void SetInnerType(const std::string& _inner_type) { inner_type = _inner_type; }

	Container(const std::vector<std::string>& words)
	{
		if (words.size() != 5) throw std::logic_error("Invalid schema format for container.");

		SetType(words[0]);
		SetInnerType(words[1]);
		SetName(words[2]);

		std::string ID = words.back();
		ID.pop_back(); //poping back the last ; (ex. vector PhoneNumber phones = 1;)

		int id = std::atoi(ID.c_str());
		SetId(id);
	}

	void print() const override
	{
		std::cout << "Vector: " << GetInnerType() << " " << GetName() << " === " << GetId() << std::endl;
	}
};


class ASTFactory
{
public:
	std::unique_ptr<AST> createAST(const std::vector<std::string>& words, bool isEnumValue)
	{
		if (isEnumValue) return std::make_unique<EnumValue>(words);

		std::string first_word = words.front();
		
		if (first_word == "namespace") return std::make_unique<Namespace>(words);
		if (first_word == "class") return std::make_unique<Class>(words);
		if (first_word == "enum") return std::make_unique<Enum>(words);
		if (first_word == "vector") return std::make_unique<Container>(words);

		return std::make_unique<Instance>(words);
	}
};