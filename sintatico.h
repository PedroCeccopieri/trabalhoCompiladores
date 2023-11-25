#include <vector>
#include <string>

void Block();
void Stmt();
void Stmt2();
void Stmt3();
void Exps();
void Exp();
void Exp2();
void PrefixExp();
void PrefixExp2();
void Field();
void BinOp();
void Vars();
void Var();
void Var2();
void Function();
void FunctionBody();
void Params();
void Names();

void getToken();
bool isIn(std::vector<std::string> vec);
bool isToken(std::string t, std::vector<std::string> procurar);
void panic(std::vector<std::string> esperado, std::vector<std::string> procurar);
