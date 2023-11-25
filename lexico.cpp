#include <iostream>
#include <string>
#include <cstring>
#include "lexico.h"

int cont_sim_lido = 0;
char *code;

int estado = 0;
int partida = 0;
int linha = 1;

std::string stringToken = "";
std::string numToken = "";
std::string idToken = "";
int bkts = 2;
int cb = 0;

std::string rwords[] = {"and", "break", "do", "else",
"elseif", "end", "false", "for", "function", "if",
"in", "local", "nil", "not", "or", "repeat", "return",
"then", "true", "until", "while"};


char *readFile(const char *fileName) {
	FILE *file = fopen(fileName, "r");
	char *code;
	int n = 0;
	int c;

	if(file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	code = (char*) malloc(f_size*sizeof(char) + 1);

	while ((c = fgetc(file))!= EOF)
	{
		code[n++] = (char) c;
	}
	code[n] = EOF;
	return code;
}

Token proximo_token() {
	Token token;
	char c;

	while(code[cont_sim_lido] != EOF) {

		c = code[cont_sim_lido];
		// std::cout << cont_sim_lido << " " << c << " estado:" << estado << std::endl;
		
		switch (estado) {
	
			case 0:
		
				while (c == ' ' || c == '\n') {
					if (c == '\n') linha++;
					estado = 0;
					c = code[++cont_sim_lido];
				}

				// Operadores //

				if (c == '+') estado = 1;
				else if (c == '-') estado = 2;
				else if (c == '*') estado = 3;
				else if (c == '/') estado = 4;
				else if (c == '^') estado = 5;
				else if (c == '(') estado = 6;
				else if (c == ')') estado = 7;
				else if (c == '{') estado = 8;
				else if (c == '}') estado = 9;
				else if (c == '[') estado = 10;
				else if (c == ']') estado = 11;
				else if (c == ';') estado = 12;
				else if (c == ':') estado = 13;
				else if (c == ',') estado = 14;
				else if (c == '.') estado = 15;
				else if (c == '=') estado = 16;
				else if (c == '~') estado = 17;
				else if (c == '<') estado = 18;
				else if (c == '>') estado = 19;


				// Strings //

				else if (c == '"') estado = 25;

				// Float //

				else if (isdigit(c)) estado = 26;
				
				// Id //
				else if (c == '_' or isalpha(c)) estado = 32;

				else estado = -1;
				break;

			case 1:
				// std::cout << "<operador, +>" << std::endl;

				token.nome = "+";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 2:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				// Comment //

				if (c == '-') estado = 28;

				else {
					// std::cout << "<operador, ->" << std::endl;

					token.nome = "-";
					token.atributo = "";
					estado = 0;

					return(token);
				}

				break;

			case 3:
				// std::cout << "<operador, *>" << std::endl;

				token.nome = "*";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 4:
				// std::cout << "<operador, />" << std::endl;

				token.nome = "/";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 5:
				// std::cout << "<operador, ^>" << std::endl;

				token.nome = "^";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 6:
				// std::cout << "<operador, (>" << std::endl;

				token.nome = "(";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 7:
				// std::cout << "<operador, )>" << std::endl;

				token.nome = ")";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 8:
				// std::cout << "<operador, {>" << std::endl;

				token.nome = "{";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 9:
				// std::cout << "<operador, }>" << std::endl;

				token.nome = "}";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 10:
				// std::cout << "<operador, [>" << std::endl;

				token.nome = "[";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 11:
				// std::cout << "<operador, ]>" << std::endl;

				token.nome = "]";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 12:
				// std::cout << "<operador, ;>" << std::endl;

				token.nome = ";";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 13:
				// std::cout << "<operador, :>" << std::endl;

				token.nome = ":";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 14:
				// std::cout << "<operador, ,>" << std::endl;

				token.nome = ",";
				token.atributo = "";
				estado = 0;

				cont_sim_lido++;

				return(token);
				break;

			case 15:
				estado = 20;
				break;

			case 16:
				estado = 21;
				break;

			case 17:
				estado = 22;
				break;

			case 18:
				estado = 23;
				break;

			case 19:
				estado = 24;
				break;
			
			case 20:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '.') {
					// std::cout << "<operador, ..>" << std::endl;

					token.nome = "..";
					token.atributo = "";
					estado = 0;

					cont_sim_lido++;
		
				} else {
					// std::cout << "<operador, .>" << std::endl;

					token.nome = ".";
					token.atributo = "";
					estado = 0;
				}

				return(token);
				break;

			case 21:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '=') {
					// std::cout << "<operador, ==>" << std::endl;

					token.nome = "==";
					token.atributo = "";
					estado = 0;

					cont_sim_lido++;

				} else {
					// std::cout << "<operador, =>" << std::endl;

					token.nome = "=";
					token.atributo = "";
					estado = 0;
				}

				return(token);
				break;

			case 22:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '=') {
					// std::cout << "<operador,~=>" << std::endl;

					token.nome = "~=";
					token.atributo = "";
					estado = 0;

					cont_sim_lido++;

				} else {
					// std::cout << "<operador, ~>" << std::endl;

					// ************************
					
					token.nome = "~";
					token.atributo = "";
					estado = -1;
				}

				return(token);
				break;

			case 23:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '=') {
					// std::cout << "<operador, <=>" << std::endl;

					token.nome = "<=";
					token.atributo = "";
					estado = 0;

					cont_sim_lido++;

				} else {
					// std::cout << "<operador, <>" << std::endl;

					token.nome = "<";
					token.atributo = "";
					estado = 0;
				}

				return(token);
				break;

			case 24:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '=') {
					// std::cout << "<operador, >=>" << std::endl;

					token.nome = ">=";
					token.atributo = "";
					estado = 0;

					cont_sim_lido++;

				} else {
					// std::cout << "<operador, >>" << std::endl;

					token.nome = ">";
					token.atributo = "";
					estado = 0;
				}

				return(token);
				break;

			case 25:

				stringToken = "";
				stringToken += c;

				do {

					c = code[++cont_sim_lido];

					if (c == '\\' && code[cont_sim_lido + 1] == '"') {
						stringToken += '\"';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == '\\') {
						stringToken += '\\';
						cont_sim_lido++;
						cb += 2;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 'v') {
						stringToken += '\v';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 't') {
						stringToken += '\t';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 'r') {
						stringToken += '\r';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 'n') {
						stringToken += '\n';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 'f') {
						stringToken += '\f';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 'b') {
						stringToken += '\b';
						cont_sim_lido++;
						cb++;
					} else if (c == '\\' && code[cont_sim_lido + 1] == 'a') {
						stringToken += '\a';
						cont_sim_lido++;
						cb++;
					} else {
						stringToken += c;
						cb = 0;
					}

				} while (c != EOF && !(c == '"' && cb % 2 == 0));

				if (c == EOF) {

					// ********************* da erro //
					cont_sim_lido--;
					token.nome = "erro";
					token.atributo = "erro";
					estado = -1;

				} else {
					
					c = code[++cont_sim_lido];

					// std::cout << "<string, " << stringToken << ">" << std::endl;

					token.nome = "string";
					token.atributo = stringToken;
					estado = 0;
				}

				return(token);
				break;

			case 26:

				numToken = "";

				while (isdigit(c)) {
					numToken += c;
					c = code[++cont_sim_lido];
				}

				if (c == '.') estado = 27;
				else {

					// std::cout << "<number, " << numToken << ">" << std::endl;

					token.nome = "number";
					token.atributo = numToken;
					estado = 0;

					return(token);
				}

				break;
			
			case 27:

				numToken += c;
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (isdigit(c)) {

					while (isdigit(c)) {
						numToken += c;
						cont_sim_lido++;
						c = code[cont_sim_lido];
					}

					// std::cout << "<num, " << numToken << ">" << std::endl;

					token.nome = "num";
					token.atributo = numToken;
					estado = 0;
					
				} else {

					if (c == EOF) cont_sim_lido--;

					token.nome = "error";
					token.atributo = "error";
					estado = -1;
				}

				return(token);
				break;

			case 28:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '[') estado = 29;
				else estado = 31;

				break;
			
			case 29:

				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '[') {
					estado = 30;
					bkts = 2;
				} else estado = 31;

				break;

			case 30:

				while (bkts != 0) {
					c = code[++cont_sim_lido];
					if (c == '[') bkts += 1;
					else if (c == ']') bkts -= 1;
				}
				
				if (c == EOF && bkts != 0) {
					cont_sim_lido--;

					token.nome = "error";
					token.atributo = "error";
					estado = -1;
				}

				if (bkts == 0) {
					// std::cout << "<long comment, >" << std::endl;

					token.nome = "long comment";
					token.atributo = "";
					estado = 0;

					c = code[++cont_sim_lido];
				}
				
				return(token);
				break;

			case 31:

				while (c != '\n' && c != EOF) {
					c = code[++cont_sim_lido];
				}

				// std::cout << "<short comment, >" << std::endl;

				token.nome = "short comment";
				token.atributo = "";
				estado = 0;

				return(token);
				break;

			case 32:

				idToken = "";

				idToken += c;
				cont_sim_lido++;
				c = code[cont_sim_lido];

				while (isdigit(c) || isalpha(c) || c == '_') {
					idToken += c;
					c = code[++cont_sim_lido];
				}

				// std::cout << "<name, " << idToken << ">" << std::endl;

				for (int i = 0; i < 21; i++) {
					if (rwords[i] == idToken) {
						token.nome = idToken;
						token.atributo = "";
						estado = 0;

						return(token);
					}
				}
				
				token.nome = "name";
				token.atributo = idToken;
				estado = 0;

				return(token);
				break;
			
			default:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				token.nome = "";
				token.atributo = "";
				estado = 0;

				return(token);
				break;
		}
	}

	token.nome = "EOF";
	token.atributo = "";
	return(token);
}


