#include "Generator.hpp"

void Generator::GenerateHeader()
{
	std::string absPath = path + "\\" + file_name + ".prt.hpp";
	std::ofstream file(absPath); // creates file if it doesnt exists

	NamespaceGen cc(root); // because the root is namespace
	file << cc.GetHeader();
}

std::string CodeGen::GetHeader()  // this method we can only call from root of AST
{
	auto* root = reinterpret_cast<Namespace*>(curr.get());
	std::string ans;

	for (auto child : root->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		ans += ChildGen->GetDeclaration(); // calling an adapter turning AST to CodeGen with right dynamic object and calling GetDeclartion
	}

	return ans;
}

std::shared_ptr<AST>& CodeGen::GetCurr() 
{
	return curr;
}

std::string NamespaceGen::GetDeclaration()  
{
	auto curr = std::dynamic_pointer_cast<Namespace>(GetCurr());

	std::string start = "namespace " + curr->GetName() + " {\n";
	std::string end = "}\n";
	std::string content;

	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(GetCurr());
		content += ChildGen->GetDeclaration();
	}

	return start + content + end;
}

std::string ClassGen::GetDeclaration()  
{
	auto curr = std::dynamic_pointer_cast<Class>(GetCurr());
	std::string start = "class " + curr->GetName() + " {\n";
	std::string end = "}\n";
	std::string content;

	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(GetCurr());
		content += ChildGen->GetDeclaration();
	}


	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(GetCurr());
		if (const auto& InstGen = std::dynamic_pointer_cast<InstanceGen>(ChildGen))
		{
			content += InstGen->GetMethods();
		}
	}


	return start + content + end;
}


std::string EnumGen::GetDeclaration()  
{
	auto curr = std::dynamic_pointer_cast<Enum>(GetCurr());

	std::string start = "enum class " + curr->GetName() + " {\n";
	std::string end = "}\n";
	std::string content;

	for (auto& child : curr->GetChildren())
	{
		content += child->GetName();
		content += ",\n";
	}

	return start + content + end;
}


std::string PrimitiveGen::GetDeclaration()  
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	return curr->GetType() + " " + curr->GetName() + "{}\n";
}

std::string PrimitiveGen::GetMethods() 
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	std::string getter = curr->GetType() + " get_" + curr->GetName() + "() const;\n";
	std::string setter = "void set_" + curr->GetName() + "(" + curr->GetType() + " i);\n";
	std::string clear = "void clear_" + curr->GetName() + "();\n";

	return (getter + setter + clear);
}

std::string ContainerGen::GetDeclaration() 
{
	auto curr = std::dynamic_pointer_cast<Container>(GetCurr());

	return "std::" + curr->GetType() + "<" + curr->GetInnerType() + "> " + curr->GetName() + "{}\n";
}

std::string ContainerGen::GetMethods()  
{
	auto curr = std::dynamic_pointer_cast<Container>(GetCurr()); 

	std::string size = "size_t " + curr->GetName() + "_size() const;\n";
	std::string getter = "const " + curr->GetType() + "&" + " get_" + curr->GetName() + "(int index) const;\n";
	std::string adder = "void add_" + curr->GetName() + "();\n";
	std::string clear = "void clear_" + curr->GetName() + "();\n";
	std::string cont = "const std::vector<" + curr->GetInnerType() + ">& " + curr->GetName() + "() const\n";

	return (size + getter + adder + clear + cont);
}

std::string StringGen::GetDeclaration()  
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	return "std::" + curr->GetType() + " " + curr->GetName() + "{}\n";
}

std::string StringGen::GetMethods()  
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	std::string getter = "const std::string&  get_" + curr->GetName() + "() const;\n";
	std::string setter = "void set_" + curr->GetName() + "(const std::string& " + "str);\n";
	std::string clear = "void clear_" + curr->GetName() + "();\n";
	std::string c_str = "const char* c_str() const\n";

	return (getter + setter + clear + c_str);
}

std::shared_ptr<CodeGen> GeneratorAdapter::Adapt(std::shared_ptr<AST>& curr)
{
	auto* curr_ptr = curr.get();
	if (auto* real_ptr = dynamic_cast<Namespace*>(curr_ptr)) 
	{
		return std::make_shared<NamespaceGen>(curr);
	}
	else if (auto* real_ptr = dynamic_cast<Class*>(curr_ptr))
	{
		return std::make_shared<ClassGen>(curr);
	}
	else if (auto* real_ptr = dynamic_cast<Enum*>(curr_ptr))
	{
		return std::make_shared<EnumGen>(curr);
	}
	else if (auto* real_ptr = dynamic_cast<Container*>(curr_ptr))
	{
		return std::make_shared<ContainerGen>(curr);
	}
	else
	{
		if (curr->GetType() == "string") return std::make_shared<StringGen>(curr);
	}

	return std::make_shared<PrimitiveGen>(curr);
}