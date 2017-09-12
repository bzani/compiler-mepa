/* ====================================================== *
* global.h                                                *
* ------------------------------------------------------- *
* Declaracoes globais                                     *
* :                                                       *
* Contempla a declaracao de bibliotecas, definicoes e     *
* demais configuracoes universais do compilador           *
* ======================================================= */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

/* ----------------------
 * declaracoes principais
 * ---------------------- */
#define BUFFER 256
#define NNUL -1
#define CNUL "\0"
#define QUEBR "\n\n---\n\n"

// -- vectors
typedef vector<char> vChar;
typedef vector<string> vString;

// -- analex
struct sAnalex {
	int id;
	string lexema;
	string token;
};
typedef vector<sAnalex> vAnalex;

// -- anasem
struct sAnasem {
	int id;
	string nome;
	int tipo;
	int categoria;
	int tamanho;
	int escopo;
	string dados;
	bool inicializada;
	bool removida;
};
typedef vector<sAnasem> vAnasem;

// -- ts
enum TSTipo {
	tstNull,
	tstInt,
	tstReal,
	tstChar,
	tstString
};

enum TSCategoria {
	tscNull,
	tscVar,
	tscConst,
	tscFuncao,
	tscTipo
};

enum TSEscopo {
	tseGlobal,
	tseLocal
};

/* ----------------------
 * estados
 * ---------------------- */
enum Estados {
	eInicial = 1,
	eNumero1,
	eNumero2,
	eReal,
	eSoma,
	eSubtr,
	eMult,
	eDiv,
	eMod,
	eIgual,
	eMenor,
	eMaior,
	eAnd,
	eOr,
	eID,
	eComentS,
	eComentA,
	eComentF,
	eCharA,
	eCharF,
	eStringA,
	eStringF
};
/* ----------------------
 * caracteres especiais
 * ---------------------- */
#define CE_APA '('
#define CE_FPA ')'
#define CE_ACO '['
#define CE_FCO ']'
#define CE_ACH '{'
#define CE_FCH '}'
#define CE_POV ';'
#define CE_DOP ':'
#define CE_CIF '$'
#define CE_ADI '+'
#define CE_SUB '-'
#define CE_MUL '*'
#define CE_DIV '/'
#define CE_MOD '%'
#define CE_POT '^'
#define CE_VIR ','
#define CE_PON '.'
#define CE_ATR '='
#define CE_INT '?'
#define CE_EXC '!'
#define CE_COM '~'
#define CE_MAI '>'
#define CE_MEN '<'
#define CE_AND '&'
#define CE_ORR '|'
#define CE_ASP '\"'
#define CE_GUA '\''
#define CE_UND '_'
#define CE_ESP ' '
#define CE_ENT '\n'
#define CE_TAB '\t'
#define CE_VAZ '\0'

/* ----------------------
 * lexemas
 * ---------------------- */

// - palavras reservadas
#define PR_VOID  "void"
#define PR_INT   "int"
#define PR_FLOAT "float"
#define PR_CHAR  "char"
#define PR_STRIN "string"
#define PR_BOOL  "bool"
#define PR_IF    "if"
#define PR_ELSE  "else"
#define PR_FOR   "for"
#define PR_WHILE "while"
#define PR_DO    "do"
#define PR_RETUR "return"
#define PR_BREAK "break"
#define PR_CONTI "continue"
#define PR_GOTO  "goto"
#define PR_TRUE  "true"
#define PR_FALSE "false"
#define PR_MAIN  "main"
#define PR_NULL  "null"
#define PR_VAR   "var"
#define PR_END   "end"
#define PR_REAL  "real"
#define PR_THEN  "then"
#define PR_ENDIF "end-if"
#define PR_LOOP  "loop"
#define PR_SCAN  "scan"
#define PR_SCLN  "scanln"
#define PR_PRINT "print"
#define PR_PRTLN "println"
#define PR_DIV   "div"
#define PR_MOD   "mod"

// - operadores aritmeticos
#define OP_ASOMA "+"
#define OP_ASUBT "-"
#define OP_AMULT "*"
#define OP_ADIVI "/"
#define OP_AMODU "%"
#define OP_APOTE "^"
#define OP_AATRB "="
#define OP_ASOIG "+="
#define OP_ASUIG "-="
#define OP_AMUIG "*="
#define OP_ADIIG "/="
#define OP_AMOID "%="
#define OP_AINCR "++"
#define OP_ADECR "--"
// - operadores decimais
#define OP_CVIRG ","
#define OP_CPONT "."
// - operadores logicos
#define OP_LTER1 "?"
#define OP_LTER2 ":"
#define OP_LNEGA "!"
#define OP_LAND  "&&"
#define OP_LOR   "||"
#define OP_LMENO "<"
#define OP_LMAIO ">"
#define OP_LDIF1 "<>"
#define OP_LIGUA "=="
#define OP_LDIF2 "!="
#define OP_LMEIG "<="
#define OP_LMAIG ">="
// - operadores de referencia
#define OP_RPONT "*"
#define OP_RENDE "&"
#define OP_RELE1 "."
#define OP_RELE2 "->"
#define OP_RCOMP "~"
// - operadores delimitadores
#define OP_DABPA "("
#define OP_DFEPA ")"
#define OP_DABCO "["
#define OP_DFECO "]"
#define OP_DABCH "{"
#define OP_DFECH "}"
// - operadores binarios
#define OP_BAND  "&"
#define OP_BOR   "|"
#define OP_BBIES "<<"
#define OP_BBIDI ">>"
// - operadores de pontuacao
#define OP_PPOVI ";"
// - operadores de identificacao
#define OP_ICIFR "$"
// - operadores de valor
#define OP_VASPA "\""
#define OP_VGUAM "\'"
// - operadores de comentario
#define OP_COME1 "//"
#define OP_COME2 "/*"
#define OP_COME3 "*/"

/* ----------------------
 * tokens
 * ---------------------- */

// - dinamicos
#define TK_NUMINT	"NUMINT  "		//numero inteiro
#define TK_NUMREAL  "NUM_REAL"		//numero real
#define TK_CARACT	"CARACTER"      //caracter
#define TK_STRING	"STRING  "      //string
#define TK_ID		"ID      "      //id
#define TK_FIM      "FIM     "      //fim do programa

// - operadores                       
#define TK_OP_AATRB "OP_AATRB"      //arit. de atribuicao (=)
#define TK_OP_ADECR "OP_ADECR"      //arit. de decremento (--)
#define TK_OP_ADIIG "OP_ADIIG"      //arit. de divisao/igualdade (/=)
#define TK_OP_ADIVI "OP_ADIVI"      //arit. de divisao (/)
#define TK_OP_AINCR "OP_AINCR"      //arit. de incremento (++)
#define TK_OP_AMODU "OP_AMODU"      //arit. de mod (%)
#define TK_OP_AMOID "OP_AMOID"      //arit. de mod/igualdade (%=)
#define TK_OP_AMUIG "OP_AMUIG"      //arit. de multiplicacao/igualdade (*=)
#define TK_OP_AMULT "OP_AMULT"      //arit. de multiplicacao (*)
#define TK_OP_APOTE "OP_APOTE"      //arit. de potencia (^)
#define TK_OP_ASOIG "OP_ASOIG"      //arit. de soma/igualdade (+=)
#define TK_OP_ASOMA "OP_ASOMA"      //arit. de soma (+)
#define TK_OP_ASUBT "OP_ASUBT"      //arit. de subtracao (-)
#define TK_OP_ASUIG "OP_ASUIG"      //arit. de subtracao/igualdade (-=)
#define TK_OP_BAND  "OP_BAND "      //binario AND (&)
#define TK_OP_BBIDI "OP_BBIDI"      //binario bit a direita (>>)
#define TK_OP_BBIES "OP_BBIES"      //binario bit a esquerda (<<)
#define TK_OP_BOR   "OP_BOR  "      //binario OR (|)
#define TK_OP_DABCH "OP_DABCH"      //delim. abre chaves ({)
#define TK_OP_DABCO "OP_DABCO"      //delim. abre colchetes ([)
#define TK_OP_DABPA "OP_DABPA"      //delim. abre parenteses (()
#define TK_OP_DFECH "OP_DFECH"      //delim. fecha chaves (})
#define TK_OP_DFECO "OP_DFECO"      //delim. fecha colchetes (])
#define TK_OP_DFEPA "OP_DFEPA"      //delim. fecha parenteses ())
#define TK_OP_ECOMP "OP_ECOMP"      //de ref. complemento (~)
#define TK_OP_EELE1 "OP_EELE1"      //de ref. elemento de (.)
#define TK_OP_EELE2 "OP_EELE2"      //de ref. elemento de (->)
#define TK_OP_EENDE "OP_EENDE"      //de ref. endereco de variavel (&)
#define TK_OP_EPONT "OP_EPONT"      //de ref. ponteiro (*)
#define TK_OP_ICIFR "OP_ICIFR"      //de identificacao ($)
#define TK_OP_LAND  "OP_LAND "      //logico AND (&&)
#define TK_OP_LDIFE "OP_LDIFE"      //logico diferente de (!=)
#define TK_OP_LIGUA "OP_LIGUA"      //logico igual a (==)
#define TK_OP_LMAIG "OP_LMAIG"      //logico maior ou igual a (>=)
#define TK_OP_LMAIO "OP_LMAIO"      //logico maior que (>)
#define TK_OP_LMEIG "OP_LMEIG"      //logico menor ou igual a (<=)
#define TK_OP_LMENO "OP_LMENO"      //logico menor que (<)
#define TK_OP_LNEGA "OP_LNEGA"      //logico de negacao (!)
#define TK_OP_LOR   "OP_LOR  "      //logico OR (||)
#define TK_OP_LTER1 "OP_LTER1"      //logico ternario (?)
#define TK_OP_LTER2 "OP_LTER2"      //logico ternario (:)
#define TK_OP_PPOVI "OP_PPOVI"      //de pontuacao ponto-e-virgula (;)
#define TK_OP_RPONT "OP_RPONT"      //decimal (.)
#define TK_OP_RVIRG "OP_RVIRG"      //decimal (,)
#define TK_OP_VASPA "OP_VASPA"      //aspas (")
#define TK_OP_VGUAM "OP_VGUAM"      //guampas (')

// - palavras reservadas              
#define TK_PR_BOOL  "PR_BOOL "      //bool
#define TK_PR_BREAK "PR_BREAK"      //break
#define TK_PR_CHAR  "PR_CHAR "      //char
#define TK_PR_CONTI "PR_CONTI"      //continue
#define TK_PR_DO    "PR_DO   "      //string
#define TK_PR_ELSE  "PR_ELSE "      //else
#define TK_PR_FALSE "PR_FALSE"      //false
#define TK_PR_FLOAT "PR_FLOAT"      //float
#define TK_PR_FOR   "PR_FOR  "      //for
#define TK_PR_GOTO  "PR_GOTO "      //goto
#define TK_PR_IF    "PR_IF   "      //if
#define TK_PR_INT   "PR_INT  "      //int
#define TK_PR_RETUR "PR_RETUR"      //return
#define TK_PR_TRUE  "PR_TRUE "      //true
#define TK_PR_VOID  "PR_VOID "      //void
#define TK_PR_WHILE "PR_WHILE"      //while
#define TK_PR_MAIN  "PR_MAIN "      //main
#define TK_PR_NULL  "PR_NULL "      //null
#define TK_PR_VAR   "PR_VAR  "      //var
#define TK_PR_END   "PR_END  "      //end
#define TK_PR_REAL  "PR_REAL "      //real
#define TK_PR_THEN  "PR_THEN "      //then
#define TK_PR_ENDIF "PR_ENDIF"      //endif
#define TK_PR_LOOP  "PR_LOOP "      //loop
#define TK_PR_SCAN  "PR_SCAN "      //scan
#define TK_PR_SCLN  "PR_SCLN "      //scanln
#define TK_PR_PRINT "PR_PRINT"      //print
#define TK_PR_PRTLN "PR_PRTLN"      //println
#define TK_PR_DIV   "PR_DIV  "      //div
#define TK_PR_MOD   "PR_MOD  "      //mod

/* ----------------------
 * MEPA
 * ---------------------- */
// - armazenamentos e e/s
#define MP_CRCT "CRCT"              //carrega constante
#define MP_CRVL "CRVL"              //carrega valor
#define MP_ARMZ "ARMZ"              //armazena valor
#define MP_LEIT "LEIT"              //leitura
#define MP_IMPR "IMPR"              //imprime
// - expressoes
#define MP_SOMA "SOMA"              //soma
#define MP_SUBT "SUBT"              //subtracao
#define MP_MULT "MULT"              //multiplicacao
#define MP_DIVI "DIVI"              //divisao
#define MP_MODI "MODI"              //resto da divisao
#define MP_CONJ "CONJ"              //conjuncao ( and )
#define MP_DISJ "DISJ"              //disjuncao (or)
#define MP_NEGA "NEGA"              //negacao (not)
#define MP_CMME "CMME"              //compara se menor
#define MP_CMMA "CMMA"              //compara se maior
#define MP_CMIG "CMIG"              //compara se igual
#define MP_CMDG "CMDG"              //compara se desigual
#define MP_CMAG "CMAG"              //compara se maior ou Igual
#define MP_CMEG "CMEG"              //compara se menor ou Igual
#define MP_INRV "INRV"              //inverte sinal
// - desvios
#define MP_DSVF "DSVF"              //desvia se falso
#define MP_DSVS "DSVS"              //desvia sempre
#define MP_NADA "NADA"              //nada
// - procedimentos
#define MP_CHPR "CHPR"              //chama procedimento
#define MP_ENPR "ENPR"              //entra no procedimento
#define MP_RTPR "RTPR"              //retorna do procedimento
// - programa e alocacao de memoria
#define MP_INPP "INPP"              //inicia programa Principal
#define MP_PARA "PARA"              //volta ao so
#define MP_AMEM "AMEM"              //aloca memoria
#define MP_DMEM "DMEM"              //desaloca memoria

/* ----------------------
 * fim
 * ---------------------- */

#endif	