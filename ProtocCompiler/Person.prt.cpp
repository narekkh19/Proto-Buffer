#include "Person.prt.hpp"

auto tutorial::Person::get_name() const -> const std::string&
{
	return name;
}

void tutorial::Person::set_name(const std::string& str)
{
	name = str;
}

void tutorial::Person::clear_name()
{
	name.clear();
}

const char* tutorial::Person::name_c_str() const
{
	return name.c_str();
}

auto tutorial::Person::get_id() const -> const int &
{
	return id;
}

void tutorial::Person::set_id(const int& val)
{
	id = val;
}

auto tutorial::Person::get_email() const -> const std::string&
{
	return email;
}

void tutorial::Person::set_email(const std::string& str)
{
	email = str;
}

void tutorial::Person::clear_email()
{
	email.clear();
}

const char* tutorial::Person::email_c_str() const
{
	return email.c_str();
}

size_t tutorial::Person::phones_size() const
{
	return phones.size();
}

auto tutorial::Person::get_PhoneNumber(int index) const -> const PhoneNumber&
{
	return phones[index];
}

void tutorial::Person::add_phones(const PhoneNumber& val)
{
	phones.push_back(val);
}

auto tutorial::Person::get_phones() const -> const std::vector<PhoneNumber>&
{
	return phones;
}

auto tutorial::Person::PhoneNumber::get_number() const -> const std::string&
{
	return number;
}

void tutorial::Person::PhoneNumber::set_number(const std::string& str)
{
	number = str;
}

void tutorial::Person::PhoneNumber::clear_number()
{
	number.clear();
}

const char* tutorial::Person::PhoneNumber::number_c_str() const
{
	return number.c_str();
}

auto tutorial::Person::PhoneNumber::get_type() const -> const PhoneType &
{
	return type;
}

void tutorial::Person::PhoneNumber::set_type(const PhoneType& val)
{
	type = val;
}

size_t tutorial::AddressBook::people_size() const
{
	return people.size();
}

auto tutorial::AddressBook::get_Person(int index) const -> const Person&
{
	return people[index];
}

void tutorial::AddressBook::add_people(const Person& val)
{
	people.push_back(val);
}

auto tutorial::AddressBook::get_people() const -> const std::vector<Person>&
{
	return people;
}

