#include <gtest/gtest.h>
#include "Parser.hpp"

TEST(TestingParser, ContainsKeyword) {
	Parser ptest;
	std::vector<std::string> words{ "class", "Parser", "{" };

	EXPECT_TRUE(ptest.SyntaxAnalize(words));
	words.clear();
	
	ASSERT_FALSE(ptest.SyntaxAnalize(words));
}