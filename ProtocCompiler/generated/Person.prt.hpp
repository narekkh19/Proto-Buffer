#include "ProtoRuntime.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace tutorial {

class Person {

public:
	enum class PhoneType {

		PHONE_TYPE_UNSPECIFIED,
		PHONE_TYPE_MOBILE,
		PHONE_TYPE_HOME,
		PHONE_TYPE_WORK,
	};

	class PhoneNumber {

	private:
		std::string number{};

		PhoneType type{};

	public:
		const std::string&  get_number() const;
		void set_number(const std::string& str);
		void clear_number();
		const char* number_c_str() const; 

		const PhoneType& get_type() const;
		void set_type(const PhoneType& i);
		std::vector<uint8_t> Serialize() const;
		void Deserialize(const std::vector<uint8_t>& _buffer);

	};

private:
	std::string name{};

	int id{};

	std::string email{};

	std::vector<PhoneNumber> phones{};

public:
	const std::string&  get_name() const;
	void set_name(const std::string& str);
	void clear_name();
	const char* name_c_str() const; 

	const int& get_id() const;
	void set_id(const int& i);
	const std::string&  get_email() const;
	void set_email(const std::string& str);
	void clear_email();
	const char* email_c_str() const; 

	size_t phones_size() const;
	const PhoneNumber& get_PhoneNumber(int index) const;
	void add_phones(const PhoneNumber& val);
	void clear_phones();
	const std::vector<PhoneNumber>& get_phones() const;

	std::vector<uint8_t> Serialize() const;
	void Deserialize(const std::vector<uint8_t>& _buffer);

};

class AddressBook {

private:
	std::vector<Person> people{};

public:
	size_t people_size() const;
	const Person& get_Person(int index) const;
	void add_people(const Person& val);
	void clear_people();
	const std::vector<Person>& get_people() const;

	std::vector<uint8_t> Serialize() const;
	void Deserialize(const std::vector<uint8_t>& _buffer);

};

}

