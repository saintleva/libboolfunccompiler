

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include "predefined-operations.h"
#include "semantic-data.h"
#include "exceptions.h"
#include <string>
#include <vector>
#include <iostream>
#include <boost/format.hpp>
#include <polishboolfunctions/Operand.h>
#include <polishboolfunctions/Constant.h>
#include <polishboolfunctions/Variable.h>
#include <polishboolfunctions/AbstractOperation.h>
#include <polishboolfunctions/NotOperation.h>
#include <polishboolfunctions/BinaryOperation.h>
#include <polishboolfunctions/RpnFunctionOperation.h>

namespace PBF = PolishBoolFunctions;


#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_functionName=1,
		_parameterName=2,
		_numericalLiteral=3
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

Sem::FunctionContainer functions;




	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void BooleanFunctions();
	void FunctionDefinition();
	void PostfixFuncDefinition();
	void InfixFuncDefinition();
	void FunctionHeader(Sem::FunctionHeader& header);
	void PostfixExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void InfixExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void ParamList(Sem::FunctionHeader::ParamMap& params);
	void Operand(PBF::Operand*& operand, const Sem::FunctionHeader& header);
	void FunctionNameUsing(PBF::AbstractOperation*& operation);
	void AdditiveTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void PrefixedByOperatorInfixExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void MultiplicativeTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void PrefixedByOperatorAdditiveTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void NegativeTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void PrefixedByOperatorMultiplicativeTerm(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void SingleExpression(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void InfixExpressionList(PBF::Rpn& rpn, const Sem::FunctionHeader& header);
	void Literal(PBF::Constant*& value);

	void Parse();

}; // end Parser



#endif // !defined(COCO_PARSER_H__)

