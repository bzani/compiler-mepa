/* ====================================================== *
* Analex.h                                                *
* ------------------------------------------------------- *
* Analisador Lexico                                       *
* :                                                       *
* Tem a funcao de realizar as validacoes lexicas do       *
* programa - fornecido na entrada do programa principal - *
* por meio de regras da gramatica D+                      *
* ======================================================= */

#ifndef _ANALEX_H_
#define _ANALEX_H_

//includes
#include "global.h"

class Analex {

	public:
		//construtor-destrutor
		Analex(string pgm);
		~Analex();
		
		//funcao principal
		void executa();

		//funcoes automato
		char proxChar();
		char voltaChar();
		void outro(string nome);
		void erro(string estado, string msg);

		//funcoes classificacao
		bool ehNumero(char c);
		int ehCaractEsp(char c);
		bool ehSeparador(char c);
		bool ehId(char c);
		string verificaPalavra(string w);
		string verifTokenSimples(char c);

		//funcoes token
		void gravaToken(string nome);
		void incrementaToken(char c);
		void limpaToken();

		//funcoes arquivo
		void gravaLista();

		//funcoes auxiliares
		const char* str2char(string s);
		string char2str(const char* c);
		string char2str(char c[]);
		string vchar2str(vChar vc);
		vChar str2vchar(string s);

		//getters/setters
		vAnalex getAnalex();

	private:
		//variaveis de tratamento
		int _estado;
		int _pos;
		vChar _vPrograma;
		vChar _vLexema;
		bool _errolex;

		//estrutura de armazenamento
		vAnalex _analex;

		//arquivos de saida
		ofstream _loglex;
		ofstream _lstlex;
};

#endif