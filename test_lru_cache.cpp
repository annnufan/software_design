#include "lru_cache.h"
#include <string>
#include <iostream>

void test_zero_size_lru_cache() {
	lru_cache<std::string, int> current_cache([](std::string key)->int {
		static int version = 0;
		return key.length() + version++;
	}, 1);
}

void test_adding_elements() {
	lru_cache<std::string, int> current_cache([](std::string key)->int {
		static int version = 0;
		return key.length() + version++;
	}, 1);
	assert(current_cache.get_value("hello") == 5);
	assert(current_cache.get_value("Anna") == 5);
}

void test_deleting_oldest_value() {
	lru_cache<std::string, int> current_cache([](std::string key)->int {
		static int version = 0;
		return key.length() + version++;
	}, 2);
	assert(current_cache.get_value("hello") == 5);
	assert(current_cache.get_value("Anna") == 5);
	assert(current_cache.get_value("hello") == 5);
	assert(current_cache.get_value("ugc") == 5);
	assert(current_cache.get_value("hello") == 5);
}

void test_deleting_from_cache() {
	lru_cache<std::string, int> current_cache([](std::string key)->int {
		static int version = 0;
		return key.length() + version++;
	}, 2);
	assert(current_cache.get_value("hello") == 5);
	assert(current_cache.get_value("Anna") == 5);
	assert(current_cache.get_value("hello") == 5);
	assert(current_cache.get_value("ugc") == 5);
	assert(current_cache.get_value("hello") == 5);
	assert(current_cache.get_value("Anna") == 7);
}


int main() {
	test_zero_size_lru_cache();
	test_adding_elements();
	test_deleting_oldest_value();
	test_deleting_from_cache();

	std::cout << "All tests successes\n";
}