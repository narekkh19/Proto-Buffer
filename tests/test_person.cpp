#include "gtest/gtest.h"

#include "Person.prt.hpp"

using namespace tutorial;

TEST(PersonSerialization, BasicFieldsRoundtrip) {
    Person p;
    p.set_name("Alice");
    p.set_id(123);
    p.set_email("alice@example.com");

    Person::PhoneNumber phone;
    phone.set_number("555-1234");
    phone.set_type(Person::PhoneType::PHONE_TYPE_MOBILE);
    p.add_phones(phone);

    // Serialize and deserialize
    auto buf = p.Serialize();
    Person p2;
    p2.Deserialize(buf);

    EXPECT_EQ(p.get_name(), p2.get_name());
    EXPECT_EQ(p.get_id(), p2.get_id());
    EXPECT_EQ(p.get_email(), p2.get_email());
    ASSERT_EQ(p.phones_size(), p2.phones_size());
    EXPECT_EQ(p.get_PhoneNumber(0).get_number(), p2.get_PhoneNumber(0).get_number());
    EXPECT_EQ(static_cast<int>(p.get_PhoneNumber(0).get_type()), static_cast<int>(p2.get_PhoneNumber(0).get_type()));
}

TEST(AddressBookSerialization, MultiplePeople) {
    AddressBook book;

    Person p1;
    p1.set_name("Bob");
    p1.set_id(1);

    Person p2;
    p2.set_name("Carol");
    p2.set_id(2);

    book.add_people(p1);
    book.add_people(p2);

    auto buf = book.Serialize();
    AddressBook book2;
    book2.Deserialize(buf);

    ASSERT_EQ(book.people_size(), book2.people_size());
    EXPECT_EQ(book.get_Person(0).get_name(), book2.get_Person(0).get_name());
    EXPECT_EQ(book.get_Person(1).get_name(), book2.get_Person(1).get_name());
}

// Entry point provided by gtest_main
