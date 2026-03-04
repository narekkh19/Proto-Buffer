#include "../include/Generator.hpp"
#include <filesystem>
#include <iostream>

class GeneratorAdapter;
namespace fs = std::filesystem;

Generator::Generator(const std::string& _file_name, const std::shared_ptr<AST>& _root, const std::string& _path)
	: path{ _path }, file_name{ _file_name }, root{ _root }
{
	// 1. Hardcoded Source (Where the master file lives)
	fs::path source_path = R"(C:\Users\narekkh19\source\repos\ProtoBuffer\ProtocCompiler\include\ProtoRuntime.hpp)";

	// 2. Destination Path (The 'generated' folder)
	fs::path dest_folder(path);
	fs::path dest_file = dest_folder / "ProtoRuntime.hpp";

	try {
		// 3. Create the 'generated' directory if it doesn't exist
		if (!fs::exists(dest_folder)) {
			fs::create_directories(dest_folder);
		}

		// 4. Check if source exists before copying
		if (fs::exists(source_path)) {
			fs::copy_file(source_path, dest_file, fs::copy_options::overwrite_existing);
			// std::cout << "Successfully copied ProtoRuntime.h to " << dest_file << "\n";
		}
		else {
			std::cerr << "Error: Source ProtoRuntime.h not found at " << source_path << "\n";
		}

	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Filesystem Error: " << e.what() << "\n";
	}
}


void Generator::GenerateHeader()
{
	std::string absPath = path + "\\" + file_name + ".prt.hpp";
	std::ofstream file(absPath); // creates file if it doesnt exists

	NamespaceGen cc(root); // because the root is namespace
	file << cc.GetHeader();
}

void Generator::GenerateCpp()
{
	std::string absPath = path + "\\" + file_name + ".prt.cpp";
	std::ofstream file(absPath); // creates file if it doesnt exists

	NamespaceGen cc(root); // because the root is namespace
	file << cc.GetCpp();
}


std::string CodeGen::GetHeader()  // this method we can only call from root of AST
{
	auto* root = reinterpret_cast<Namespace*>(curr.get());
	std::string ans;
	ans += "#include \"ProtoRuntime.hpp\"\n"; // Add this line!
	ans += "#include <iostream>\n#include <string>\n#include <vector>\n\n";

	for (auto child : root->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		ans += ChildGen->GetDeclaration(""); // calling an adapter turning AST to CodeGen with right dynamic object and calling GetDeclartion
	}

	return ans;
}



std::string CodeGen::GetCpp()  // this method we can only call from root of AST
{
	auto* root = reinterpret_cast<Namespace*>(curr.get());
	std::string ans = "#include \"Person.prt.hpp\"\n\n";

	for (auto child : root->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		ans += ChildGen->GetMethodDefenition(""); // calling an adapter turning AST to CodeGen with right dynamic object and calling GetDeclartion
	}

	return ans;
}


std::shared_ptr<AST>& CodeGen::GetCurr() 
{
	return curr;
}

std::string NamespaceGen::GetDeclaration(std::string intent)  
{
	auto curr = std::dynamic_pointer_cast<Namespace>(GetCurr());

	std::string start = "namespace " + curr->GetName() + " {\n\n";
	std::string end = intent + "}\n\n";
	std::string content;

	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		content += ChildGen->GetDeclaration(intent);
	}

	return start + content + end;
}

std::string NamespaceGen::GetMethodDefenition(std::string class_name)
{
	auto curr = std::dynamic_pointer_cast<Namespace>(GetCurr());
	std::string ans;
	class_name += curr->GetName() + "::";

	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		if (dynamic_cast<ClassGen*>(ChildGen.get()))
		{
			ans += ChildGen->GetMethodDefenition(class_name);
		}
	}

	return ans;
}

std::string ClassGen::GetDeclaration(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Class>(GetCurr());
	std::string start = intent + "class " + curr->GetName() + " {\n\n";
	std::string end = intent + "};\n\n";
	std::string content;

	std::vector<std::shared_ptr<CodeGen>> NestedMembers;
	std::vector<std::shared_ptr<InstanceGen>> DataMembers;

	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		if (const auto& InstGen = std::dynamic_pointer_cast<InstanceGen>(ChildGen))
		{
			DataMembers.push_back(InstGen);
		}
		else
		{
			NestedMembers.push_back(ChildGen);
		}
	}
	
	if (!NestedMembers.empty())
		content += intent + "public:\n";

	for (auto& Nest : NestedMembers)
	{
		content += Nest->GetDeclaration(intent + "\t");
	}

	if (!DataMembers.empty())
		content += intent + "private:\n";

	for (auto& Data : DataMembers)
	{
		content += Data->GetDeclaration(intent + "\t");
	}

	if (!DataMembers.empty())
		content += intent + "public:\n";

	for (auto& Data : DataMembers)
	{
		content += Data->GetMethods(intent + "\t");
	}

	content += intent + "\t" + "std::vector<uint8_t> Serialize() const;\n";
	content += intent + "\t" + "void Deserialize(const std::vector<uint8_t>& _buffer);\n\n";

	return start + content + end;
}

std::string ClassGen::GetMethodDefenition(std::string class_name)
{
	auto curr = std::dynamic_pointer_cast<Class>(GetCurr());
	std::string ans;
	class_name += curr->GetName() + "::";

	std::string serDef = std::format("std::vector<uint8_t> {}Serialize() const {}{{{}{}std::vector<uint8_t> _buffer;{}{}", class_name, '\n', '\n', '\t', '\n', '\n');

	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		if (auto InstGen = dynamic_cast<InstanceGen*>(ChildGen.get()))
		{
			ans += ChildGen->GetMethodDefenition(class_name); 
			serDef += "\t";
			serDef += InstGen->GetSerialization();
			serDef += "\n";
		}
	}


	for (auto& child : curr->GetChildren())
	{
		auto ChildGen = GeneratorAdapter::Adapt(child);
		if (!dynamic_cast<InstanceGen*>(ChildGen.get()))
		{
			ans += ChildGen->GetMethodDefenition(class_name);
		}
	}								

	serDef += "\n\treturn _buffer;\n}\n\n";


	std::string deserDef = "void " + class_name + "Deserialize(const std::vector<uint8_t>& _buffer)\n{\n";
	deserDef += "\tsize_t _pos = 0;\n\n";
	deserDef += "\twhile (_pos < _buffer.size()) {\n\n";
	deserDef += "\t\tuint64_t _tag = ProtoRuntime::ReadVarint(_buffer, _pos);\n";
	deserDef += "\t\tint _fieldNum = _tag >> 3;\n";
	deserDef += "\t\tint _wireType = _tag & 0x07;\n";
	deserDef += "\t\tswitch (_fieldNum) {\n\n";

	for (auto& child : curr->GetChildren()) {

		if (auto InstGen = std::dynamic_pointer_cast<InstanceGen>(GeneratorAdapter::Adapt(child))) {
			
			auto inst_ast = std::dynamic_pointer_cast<Instance>(child);
			deserDef += std::format("\t\t\tcase {0}: ProtoRuntime::ReadField(_buffer, _pos, {1}); break;\n",
				inst_ast->GetFN(), inst_ast->GetName());
		}
	}

	deserDef += "\t\t\tdefault: ProtoRuntime::SkipField(_buffer, _pos, _wireType); break;\n";
	deserDef += "\t\t}\n\t}\n}\n\n";

	return ans + serDef + deserDef;
}

std::string EnumGen::GetDeclaration(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Enum>(GetCurr());

	std::string start = intent + "enum class " + curr->GetName() + " {\n\n";
	std::string end = intent + "};\n\n";
	std::string content;

	for (auto& child : curr->GetChildren())
	{
		content += (intent + "\t");
		content += child->GetName();
		content += ",\n";
	}

	return start + content + end;
}

std::string InstanceGen::GetSerialization()
{
	auto inst_curr = std::dynamic_pointer_cast<Instance>(GetCurr());


	return std::format("ProtoRuntime::WriteField(_buffer, {0}, {1});", inst_curr->GetFN(), inst_curr->GetName());
}

std::string EnumGen::GetMethodDefenition(std::string class_name) { return ""; }

std::string PrimitiveGen::GetDeclaration(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	return intent + curr->GetType() + " " + curr->GetName() + "{};\n\n";
}

std::string PrimitiveGen::GetMethods(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	std::string getter = "const " + curr->GetType() + "& get_" + curr->GetName() + "() const;\n";
	std::string setter = "void set_" + curr->GetName() + "(const " + curr->GetType() + "& i);\n";

	return (intent + getter + intent + setter);
}

std::string PrimitiveGen::GetMethodDefenition(std::string class_name)
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	std::string getter = "auto " + class_name + "get_" + curr->GetName() + "() const -> const " + curr->GetType() + " &\n";
	std::string getter_body = std::string("{\n\t") + "return " + curr->GetName() + ";\n}\n\n";

	std::string setter = "void " + class_name + "set_" + curr->GetName() + "(const " + curr->GetType() + "& val)\n";
	std::string setter_body = std::string("{\n\t") + curr->GetName() + " = val;\n}\n\n"; 

	return (getter + getter_body + setter + setter_body);
}


std::string ContainerGen::GetDeclaration(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Container>(GetCurr());

	return intent + "std::" + curr->GetType() + "<" + curr->GetInnerType() + "> " + curr->GetName() + "{};\n\n";
}

std::string ContainerGen::GetMethods(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Container>(GetCurr()); 

	std::string size = "size_t " + curr->GetName() + "_size() const;\n";
	std::string getter = "const " + curr->GetInnerType() + "&" + " get_" + curr->GetInnerType() + "(int index) const;\n";
	std::string adder = "void add_" + curr->GetName() + "(const " + curr->GetInnerType() +  "& val);\n";
	std::string clear = "void clear_" + curr->GetName() + "();\n";
	std::string cont = "const std::vector<" + curr->GetInnerType() + ">& " + "get_" + curr->GetName() + "() const;\n\n";

	return (intent + size + intent + getter + intent + adder + intent + clear + intent + cont);
}

std::string ContainerGen::GetMethodDefenition(std::string class_name)
{
	auto curr = std::dynamic_pointer_cast<Container>(GetCurr());

	std::string size = "size_t " + class_name + curr->GetName() + "_size() const\n";
	std::string size_body = std::string("{\n\t") + "return " + curr->GetName() + ".size();\n}\n\n";

	std::string getter = "auto " + class_name + "get_" + curr->GetInnerType() + "(int index) const -> const " + curr->GetInnerType() + "&\n";
	std::string getter_body = std::string("{\n\t") + "return " + curr->GetName() + "[index];\n}\n\n";

	std::string adder = "void " + class_name + "add_" + curr->GetName() + "(const " + curr->GetInnerType() + "& val)\n";
	std::string adder_body = std::string("{\n\t") + curr->GetName() + ".push_back(val);\n}\n\n";

	std::string clear = "void " + class_name + "clear_" + curr->GetName() + "()\n";
	std::string clear_body = std::string("{\n\t") + curr->GetName() + ".clear();\n}\n\n";

	std::string cont = "auto " + class_name  + "get_" + curr->GetName() + "() const -> const std::vector<" + curr->GetInnerType() + ">&\n";
	std::string cont_body = std::string("{\n\t") + "return " + curr->GetName() + ";\n}\n\n";

	return (size + size_body + getter + getter_body + adder + adder_body + cont + cont_body);
}



std::string StringGen::GetDeclaration(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	return intent + "std::" + curr->GetType() + " " + curr->GetName() + "{};\n\n";
}

std::string StringGen::GetMethods(std::string intent)
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	std::string getter = "const std::string&  get_" + curr->GetName() + "() const;\n";
	std::string setter = "void set_" + curr->GetName() + "(const std::string& str);\n";
	std::string clear = "void clear_" + curr->GetName() + "();\n";
	std::string c_str = "const char* " + curr->GetName() + "_c_str() const; \n\n";

	return (intent + getter + intent + setter + intent + clear + intent + c_str);
}


std::string StringGen::GetMethodDefenition(std::string class_name)
{
	auto curr = std::dynamic_pointer_cast<Instance>(GetCurr());

	std::string getter = "auto " + class_name + "get_" + curr->GetName() + "() const -> const std::string&\n";
	std::string getter_body = std::string("{\n\t") + "return " + curr->GetName() + ";\n}\n\n";

	std::string setter = "void " + class_name + "set_" + curr->GetName() + "(const std::string& str)\n";
	std::string setter_body = std::string("{\n\t") + curr->GetName() + " = str;\n}\n\n";

	std::string clear = "void " + class_name + "clear_" + curr->GetName() + "()\n";
	std::string clear_body = std::string("{\n\t") + curr->GetName() + ".clear();\n}\n\n";

	std::string c_str = "const char* " + class_name +  curr->GetName() + "_c_str() const\n";
	std::string c_str_body = std::string("{\n\t") + "return " + curr->GetName() + ".c_str();\n}\n\n";

	return (getter + getter_body + setter + setter_body + clear + clear_body + c_str + c_str_body);
}



std::shared_ptr<CodeGen> GeneratorAdapter::Adapt(const std::shared_ptr<AST>& curr)
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