#pragma once
#include "lexer.h"
#include "error.h"


using Error::error;
using namespace Lexer;

namespace Parser {
	double prim(bool get, Token_stream& ts);
	double term(bool get, Token_stream& ts);
	double expr(bool get, Token_stream& ts);
}