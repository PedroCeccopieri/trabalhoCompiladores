#include <string>
#include <cstring>

extern int cont_sim_lido;
extern char *code;
extern int linha;

struct Token {
 std::string nome = "";
 std::string atributo = "";
};

char *readFile(const char *fileName);
Token proximo_token();