#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "lexico.h"
#include "sintatico.h"
#include "first.h"

Token token;

int main () {

	std::string file = "programa.txt";
	code = readFile(file.c_str());

	getToken();

	while (token.nome != "EOF") Block();

	std::cout << "DONE" << std::endl;
	std::cout << "Panic: " << countPanic << std::endl;
}

void getToken() {
	token = proximo_token();
	// std::cout << "< " << token.nome << " | " << token.atributo << " >" << std::endl;
	if (token.nome == "short comment" || token.nome == "long comment") getToken();
}

bool isIn(std::vector<std::string> vec) {
	return std::find(vec.begin(), vec.end(), token.nome) != vec.end();
}

bool isToken(std::string t, std::vector<std::string> procurar) {
	if (token.nome == t) {
		getToken();
		return true;
	} else {
		panic(std::vector<std::string> {t}, procurar);
		return true;
	}

	return false;
}

void panic(std::vector<std::string> esperado, std::vector<std::string> procurar) {
	countPanic++;

	std::string inicial = token.nome;

	while (!isIn(procurar)) {
		getToken();

		if (token.nome == "EOF") {
		break;
		}
	}

	std::string final = token.nome;

	std::cout << "-------------PANIC-------------" << std::endl;
	std::cout << "linha: " << linha << std::endl;
	std::cout << "token inicial: " << inicial << std::endl;
	std::cout << "token esperado: " << esperado[0];
	for (int i = 1; i < esperado.size(); i++) {
		std::cout << " | " << esperado[i];
	}
	std::cout << std::endl << "token a procurar: " << procurar[0];
	for (int i = 1; i < procurar.size(); i++) {
		std::cout << " | " << procurar[i];
	}
	std::cout << std::endl;

	std::cout << "token final: " << final << std::endl;
}

std::vector<std::string> merge(std::vector<std::vector<std::string>> vetores) {

	std::vector<std::string> v;

	for (int i = 0; i < vetores.size(); i++) {
		v.insert(v.end(), vetores[i].begin(), vetores[i].end());
	}

	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end());

	return v;
}

void Block() {
	// countBlock++;
	// std::cout << "Block " << countBlock << " executado" << std::endl;

	if (isIn(First_Block)) {
		Stmt();
		isToken(";", First_Block);
		Block();
	} else {
		if (!isIn(Follow_Block)) {
			panic(First_Block, merge(std::vector<std::vector<std::string>> {First_Block, Follow_Block}));
		}
	}

	// std::cout << "Block " << countBlock << " finalizado" << std::endl;
	// countBlock--;
}

void Stmt() {
	// countStmt++;
	// std::cout << "Stmt " << countStmt << " executado" << std::endl;

	if (isIn(First_Stmt)) {

		if (isIn(First_Vars)) {
			Vars();
			isToken("=", merge(std::vector<std::vector<std::string>> {First_Exps, Follow_Stmt}));
			Exps();
		}

		else if (isIn(First_Function)) {
			Function();
		}

		else if (token.nome == "do") {
			getToken();
			Block();
			isToken("end", Follow_Stmt);
		}

		else if (token.nome == "while") {
			getToken();
			Exp();
			isToken("do", merge(std::vector<std::vector<std::string>> {First_Block, Follow_Stmt}));
			Block();
			isToken("end", Follow_Stmt);
		}

		else if (token.nome == "if") {
			getToken();
			Exp();
			isToken("then", merge(std::vector<std::vector<std::string>> {First_Block, Follow_Block, Follow_Stmt}));
			Block();

			while (token.nome == "elseif") {
				getToken();
				Exp();
				isToken("then", merge(std::vector<std::vector<std::string>> {First_Block, Follow_Block, Follow_Stmt}));
				Block();
			}

			if (token.nome == "else") {
				getToken();
				Block();
			}

			isToken("end", Follow_Stmt);
		}

		else if (token.nome == "return") {
			getToken();
			Exps();
		}

		else if (token.nome == "break") {
			getToken();
		}

		else if (token.nome == "for") {
			getToken();
			isToken("name", merge(std::vector<std::vector<std::string>> {First_Stmt2, Follow_Stmt}));
			Stmt2();
			isToken("do", merge(std::vector<std::vector<std::string>> {First_Block, Follow_Stmt2, std::vector<std::string> {"end"}}));
			Block();
			isToken("end", Follow_Stmt2);
		}

		else if (token.nome == "local") {
			getToken();
			Stmt3();
		}
	} else {
		panic(First_Stmt, Follow_Stmt);
	}

	// std::cout << "Stmt " << countStmt << " finalizado" << std::endl;
	// countStmt--;
}

void Stmt2() {
	// countStmt2++;
	// std::cout << "Stmt2 " << countStmt2 << " executado" << std::endl;

	if (isIn(First_Stmt2)){
		if (token.nome == "=") {
			getToken();
			Exp();
			isToken(",", merge(std::vector<std::vector<std::string>> {First_Exp, First_Block, Follow_Stmt2, std::vector<std::string> {",", "do", "end"}}));
			Exp();

			if (token.nome == ",") {
				getToken();
				Exp();
			}
		}

		else if (token.nome == "in") {
			getToken();
			Exps();
		}

		else if (token.nome == ",") {
			getToken();
			isToken("name", merge(std::vector<std::vector<std::string>> {First_Exps, First_Block, Follow_Stmt2, std::vector<std::string> {"in", ",", "do", "end"}}));

			while (token.nome == ",") {
				getToken();
				isToken("name", merge(std::vector<std::vector<std::string>> {First_Exps, First_Block, Follow_Stmt2, std::vector<std::string> {"in", ",", "do", "end"}}));
			}

			isToken("in", merge(std::vector<std::vector<std::string>> {First_Exps, First_Block, Follow_Stmt2, std::vector<std::string> {"do", "end"}}));
			Exps();
		}
	} else {
		panic(First_Stmt2, Follow_Stmt2);
	}

	// std::cout << "Stmt2 " << countStmt2 << " finalizado" << std::endl;
	// countStmt2--;
}

void Stmt3() {
	// countStmt3++;
	// std::cout << "Stmt3 " << countStmt3 << " executado" << std::endl;

	if (isIn(First_Stmt3)) {
		if (isIn(First_Function)) {
			Function();
		}

		else if (isIn(First_Names)) {
			Names();
			isToken("=", First_Exps);
			Exps();
		}
	} else {
		panic(First_Stmt3, Follow_Stmt3);
	}
	// std::cout << "Stmt3 " << countStmt3 << " finalizado" << std::endl;
	// countStmt3--;
}

void Exps() {
	// countExps++;
	// std::cout << "Exps " << countExps << " executado" << std::endl;

	if (isIn(First_Exps)) {
		Exp();
		while (token.nome == ",") {
			getToken();
			Exp();
		}
	} else {
		panic(First_Exps, Follow_Exps);
	}

	// std::cout << "Exps " << countExps << " finalizado" << std::endl;
	// countExps--;
}

void Exp() {
	// countExp++;
	// std::cout << "Exp " << countExp << " executado" << std::endl;

	if (isIn(First_Exp)) {
		if (token.nome == "not" || token.nome == "-") {
			getToken();
			Exp();
			Exp2();
		}

		else if (isIn(First_PrefixExp)) {
			PrefixExp();
			Exp2();
		}

		else if (isIn(First_Function)) {
			Function();
			Exp2();
		}

		else if (token.nome == "{") {
			getToken();
			Field();

			while (token.nome == ",") {
				getToken();
				Field();
			}

			isToken("}", First_Exp2);
			Exp2();
		}

		else if (token.nome == "nil" || token.nome == "true" || token.nome == "false" || token.nome == "number" || token.nome == "string") {
			getToken();
			Exp2();
		}
	} else {
		panic(First_Exp, Follow_Exp);
	}

	// std::cout << "Exp " << countExp << " finalizado" << std::endl;
	// countExp--;
}

void Exp2() {
	// countExp2++;
	// std::cout << "Exp2 " << countExp2 << " executado" << std::endl;

	if (isIn(First_Exp2)) {
		BinOp();
		Exp();
		Exp2();
	}

	// std::cout << "Exp2 " << countExp2 << " finalizado" << std::endl;
	// countExp2--;
}

void PrefixExp() {
	// countPrefixExp++;
	// std::cout << "PrefixExp " << countPrefixExp << " executado" << std::endl;

	if (isIn(First_PrefixExp)) {
		if (token.nome == "name") {
			getToken();
			PrefixExp2();
		}

		else if (token.nome == "(") {
			getToken();
			Exp();
			isToken(")", First_PrefixExp2);
			PrefixExp2();
		}
	} else {
		panic(First_PrefixExp, Follow_PrefixExp);
	}

	// std::cout << "PrefixExp " << countPrefixExp << " finalizado" << std::endl;
	// countPrefixExp--;
}

void PrefixExp2() {
	// countPrefixExp2++;
	// std::cout << "PrefixExp2 " << countPrefixExp2 << " executado" << std::endl;

	if (isIn(First_PrefixExp2)) {
		if (token.nome == "[") {
			getToken();
			Exp();
			isToken("]", First_PrefixExp2);
			PrefixExp2();
		}
	}

	// std::cout << "PrefixExp2 " << countPrefixExp2 << " finalizado" << std::endl;
	// countPrefixExp2--;
}

void Field() {
	// countField++;
	// std::cout << "Field " << countField << " executado" << std::endl;

	if (isIn(First_Field)) {
		if (token.nome == "[") {
			getToken();
			Exp();
			isToken("]", merge(std::vector<std::vector<std::string>> {First_Exp, std::vector<std::string> {"="}}));
			isToken("=", First_Exp);
			Exp();
		}

		else if (token.nome == "name") {
			getToken();
			isToken("=", First_Exp);
			Exp();
		}
	} else {
		panic(First_Field, Follow_Field);
	}

	// std::cout << "Field " << countField << " finalizado" << std::endl;
	// countField--;
}

void BinOp() {
	// countBinOp++;
	// std::cout << "BinOp " << countBinOp << " executado" << std::endl;

	if (isIn(First_BinOp)) {
		getToken();
	} else {
		panic(First_BinOp, Follow_BinOp);
	}

	// std::cout << "BinOp " << countBinOp << " finalizado" << std::endl;
	// countBinOp--;
}

void Vars() {
	// countVars++;
	// std::cout << "Vars " << countVars << " executado" << std::endl;

	if (isIn(First_Vars)) {
		Var();
		while (token.nome == ",") {
			getToken();
			Var();
		}
	} else {
		panic(First_Vars, Follow_Vars);
	}

	// std::cout << "Vars " << countVars << " finalizado" << std::endl;
	// countVars--;
}

void Var() {
	// countVar++;
	// std::cout << "Var " << countVar << " executado" << std::endl;

	if (isIn(First_Var)) {
		if (token.nome == "name") {
			getToken();
			Var2();
		}

		else if (token.nome == "(") {
			getToken();
			Exp();
			isToken(")", First_PrefixExp2);
			PrefixExp2();
			isToken("[", First_Exp);
			Exp();
			isToken("]", Follow_Var);
		}
	} else {
		panic(First_Var, Follow_Var);
	}

	// std::cout << "Var " << countVar << " finalizado" << std::endl;
	// countVar--;
}

void Var2() {
	// countVar2++;
	// std::cout << "Var2 " << countVar2 << " executado" << std::endl;

	if (isIn(First_Var2)) {
		PrefixExp2();
		isToken("[", First_Exp);
		Exp();
		isToken("]", Follow_Var2);
	}

	// std::cout << "Var2 " << countVar2 << " finalizdo" << std::endl;
	// countVar2--;
}

void Function() {
	// countFunction++;
	// std::cout << "Function " << countFunction << " executado" << std::endl;

	if (isIn(First_Function)) {
		isToken("function", First_FunctionBody);
		FunctionBody();
	} else {
		panic(First_Function, Follow_Function);
	}

	// std::cout << "Function " << countFunction << " finalizado" << std::endl;
	// countFunction--;
}

void FunctionBody() {
	// countFunctionBody++;
	// std::cout << "FunctionBody " << countFunctionBody << " executado" << std::endl;

	if (isIn(First_FunctionBody)) {
		isToken("name", merge(std::vector<std::vector<std::string>> {First_Params, std::vector<std::string> {"("}}));
		isToken("(", First_Params);
		Params();
		isToken(")", First_Block);
		Block();
		isToken("end", Follow_FunctionBody);
	} else {
		panic(First_FunctionBody, Follow_FunctionBody);
	}

	// std::cout << "FunctionBody " << countFunctionBody << " finalizado" << std::endl;
	// countFunctionBody--;
}

void Params() {
	// countParams++;
	// std::cout << "Params " << countParams << " executado" << std::endl;

	if (isIn(First_Params)) {
		Names();
	}

	// std::cout << "Params " << countParams << " finalizado" << std::endl;
	// countParams--;
}

void Names() {
	// countNames++;
	// std::cout << "Names " << countNames << " executado" << std::endl;

	if (isIn(First_Names)) {
		getToken();
		while (token.nome == ",") {
			getToken();
			if (token.nome == "name") {
				getToken();
			}
		}
	} else {
		panic(First_Names, Follow_Names);
	}

	// std::cout << "Names " << countNames << " finalizado" << std::endl;
	// countNames--;
}