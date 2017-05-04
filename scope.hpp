#pragma once

enum {
	function_parameter_scope,
	function_scope,
	block_scope
}

struct scope {

scope(int k) : scope(k, nulltr) { }

virtual ~scope() = defualt;


};