#pragma once

#include <string>
#include <unordered_map>

using symbol = const std::string;

struct symbol_table {
public:
	// inserts a new symbol into the symbol table. 
	symbol* insert(const std::string&);
	// returns symbol correspodning to the string passed in. 
	symbol* find(const std::string&);

private:
	std::unordered_map<std::string, void*> syms;
};

symbol_table::insert(const std::string& str) {
	auto result = syms.insert({str, nullptr});
	return &result.first->first;
}

symbol_table::find(const std::string& str) {
	auto iter = syms.find(str);
	return &iter->first;
}