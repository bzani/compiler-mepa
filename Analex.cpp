/* ====================================================== *
* Analex.cpp                                              *
* ======================================================= */

#include "Analex.h"

/* --------------------------------
 * construtor/destrutor
 * -------------------------------- */
Analex::Analex(string pgm)
{
	//variaveis do Analex
	_estado = eInicial;
	_pos = 0;
	_vPrograma = str2vchar(pgm);
	_errolex = 0;
	
	//log de saida
	_loglex.open("log_analex.txt");
	_lstlex.open("lista_analex.txt");
	_loglex << "-- Inicio Log Analex --" << endl;
}

Analex::~Analex()
{
	//destrutor
	_loglex << "-- Fim Log Analex --" << endl;
	_loglex.close();
	_lstlex.close();
}

/* --------------------------------
 * funcao principal
 * -------------------------------- */
void Analex::executa()
{
	char ch = proxChar();
	while (true) 
	{
		switch (_estado) 
		{
			
			// -- ESTADO INICIAL
			case eInicial:
				//numero
				if (ehNumero(ch)) 
				{
					incrementaToken(ch);
					_estado = eNumero1;
				} 
				else 
				{
					//operadores simples
					int e = ehCaractEsp(ch);
					if (e == eInicial)
					{
						incrementaToken(ch);
						gravaToken(verifTokenSimples(ch));
					}
					//operadores compostos
					else if (e > eInicial) 
					{
						incrementaToken(ch);
						_estado = e;
					}
					//ID
					else if (ehId(ch))
					{
						incrementaToken(ch);
						_estado = eID;
					}
					else if (!(ehSeparador(ch)))
						erro("inicial", "Caracter nao identificado.");

				}
				break;
			
			// -- NUMERO INTEIRO
			case eNumero1:
				//numero
				if (ehNumero(ch)) 
					incrementaToken(ch);

				//real -> simbolo decimal (. ou ,)
				else if (ch == CE_PON || ch == CE_VIR) 
				{
					incrementaToken(ch);
					_estado = eReal;
				}
				//separador
				else if (ehSeparador(ch)) 
					gravaToken(TK_NUMINT);

				//outro
				else 
					outro(TK_NUMINT);
				break;

			// -- NUMERO REAL
			case eReal:
				//numero
				if (ehNumero(ch)) 
				{
					incrementaToken(ch);
					_estado = eNumero2;
				}
				//separador
				else if (ehSeparador(ch)) 
					gravaToken(TK_NUMREAL);

				//outro
				else 
					outro(TK_NUMREAL);
				break;

			// -- NUMERO REAL (DECIMAL)
			case eNumero2:
				//numero
				if (ehNumero(ch))
					incrementaToken(ch);

				//separador
				else if (ehSeparador(ch)) 
					gravaToken(TK_NUMREAL);

				//outro
				else 
					outro(TK_NUMREAL);
				break;

			// -- SOMA / INCREMENTO / SOMA-IGUAL
			case eSoma:
				//incremento
				if (ch == CE_ADI) 
				{
					incrementaToken(ch);
					gravaToken(TK_OP_AINCR);
				}

				//soma/igual
				else if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_ASOIG);
				}

				//soma simples (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_ASOMA);

				//outro
				else
					outro(TK_OP_ASOMA);
				break;

			// -- SUBTRACAO / DECREMENTO / SUBT-IGUAL
			case eSubtr:
				//decremento
				if (ch == CE_SUB)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_ADECR);
				}

				//subtracao/igual
				else if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_ASUIG);
				}

				//subtracao simples (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_ASUBT);

				//outro
				else
					outro(TK_OP_ASUBT);
				break;

			// -- MULTIPLICACAO / MULT-IGUAL
			case eMult:
				//multiplica/igual
				if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_AMUIG);
				}

				//multiplicacao simples (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_AMULT);

				//outro
				else
					outro(TK_OP_AMULT);
				break;

			// -- DIVISAO / DIV-IGUAL / COMENTARIO
			case eDiv:
				//divide/igual
				if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_ADIIG);
				}

				//divisao simples (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_ADIVI);

				//comentario simples "//"
				else if (ch == CE_DIV) {
					limpaToken();
					_estado = eComentS;
				}

				//comentario extenso "/* */"
				else if (ch == CE_MUL) {
					limpaToken();
					_estado = eComentA;
				}

				//outro
				else
					outro(TK_OP_ADIVI);
				break;

			// -- MOD / MOD-IGUAL
			case eMod:
				//mod/igual
				if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_AMOID);
				}

				//mod simples (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_AMODU);

				//outro
				else
					outro(TK_OP_AMODU);
				break;

			// -- IGUAL A / ATRIBUICAO 
			case eIgual:
				//comparacao
				if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_LIGUA);
				}

				//atribuicao (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_AATRB);

				//outro
				else
					outro(TK_OP_AATRB);
				break;

			// -- MENOR QUE / MENOR IGUAL / BIT.ESQ.
			case eMenor:
				//menor ou igual
				if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_LMEIG);
				}

				//bit a esq.
				else if (ch == CE_MEN)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_BBIES);
				}

				//menor (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_LMENO);
				
				//outro
				else
					outro(TK_OP_LMENO);
				break;

			// -- MAIOR QUE / MAIOR IGUAL / BIT.DIR.
			case eMaior:
				//maior ou igual
				if (ch == CE_ATR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_LMAIG);
				}

				//bit a dir.
				else if (ch == CE_MAI)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_BBIDI);
				}

				//menor (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_LMAIO);

				//outro
				else
					outro(TK_OP_LMAIO);
				break;

			// -- BIT.AND / AND
			case eAnd:
				//logico
				if (ch == CE_AND)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_LAND);
				}

				//binario (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_BAND);

				//outro
				else
					outro(TK_OP_BAND);
				break;

			// -- BIT.OR / OR
			case eOr:
				//logico
				if (ch == CE_ORR)
				{
					incrementaToken(ch);
					gravaToken(TK_OP_LOR);
				}

				//binario (separador)
				else if (ehSeparador(ch))
					gravaToken(TK_OP_BOR);

				//outro
				else
					outro(TK_OP_BOR);
				break;

			// -- ID
			case eID:
				//forma ID
				if (ehId(ch))
				{
					incrementaToken(ch);
					_estado = eID;
				}

				//separador
				else if (ehSeparador(ch))
					gravaToken(TK_ID);

				//outro
				else
					outro(TK_ID);
				break;

			// -- COMENTARIO SIMPLES
			case eComentS:
				if (ch == CE_ENT)
					_estado = eInicial;
				break;

			// -- COMENTARIO LINHAS
			case eComentA:
				if (ch == CE_MUL)
					_estado = eComentF;
				break;

			// -- FIM COMENT.SIMPLES
			case eComentF:
				if (ch == CE_DIV)
					_estado = eInicial;
				else
					_estado = eComentA;
				break;

			// -- CARACTER
			case eCharA:
				if (ch == CE_GUA)
					erro("caracter", "Variavel char vazia.");
				
				else if (ehId(ch) || ehCaractEsp(ch))
				{
					incrementaToken(ch);
					_estado = eCharF;
				}
				break;

			// -- FIM CARACTER
			case eCharF:
				if (ch == CE_GUA) 
				{
					incrementaToken(ch);
					gravaToken(TK_CARACT);
				}
				break;

			// -- STRING
			case eStringA:
				if (ch == CE_ASP)
					erro("string", "Variavel string vazia.");

				else if (ehId(ch) || ehCaractEsp(ch))
				{
					incrementaToken(ch);
					_estado = eStringF;
				}
				break;

			// -- FIM STRING
			case eStringF:
				incrementaToken(ch);
				if (ch == CE_ASP)
				{
					gravaToken(TK_STRING);
				}
				break;

			// -- ERRO
			default:
				erro("(vazio)", "Estado atual nao identificado.");
		}

		//condicao de parada -> fim do programa
		if (_pos == _vPrograma.size()) {
			gravaToken(TK_FIM);
			break;
		}
		else 
			ch = proxChar();
	}

	if (!_errolex)
		cout << "-> Analise lexica realizada com sucesso." << endl;
	else
		cout << "-> Analise lexica finalizada com erro(s)." << endl;
	
}

/* --------------------------------
 * funcoes automato
 * -------------------------------- */
char Analex::proxChar()
{
	return _vPrograma[_pos++];
}

char Analex::voltaChar()
{
	return _vPrograma[_pos--];
}

void Analex::outro(string nome)
{
	gravaToken(nome);
	voltaChar();
	_estado = eInicial;
}

void Analex::erro(string funcao, string msg)
{ 
	_loglex << "**ERRO|ANALEX: funcao/estado: " << funcao << ": " << msg << endl;
	_errolex = 1;
	//exit(1);
}

/* --------------------------------
 * funcoes classificacao
 * -------------------------------- */

// -- verifica se caracter eh um numero
bool Analex::ehNumero(char c)
{
	return isdigit(c);
}

// -- verifica se caracter eh especial 
// -- no caso de nao ser letra
int Analex::ehCaractEsp(char c)
{
	switch (c) {
		case CE_APA:
		case CE_FPA:
		case CE_ACO:
		case CE_FCO:
		case CE_ACH:
		case CE_FCH:
		case CE_POV:
		case CE_DOP:
		case CE_POT:
		case CE_VIR:
		case CE_PON:
		case CE_INT:
		case CE_EXC:
		case CE_COM:
			return eInicial;
		case CE_GUA:
			return eCharA;
		case CE_ASP:
			return eStringA;
		case CE_ADI:
			return eSoma;
		case CE_SUB:
			return eSubtr;
		case CE_MUL:
			return eMult;
		case CE_DIV:
			return eDiv;
		case CE_MOD:
			return eMod;
		case CE_ATR:
			return eIgual;
		case CE_MAI:
			return eMaior;
		case CE_MEN:
			return eMenor;
		case CE_AND:
			return eAnd;
		case CE_ORR:
			return eOr;
		case CE_CIF:
		case CE_UND:
			return eID;
	}
	return 0;
}

// -- verifica se caracter eh um separador
// -- exemplo ' ', '\n', '\t'
bool Analex::ehSeparador(char c)
{
	switch (c) {
		case CE_ESP:
		case CE_ENT:
		case CE_TAB:
			return true;
	}
	return false;
}

// -- verifica se caracter indica ser inicio de id
bool Analex::ehId(char c)
{
	return (c == CE_CIF || c == CE_UND || c == CE_SUB || isalpha(c));
}

// -- verifica palavra reservada
string Analex::verificaPalavra(string w)
{
	transform(w.begin(), w.end(), w.begin(), ::tolower);

	if (w.compare(PR_BOOL) == 0)
		return TK_PR_BOOL;
	else 
	if (w.compare(PR_BREAK) == 0)
		return TK_PR_BREAK;
	else 
	if (w.compare(PR_CHAR) == 0)
		return TK_PR_CHAR;
	else 
	if (w.compare(PR_CONTI) == 0)
		return TK_PR_CONTI;
	else 
	if (w.compare(PR_DO) == 0)
		return TK_PR_DO;
	else 
	if (w.compare(PR_ELSE) == 0)
		return TK_PR_ELSE;
	else 
	if (w.compare(PR_FALSE) == 0)
		return TK_PR_FALSE;
	else 
	if (w.compare(PR_FLOAT) == 0)
		return TK_PR_FLOAT;
	else 
	if (w.compare(PR_FOR) == 0)
		return TK_PR_FOR;
	else 
	if (w.compare(PR_GOTO) == 0)
		return TK_PR_GOTO;
	else 
	if (w.compare(PR_IF) == 0)
		return TK_PR_IF;
	else 
	if (w.compare(PR_INT) == 0)
		return TK_PR_INT;
	else 
	if (w.compare(PR_RETUR) == 0)
		return TK_PR_RETUR;
	else 
	if (w.compare(PR_TRUE) == 0)
		return TK_PR_TRUE;
	else 
	if (w.compare(PR_VOID) == 0)
		return TK_PR_VOID;
	else 
	if (w.compare(PR_WHILE) == 0)
		return TK_PR_WHILE;
	else 
	if (w.compare(PR_MAIN) == 0)
		return TK_PR_MAIN;
	else 
	if (w.compare(PR_NULL) == 0)
		return TK_PR_NULL;
	else 
	if (w.compare(PR_VAR) == 0)
		return TK_PR_VAR;

	if (w.compare(PR_END) == 0)
		return TK_PR_END;

	if (w.compare(PR_REAL) == 0)
		return TK_PR_REAL;

	if (w.compare(PR_THEN) == 0)
		return TK_PR_THEN;

	if (w.compare(PR_ENDIF) == 0)
		return TK_PR_ENDIF;

	if (w.compare(PR_LOOP) == 0)
		return TK_PR_LOOP;

	if (w.compare(PR_SCAN) == 0)
		return TK_PR_SCAN;

	if (w.compare(PR_SCLN) == 0)
		return TK_PR_SCLN;

	if (w.compare(PR_PRINT) == 0)
		return TK_PR_PRINT;

	if (w.compare(PR_PRTLN) == 0)
		return TK_PR_PRTLN;

	if (w.compare(PR_DIV) == 0)
		return TK_PR_DIV;

	if (w.compare(PR_MOD) == 0)
		return TK_PR_MOD;
	else
		return TK_ID;
}

// -- verifica se caracter fecha um lexema
string Analex::verifTokenSimples(char c)
{
	switch (c) {
		case CE_APA:
			return TK_OP_DABPA;
		case CE_FPA:
			return TK_OP_DFEPA;
		case CE_ACO:
			return TK_OP_DABCO;
		case CE_FCO:
			return TK_OP_DFECO;
		case CE_ACH:
			return TK_OP_DABCH;
		case CE_FCH:
			return TK_OP_DFECH;
		case CE_POV:
			return TK_OP_PPOVI;
		case CE_DOP:
			return TK_OP_LTER2;
		case CE_CIF:
			return TK_OP_ICIFR;
		case CE_POT:
			return TK_OP_APOTE;
		case CE_VIR:
			return TK_OP_RVIRG;
		case CE_PON:
			return TK_OP_RPONT;
		case CE_INT:
			return TK_OP_LTER1;
		case CE_EXC:
			return TK_OP_LNEGA;
		case CE_COM:
			return TK_OP_ECOMP;
		case CE_ASP:
			return TK_OP_VASPA;
		case CE_GUA:
			return TK_OP_VGUAM;
		case CE_ADI:
			return TK_OP_ASOMA;
		case CE_SUB:
			return TK_OP_ASUBT;
		case CE_MUL:
			return TK_OP_AMULT;
		case CE_DIV:
			return TK_OP_ADIVI;
		case CE_MOD:
			return TK_OP_AMODU;
		case CE_ATR:
			return TK_OP_AATRB;
		case CE_MAI:
			return TK_OP_LMAIO;
		case CE_MEN:
			return TK_OP_LMENO;
		case CE_AND:
			return TK_OP_BAND;
		case CE_ORR:
			return TK_OP_BOR;
		default:
			erro("verifTokenSimples", "Caracter simples nao encontrado");
	}
	return CNUL;
}

/* --------------------------------
 * funcoes token
 * -------------------------------- */

// -- verifica token
// -- grava lexema e token
// -- reseta vetor lexema
// -- reseta estado (volta inicio automato)
void Analex::gravaToken(string nome)
{
	//define lexema
	sAnalex analex;
	analex.lexema = vchar2str(_vLexema);
	
	//define token
	if (nome == TK_ID)
		analex.token = verificaPalavra(analex.lexema);
	else 
		analex.token = nome;

	//grava lexema e token
	_analex.push_back(analex);

	//limpa buffer
	limpaToken();

	//reseta estado
	_estado = eInicial;
}

// -- adiciona caracter a lexema para analise de token
void Analex::incrementaToken(char c)
{
	_vLexema.push_back(c);
}

// -- reseta vetor contendo lexema de token gravado
void Analex::limpaToken()
{
	_vLexema.clear();
}

/* --------------------------------
 * funcoes de arquivo
 * -------------------------------- */
// -- grava lista de tokens e lexemas
void Analex::gravaLista()
{
	_lstlex << "Lexema\t| Token" << endl;
	_lstlex << "------\t+ ------" << endl;

	for (int i = 0; i < _analex.size(); i++)
		_lstlex << _analex[i].lexema << "\t| " << _analex[i].token << endl;
}


/* --------------------------------
 * funcoes auxiliares
 * -------------------------------- */
const char* Analex::str2char(string s)
{
	return s.c_str();
}

//string Analex::char2str(const char* c)
string Analex::char2str(char c[])
{
	return string(c);
}

string Analex::vchar2str(vChar vc)
{
	return string(vc.begin(), vc.end());
}

vChar Analex::str2vchar(string pgm)
{
	return vChar(pgm.begin(), pgm.end());
}


/* --------------------------------
 * getters / setters
 * -------------------------------- */
vAnalex Analex::getAnalex()
{
	return _analex;
}