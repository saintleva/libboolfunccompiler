

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::BooleanFunctions() {
		FunctionDefinition();
		while (la->kind == 4 || la->kind == 6) {
			FunctionDefinition();
		}
}

void Parser::FunctionDefinition() {
		if (la->kind == 4) {
			PostfixFuncDefinition();
		} else if (la->kind == 6) {
			InfixFuncDefinition();
		} else SynErr(19);
}

void Parser::PostfixFuncDefinition() {
		Sem::FunctionDefinition& current = functions.addUnspecifiedFunc(); 
		Expect(4);
		FunctionHeader(current.header);
		functions.indexLastFunc(current.header.name); 
		PostfixExpression(current.rpn, current.header);
		Expect(5);
		PBF::checkCorrectnessOfRpn(current.rpn); 
}

void Parser::InfixFuncDefinition() {
		Sem::FunctionDefinition& current = functions.addUnspecifiedFunc(); 
		Expect(6);
		FunctionHeader(current.header);
		functions.indexLastFunc(current.header.name); 
		InfixExpression(current.rpn, current.header);
		Expect(5);
		PBF::checkCorrectnessOfRpn(current.rpn); 
}

void Parser::FunctionHeader(Sem::FunctionHeader& header) {
		Expect(1);
		header.name = wide2string(t->val);
		if (Sem::predefinedOperation(header.name))
		    SemErr((boost::wformat(L"There is predefined function with name %1%") % t->val).str().c_str());
		if (functions.isDefined(header.name))
		    SemErr((boost::wformat(L"Function '%1%' is already defined") % t->val).str().c_str());
		
		Expect(7);
		ParamList(header.params);
		Expect(8);
		Expect(9);
}

void Parser::PostfixExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		using PBF::TokenPtr;
		PBF::Operand* operand; 
		Operand(operand,header);
		rpn.push_back(TokenPtr(operand)); 
		while (StartOf(1)) {
			if (StartOf(2)) {
				Operand(operand,header);
				rpn.push_back(TokenPtr(operand)); 
			} else {
				PBF::AbstractOperation* op; 
				FunctionNameUsing(op);
				rpn.push_back(TokenPtr(op)); 
			}
		}
}

void Parser::InfixExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		AdditiveTerm(rpn,header);
		PrefixedByOperatorInfixExpression(rpn,header);
}

void Parser::ParamList(Sem::FunctionHeader::ParamMap& params) {
		typedef Sem::FunctionHeader::ParamMap::value_type Pair;
		Sem::FunctionHeader::Arity argIndex = 0; 
		if (la->kind == 2) {
			Get();
			params.insert(Pair(wide2string(t->val), argIndex++)); 
			while (la->kind == 10) {
				Get();
				Expect(2);
				params.insert(Pair(wide2string(t->val), argIndex++)); 
			}
		}
}

void Parser::Operand(PBF::Operand*& operand, const Sem::FunctionHeader& header) {
		if (la->kind == 3 || la->kind == 16 || la->kind == 17) {
			PBF::Constant* constant; 
			Literal(constant);
			operand = constant; 
		} else if (la->kind == 2) {
			Get();
			try
			{
			    operand = new PBF::Variable(header.findParameter(wide2string(t->val)));
			}
			catch (Sem::UnknownParameter& error)
			{
			    SemErr(string2wide(error.what()).c_str());
			}
			
		} else SynErr(20);
}

void Parser::FunctionNameUsing(PBF::AbstractOperation*& operation) {
		Expect(1);
		Sem::String name(wide2string(t->val));
		operation = Sem::predefinedOperation(name);
		if (!operation)
		    try
		    {
		        operation = new PBF::RpnFunctionOperation(functions.findRpnFunction(name));
		    }
		    catch (Sem::FunctionHasNotDefined& error)
		    {
		        SemErr(string2wide(error.what()).c_str());
		    }
		
}

void Parser::AdditiveTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		MultiplicativeTerm(rpn,header);
		PrefixedByOperatorAdditiveTerm(rpn,header);
}

void Parser::PrefixedByOperatorInfixExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		if (la->kind == 11) {
			Get();
			InfixExpression(rpn,header);
			rpn.push_back(PBF::TokenPtr(PBF::BinaryOperators::Equal.clone())); 
		}
}

void Parser::MultiplicativeTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		NegativeTerm(rpn,header);
		PrefixedByOperatorMultiplicativeTerm(rpn,header);
}

void Parser::PrefixedByOperatorAdditiveTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		if (la->kind == 12 || la->kind == 13) {
			PBF::BinaryOperation* op; 
			if (la->kind == 12) {
				Get();
				op = PBF::BinaryOperators::Or.clone(); 
			} else {
				Get();
				op = PBF::BinaryOperators::Xor.clone(); 
			}
			AdditiveTerm(rpn,header);
			rpn.push_back(PBF::TokenPtr(op)); 
		}
}

void Parser::NegativeTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		if (la->kind == 15) {
			Get();
			SingleExpression(rpn,header);
			rpn.push_back(PBF::TokenPtr(PBF::UnaryOperators::Not.clone())); 
		} else if (StartOf(3)) {
			SingleExpression(rpn,header);
		} else SynErr(21);
}

void Parser::PrefixedByOperatorMultiplicativeTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		if (la->kind == 14) {
			Get();
			MultiplicativeTerm(rpn,header);
			rpn.push_back(PBF::TokenPtr(PBF::BinaryOperators::And.clone())); 
		}
}

void Parser::SingleExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		if (la->kind == 1) {
			PBF::AbstractOperation* op; 
			FunctionNameUsing(op);
			Expect(7);
			InfixExpressionList(rpn,header);
			Expect(8);
			rpn.push_back(PBF::TokenPtr(op)); 
		} else if (la->kind == 7) {
			Get();
			InfixExpression(rpn,header);
			Expect(8);
		} else if (StartOf(2)) {
			PBF::Operand* operand; 
			Operand(operand,header);
			rpn.push_back(PBF::TokenPtr(operand)); 
		} else SynErr(22);
}

void Parser::InfixExpressionList(PBF::Rpn& rpn, const Sem::FunctionHeader& header) {
		if (StartOf(4)) {
			InfixExpression(rpn,header);
			while (la->kind == 10) {
				Get();
				InfixExpression(rpn,header);
			}
		}
}

void Parser::Literal(PBF::Constant*& value) {
		if (la->kind == 3) {
			Get();
			value = new PBF::Constant(wide2string(t->val) == "1"); 
		} else if (la->kind == 16) {
			Get();
			value = new PBF::Constant(false); 
		} else if (la->kind == 17) {
			Get();
			value = new PBF::Constant(true); 
		} else SynErr(23);
}



void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	BooleanFunctions();

	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 18;

	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[5][20] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,T,T,T, x,x,x,x, x,x,x,x, x,x,x,x, T,T,x,x},
		{x,x,T,T, x,x,x,x, x,x,x,x, x,x,x,x, T,T,x,x},
		{x,T,T,T, x,x,x,T, x,x,x,x, x,x,x,x, T,T,x,x},
		{x,T,T,T, x,x,x,T, x,x,x,x, x,x,x,T, T,T,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"functionName expected"); break;
			case 2: s = coco_string_create(L"parameterName expected"); break;
			case 3: s = coco_string_create(L"numericalLiteral expected"); break;
			case 4: s = coco_string_create(L"\"POSTFIXFUNC\" expected"); break;
			case 5: s = coco_string_create(L"\";\" expected"); break;
			case 6: s = coco_string_create(L"\"INFIXFUNC\" expected"); break;
			case 7: s = coco_string_create(L"\"(\" expected"); break;
			case 8: s = coco_string_create(L"\")\" expected"); break;
			case 9: s = coco_string_create(L"\"=\" expected"); break;
			case 10: s = coco_string_create(L"\",\" expected"); break;
			case 11: s = coco_string_create(L"\"==\" expected"); break;
			case 12: s = coco_string_create(L"\"||\" expected"); break;
			case 13: s = coco_string_create(L"\"^^\" expected"); break;
			case 14: s = coco_string_create(L"\"&&\" expected"); break;
			case 15: s = coco_string_create(L"\"!\" expected"); break;
			case 16: s = coco_string_create(L"\"FALSE\" expected"); break;
			case 17: s = coco_string_create(L"\"TRUE\" expected"); break;
			case 18: s = coco_string_create(L"??? expected"); break;
			case 19: s = coco_string_create(L"invalid FunctionDefinition"); break;
			case 20: s = coco_string_create(L"invalid Operand"); break;
			case 21: s = coco_string_create(L"invalid NegativeTerm"); break;
			case 22: s = coco_string_create(L"invalid SingleExpression"); break;
			case 23: s = coco_string_create(L"invalid Literal"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}



