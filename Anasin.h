/* ====================================================== *
* Anasin.h                                                *
* ------------------------------------------------------- *
* Analisador Sintatico/Semantico                          *
* :                                                       *
* Tem a funcao de realizar as validacoes sintaticas e     *
* semanticas, atraves da lista de token e de lexemas      *
* geradas a partir do processamento do Analisador Lexico  *
* alem de gerar o codigo intermediario MEPA.              *
* ======================================================= */

#ifndef _ANASIN_H_
#define _ANASIN_H_

//includes
#include "global.h"

class Anasin {

	public:
		// construtor-destrutor
		Anasin(vAnalex analex);
		~Anasin();

		// funcao principal
		void executa();

		/* ------------------------------------
			SINTATICO
		------------------------------------ */
		// funcoes sintatico
		void PG();				//programa
		void LID();				//lista-decl
		void LIDf();			//lista-decl'
		void DE();				//decl
		void DEV();				//decl-var
		void DEM();				//decl-main
		int EST();				//espec-tipo
		void BL();				//bloco
		void LIC();				//lista-com
		void CO();				//comando
		void COA();				//com-atrib
		void COL();				//com-leitura
		void COE();				//com-escrita
		void COS();				//com-selecao
		void COSf(string rot);	//com-selecao'
		void COR();				//com-repeticao
		void EX(sAnasem as);	//exp
		void EXf(sAnasem as);	//exp'
		void OPS();				//op-soma
		void EXM(sAnasem as);	//exp-mult
		void EXMf(sAnasem as);	//exp-mult'
		void OPM();				//op-mult
		void EXS(sAnasem as);	//exp-simples
		void LI(sAnasem as);	//literal
		string VA();			//var
		string VA(sAnasem as);	//var

		// funcoes automato
		void proxToken();
		string getToken();
		string getLexema();
		void aceitaToken(string token, string msg);
		void aceitaToken(vString ltoken, string msg);

		/* ------------------------------------
			SEMANTICO
		------------------------------------ */
		// -- Verificacoes semanticas:
		// -- 1. identificador redeclarado				
		// -- 2. identificador não declarado			
		// -- 3. tipo do identificador não compatível	
		// -- 4. variável não inicializada				
		// -- 5. variável não utilizada					

		// funcoes ts
		void TS_insere(sAnasem anasem);					// insere variavel
		void TS_atualiza(int id, sAnasem anasem);		// atuliza variavel
		void TS_remove(int id);							// remove variavel
		sAnasem TS_consulta(int id);					// consulta dados variavel
		int TS_busca(string nome);						// busca variavel (ret.id)
		int TS_tipo(int id);							// consulta tipo variavel
		void TS_lista();								// lista variaveis
		int TS_qtdvar();								// conta quantidade de variaveis 
		void TS_finaliza();								// encerra variaveis
		vAnasem get_TS();								// retorna dados da TS

		/* ------------------------------------
			MEPA
		------------------------------------ */
		void MP_insere(string instr);
		void MP_insere(string instr, int param);
		void MP_insere(string instr, string param);
		void MP_insereRotulo(string nome);
		void MP_lista();
		string MP_geraRotulo();
		vString get_MEPA();

		/* ------------------------------------
			OUTRAS FUNCOES
		------------------------------------ */
		void errosin(string funcao, string msg);
		void errosem(string funcao, string msg, string compl);
		void errosem(string funcao, string msg, int compl);
		void errosem(string funcao, string msg, int compl1, int compl2);
		void errosem(string funcao, string msg, string compl1, int compl2);
		void aviso(string funcao, string msg, string compl);
		void encerra();

	private:
		// -- sintatico
		int _pos;
		string _token;
		vAnalex _analex;
		ofstream _logsin;
		bool _errosin;

		// -- semantico
		int _id;
		vAnasem _anasem;
		ofstream _lstsem;
		ofstream _logsem;
		int _errosem;

		// -- mepa
		int _rotulo;
		vString _mepa;
		ofstream _codmepa;
};

#endif