namespace tutorial {

class Person {

	std::string name{};

	int id{};

	std::string email{};

	enum class PhoneType {

		PHONE_TYPE_UNSPECIFIED,
		PHONE_TYPE_MOBILE,
		PHONE_TYPE_HOME,
		PHONE_TYPE_WORK,
	}

	class PhoneNumber {

		std::string number{};

		PhoneType type{};

		const std::string&  get_number() const;
		void set_number(const std::string& str);
		void clear_number();
		const char* c_str() const;

		PhoneType get_type() const;
		void set_type(PhoneType i);
		void clear_type();

	}

	std::vector<PhoneNumber> phones{};

	const std::string&  get_name() const;
	void set_name(const std::string& str);
	void clear_name();
	const char* c_str() const;

	int get_id() const;
	void set_id(int i);
	void clear_id();

	const std::string&  get_email() const;
	void set_email(const std::string& str);
	void clear_email();
	const char* c_str() const;

	size_t phones_size() const;
	const vector& get_phones(int index) const;
	void add_phones();
	void clear_phones();
	const std::vector<PhoneNumber>& phones() const;

}

class AddressBook {

	std::vector<Person> people{};

	size_t people_size() const;
	const vector& get_people(int index) const;
	void add_people();
	void clear_people();
	const std::vector<Person>& people() const;

}

}

