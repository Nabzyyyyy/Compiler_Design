#pragma once

#include "type.hpp"
#include "token.hpp"
#include "decl.hpp"

#include <unordered_map>

struct Context {

	const Bool_Type Bool_; // type bool
	const Int_Type Int_; // type int
	//char outputForm; // output format for the integers
	std::unordered_map<std::string, Decl*> symbol_table; // table of symbols

	// context constructor
	//Context(char outputFormat) : outputForm(outputFormat) {} 
	Context() {}
	Token *check_kw(const std::string);	// check keyword
	Token *insert_sym(Decl* d);		// insert symbol
	Decl *find_sym(const std::string);		// find symbol
	void update_sym(const std::string, Decl*);		// update symbol func
};

Token* Context::insert_sym(Decl* d) {
	const std::string name = d->getName();
	auto it = symbol_table.find(name);
	if(it == symbol_table.end())
		symbol_table.insert({name, d}); //add when not already exists
	return new Id_token(name);
}

Token *Context::check_kw(const std::string s) {
	Keyword_Table keywords;

	auto it = keywords.find(s);

	if(it != keywords.end()) {
		Token* t = new Id_token(s, it->second);
		return t;
	}

	return nullptr;
}

Decl* Context::find_sym(const std::string str) {
	auto it = symbol_table.find(str);
	
	if(it != symbol_table.end())
		return it->second;

	return nullptr;
}

void Context::update_sym(const std::string str, Decl* d) {
	auto it = symbol_table.find(str);
	if(it == symbol_table.end())
		throw std::runtime_error("Symbol not in table.");

	symbol_table[str] = d;
	return;
}