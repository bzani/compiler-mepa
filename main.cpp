/* ====================================================== *
* main.cpp                                                *
* ------------------------------------------------------- *
* Programa Principal                                      *
* :                                                       *
* Realiza os processos de validacao lexica, sintatica e   *
* semantica do codigo de entrada, atraves das classes     *
* Analex e Anasin, e deste, gerando a partir deste o      *
* arquivo executavel                                      *
* ======================================================= */

#include "Analex.h"
#include "Anasin.h"

string leEntrada(string);

int main(int argc, char *argv[]) {

	/* ------------------------------
	 * leitura do programa
	 * ------------------------------ */
	if (argc != 2) {
		printf("Entrar com: Compilador.exe <nome_pgm>");
		exit(1);
	}
	
	string programa = leEntrada(argv[1]);

	printf("\nIniciando compilacao...\n\n");

	/* ------------------------------
	 * Analise lexica
	 * ------------------------------ */
	Analex analex(programa);
	
	analex.executa();

	analex.gravaLista();

	/* ------------------------------
	 * Analise sintatica
	 * Analise semantica
	 * Geracao de codigo
	 * ------------------------------ */
	Anasin anasin(analex.getAnalex());

	anasin.executa();

	/* ------------------------------
	 * fim execucao
	 * ------------------------------ */
	printf("\n\n---\nPrograma compilado com sucesso.\n");
	cin.get();

	return 0;
}

string leEntrada(string nome)
{
	string l, s = "";
	ifstream entrada(nome);
	if (entrada.is_open()) {
		while (!entrada.eof()) {
			getline(entrada, l);
			s.append(l);
		}
		entrada.close();
	}
	return s;
}
