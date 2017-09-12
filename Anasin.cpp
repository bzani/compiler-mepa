/* ====================================================== *
* Anasin.cpp                                              *
* ======================================================= */

#include "Anasin.h"

/* --------------------------------
 * construtor/destrutor
 * -------------------------------- */
Anasin::Anasin(vAnalex analex)
{
	//variaveis do Anasin
	_pos = 0;
	_id = 0;
	_analex = analex;
	_token = getToken();
	_errosem = 0;
	_errosin = 0;

	//logs de saida
	_logsin.open("log_anasin.txt");
	_logsem.open("log_anasem.txt");
	_lstsem.open("lista_ts.txt");
	_codmepa.open("mepa.txt");
	_logsin << "-- Inicio Log Anasin --" << endl;
	_logsem << "-- Inicio Log Anasem --" << endl;
}

Anasin::~Anasin()
{
	//destrutor
	_logsin << "-- Fim Log Anasin --" << endl;
	_logsem << "-- Fim Log Anasem --" << endl;
	_logsin.close();
	_logsem.close();
	_lstsem.close();
	_codmepa.close();
}

/* --------------------------------
 * funcao principal
 * -------------------------------- */
void Anasin::executa()
{
	while (true)
	{
		PG();
		aceitaToken(TK_FIM, "Esperado token de finalizacao.");
		break;
	}
	if (!_errosin)
		cout << "-> Analise sintatica realizada com sucesso." << endl;
	else
		cout << "-> Analise sintatica finalizada com erro(s)." << endl;
	
	if (_errosem == 0)
		cout << "-> Analise semantica realizada com sucesso." << endl;
	else if (_errosem == 1)
		cout << "-> Analise semantica finalizada com erro(s)." << endl;
	else 
		cout << "-> Analise semantica finalizada com aviso(s)." << endl;

	TS_lista();
	cout << "-> TS gerada com sucesso." << endl;

	MP_lista();
	cout << "-> Codigo MEPA gerado com sucesso." << endl;
}
	
/* --------------------------------
 * funcoes sintaticas
 * -------------------------------- */

// -- DECLARACOES
// ---------------------

void Anasin::PG() {
	LID();
}
void Anasin::LID() {
	DE();
	LIDf();
}
void Anasin::LIDf() {
	if (_token == TK_PR_VAR)
		LID();
}
void Anasin::DE() {
	if (_token == TK_PR_VAR) {
		DEV();
	}
	else {
		DEM();
	}
}
void Anasin::DEV() {
	aceitaToken(TK_PR_VAR, "Esperado 'var'");
	
	MP_insere(MP_AMEM, 1);

	// -- define variavel para insercao na TS
	sAnasem anasem;
	anasem.categoria = tscVar;
	anasem.tipo = EST();
	anasem.nome = VA();
	
	aceitaToken(TK_OP_PPOVI, "Esperado ';'");
	
	// -- se ok insere na TS
	TS_insere(anasem);
}
void Anasin::DEM() {
	aceitaToken(TK_PR_MAIN, "Esperado 'main'");
	
	MP_insere(MP_INPP);
	
	// -- insere funcao main() na TS
	sAnasem anasem;
	anasem.nome = "main";
	anasem.tipo = tstNull;
	anasem.categoria = tscFuncao;
	anasem.tamanho = 0;
	anasem.escopo = tseGlobal;
	TS_insere(anasem);

	aceitaToken(TK_OP_DABPA, "Esperado '('");
	aceitaToken(TK_OP_DFEPA, "Esperado ')'");
	BL();
	aceitaToken(TK_PR_END, "Esperado 'end'");
	
	int ts_qtdvar = TS_qtdvar();
	MP_insere(MP_DMEM, ts_qtdvar);
	MP_insere(MP_PARA);

	// -- programa terminou (end)
	// -- trata variaveis da TS
	TS_finaliza();
}
int Anasin::EST() {

	// -- guarda tipo da variavel atual
	string tk_tipo = _token;

	vString ltk;
	ltk.push_back(TK_PR_INT);
	ltk.push_back(TK_PR_REAL);
	ltk.push_back(TK_PR_CHAR);
	aceitaToken(ltk, "Esperado 'int', 'real' ou 'char'");

	// -- retorna tipo da variavel
	if (tk_tipo == TK_PR_INT)
		return tstInt;
	else if (tk_tipo == TK_PR_REAL)
		return tstReal;
	else if (tk_tipo == TK_PR_CHAR)
		return tstChar;
	return 0;
}

// -- COMANDOS
// ---------------------

void Anasin::BL() {
	LIC();
}
void Anasin::LIC() {
	if (_token == TK_PR_VAR		||
		_token == TK_ID			||
		_token == TK_PR_IF		||
		_token == TK_PR_WHILE	||
		_token == TK_PR_SCAN	|| 
		_token == TK_PR_SCLN	||
		_token == TK_PR_PRINT	||
		_token == TK_PR_PRTLN ) 
	{
		CO();
		LIC();
	}
}
void Anasin::CO() {
	if (_token == TK_PR_VAR)
		DEV();
	else if (_token == TK_ID)
		COA();
	else if (_token == TK_PR_IF)
		COS();
	else if (_token == TK_PR_WHILE)
		COR();
	else if (_token == TK_PR_SCAN || _token == TK_PR_SCLN)
		COL();
	else if (_token == TK_PR_PRINT || _token == TK_PR_PRTLN)
		COE();
	else
		errosin("CO", "Esperado 'var', 'id', 'if', 'while', 'scan', 'scanln', 'print' ou 'println'");
}
void Anasin::COA() {
	
	// -- verifica se variavel foi declarada
	string varNome = VA();
	int varId = TS_busca(varNome);
	if (varId < 0)
		errosem("COA", "Variavel nao declarada: ", varNome);
	
	// -- se sim, atualiza variavel (inicializada)
	sAnasem anasem = TS_consulta(varId);
	anasem.inicializada = true;
	TS_atualiza(varId, anasem);

	aceitaToken(TK_OP_AATRB, "Esperado '='");
	EX(anasem);
	
	MP_insere(MP_ARMZ, varNome);

	aceitaToken(TK_OP_PPOVI, "Esperado ';'");
}
void Anasin::COL() {
	vString ltk;
	ltk.push_back(TK_PR_SCAN);
	ltk.push_back(TK_PR_SCLN);
	aceitaToken(ltk, "Esperado 'scan' ou 'scanln'");

	MP_insere(MP_LEIT);

	aceitaToken(TK_OP_DABPA, "Esperado '('");
	string var = VA();

	MP_insere(MP_ARMZ, var);

	aceitaToken(TK_OP_DFEPA, "Esperado ')'");
	aceitaToken(TK_OP_PPOVI, "Esperado ';'");
}
void Anasin::COE() {
	vString ltk;
	ltk.push_back(TK_PR_PRINT);
	ltk.push_back(TK_PR_PRTLN);
	aceitaToken(ltk, "Esperado 'print' ou 'println'");
	aceitaToken(TK_OP_DABPA, "Esperado '('");

	// -- passa parametro vazio (nao utiliza na An.Sem.)
	sAnasem anasem;
	anasem.tipo = NNUL;
	EX(anasem);
	
	MP_insere(MP_IMPR);
	
	aceitaToken(TK_OP_DFEPA, "Esperado ')'");
	aceitaToken(TK_OP_PPOVI, "Esperado ';'");
}
void Anasin::COS() {
	aceitaToken(TK_PR_IF, "Esperado 'if'");

	// -- passa parametro vazio (nao utiliza na An.Sem.)
	sAnasem anasem;
	anasem.tipo = NNUL;
	EX(anasem);

	string rotif = MP_geraRotulo();
	MP_insere(MP_CMIG);
	MP_insere(MP_DSVF, rotif);

	aceitaToken(TK_PR_THEN, "Esperado 'then'");
	BL();
	
	string rotel = MP_geraRotulo();
	MP_insere(MP_DSVS, rotel);
	
	COSf(rotif);

	MP_insereRotulo(rotel);
}
void Anasin::COSf(string rotulo) {
	if (_token == TK_PR_ELSE) {
		aceitaToken(TK_PR_ELSE, "Esperado 'else'");
		
		MP_insereRotulo(rotulo);
		
		BL();
		aceitaToken(TK_PR_ENDIF, "Esperado 'end-if'");
	}
	else {
		aceitaToken(TK_PR_ENDIF, "Esperado 'end-if'");
	}
}
void Anasin::COR() {
	aceitaToken(TK_PR_WHILE, "Esperado 'while'");

	string rotwh = MP_geraRotulo();
	MP_insereRotulo(rotwh);
	
	sAnasem anasem;
	anasem.tipo = NNUL;
	EX(anasem);

	string rotlo = MP_geraRotulo();
	MP_insere(MP_CMIG);
	MP_insere(MP_DSVF, rotlo);

	aceitaToken(TK_PR_DO, "Esperado 'do'");
	BL();

	MP_insere(MP_DSVS, rotwh);

	aceitaToken(TK_PR_LOOP, "Esperado 'loop'");

	MP_insereRotulo(rotlo);
}


// -- EXPRESSOES
// ---------------------

void Anasin::EX(sAnasem anasem) {

	EXM(anasem);
	EXf(anasem);
}
void Anasin::EXf(sAnasem anasem) {
	if (_token == TK_OP_ASOMA || 
		_token == TK_OP_ASUBT ) 
	{
		OPS();
		EX(anasem);

		if (_token == TK_OP_ASOMA)
			MP_insere(MP_SOMA);
		else
			MP_insere(MP_SUBT);
	}
}
void Anasin::OPS() {
	vString ltk;
	ltk.push_back(TK_OP_ASOMA);
	ltk.push_back(TK_OP_ASUBT);
	aceitaToken(ltk, "Esperado '+' ou '-'");
}
void Anasin::EXM(sAnasem anasem) {
	EXS(anasem);
	EXMf(anasem);
}
void Anasin::EXMf(sAnasem anasem) {
	if (_token == TK_OP_AMULT ||
		_token == TK_OP_ADIVI ||
		_token == TK_PR_MOD	  ||
		_token == TK_PR_DIV)
	{
		OPM();
		EXM(anasem);

		if (_token == TK_OP_AMULT)
			MP_insere(MP_MULT);
		else if (_token == TK_OP_ADIVI || _token == TK_PR_DIV)
			MP_insere(MP_DIVI);
	}
}
void Anasin::OPM() {
	vString ltk;
	ltk.push_back(TK_OP_AMULT);
	ltk.push_back(TK_OP_ADIVI);
	ltk.push_back(TK_PR_MOD);
	ltk.push_back(TK_PR_DIV);
	aceitaToken(ltk, "Esperado '*', '/', 'mod' ou 'div'");
}
void Anasin::EXS(sAnasem anasem) {
	
	if (_token == TK_OP_DABPA) {
		aceitaToken(TK_OP_DABPA, "Esperado '('");
		EX(anasem);
		aceitaToken(TK_OP_DFEPA, "Esperado ')'");
	}
	else if (_token == TK_ID) {
		string var = VA(anasem);
		MP_insere(MP_CRVL, var);
	}
	else {
		LI(anasem);
	}
}
void Anasin::LI(sAnasem anasem) {
	
	// verifica se:
	// -- esta verificando tipo da operacao (tipo != null)
	// -- literal possui mesmo tipo que variavel da operacao
	string tok = getToken();
	if (!((tok == TK_NUMINT && anasem.tipo == tstInt) ||
		(tok == TK_NUMREAL && anasem.tipo == tstReal) ||
		(tok == TK_CARACT && anasem.tipo == tstChar) ||
		(tok == TK_STRING && anasem.tipo == tstString)) &&
		(anasem.tipo != NNUL))
		errosem("LI", "Tipo de literal difere do tipo da operacao: ", tok, anasem.tipo);
	
	string lit = getLexema();
	MP_insere(MP_CRCT, lit);

	vString ltk;
	ltk.push_back(TK_NUMINT);
	ltk.push_back(TK_NUMREAL);
	ltk.push_back(TK_CARACT);
	ltk.push_back(TK_STRING);
	aceitaToken(ltk, "Esperado inteiro, real, caractere ou string");
}
string Anasin::VA() {
	string tk_id = getLexema();

	aceitaToken(TK_ID, "Esperado id");
	
	return tk_id;
}

string Anasin::VA(sAnasem anasem) {
	
	// verifica se:
	// -- variavel foi declarada
	// -- variavel foi inicializada
	// -- esta verificando tipo da operacao (tipo != null)
	// -- variavel possui mesmo tipo que variavel da operacao
	
	string varNome = getLexema();
	int varId = TS_busca(varNome);
	if (varId < 0)
		errosem("VA", "Variavel nao declarada: ", varNome);
	sAnasem as = TS_consulta(varId);
	if (as.inicializada == false)
		errosem("VA", "Variavel nao inicializada: ", as.nome);
	if ((anasem.tipo != as.tipo) && (anasem.tipo != NNUL))
		errosem("VA", "Tipo de variavel difere do tipo da operacao: ", as.tipo, anasem.tipo);
	
	aceitaToken(TK_ID, "Esperado id");

	return varNome;
}

/* --------------------------------
 * funcoes automato
 * -------------------------------- */
void Anasin::proxToken()
{
	_pos++;
	_token = getToken();
}

string Anasin::getToken() {
	return _analex[_pos].token;
}

string Anasin::getLexema() {
	return _analex[_pos].lexema;
}

void Anasin::aceitaToken(string token, string msg)
{
	_logsin << "[lexema] " << getLexema() << "\t";
	if (_token == token)
		_logsin << "[Token] " << _token << " == " << token << endl;
	else {
		_logsin << "[Token] " << _token << " <> " << token << "\t**ERRO**" << endl;
		errosin("aceitaToken", msg);
	}

	if (token != TK_FIM)
		proxToken();
}

void Anasin::aceitaToken(vString ltoken, string msg)
{
	_logsin << "[lexema] " << getLexema() << "\t";
	bool achou = false;
	for (int i = 0; i < ltoken.size(); i++) {
		if (_token == ltoken[i]) {
			achou = true;
			_logsin << "[lTokn] " << _token << " == " << ltoken[i] << endl;
		}
	}
	if (!achou) {
		_logsin << "[lTokn] " << _token << " <> ";
		for (int i = 0; i < ltoken.size(); i++) {
			_logsin << ltoken[i] << ' ';
		}
		_logsin << "\t**ERRO**" << endl;
		errosin("aceitaToken", msg);
	}
	proxToken();
}


/* --------------------------------
* funcoes semantica
* -------------------------------- */

void Anasin::TS_insere(sAnasem anasem) {
	
	// verifica se variavel ja foi declarada
	if (TS_busca(anasem.nome) >= 0)
		errosem("TS_insere", "Variavel ja declarada: ", anasem.nome);

	// senao inseren
	anasem.id = _id;
	anasem.tamanho = 1;
	anasem.escopo = tseGlobal;
	anasem.dados = CNUL;
	anasem.inicializada = false;
	anasem.removida = false;
	_anasem.push_back(anasem);
	_id++;
}

void Anasin::TS_atualiza(int id, sAnasem anasem) {
	_anasem[id] = anasem;
}

void Anasin::TS_remove(int id) {
	_anasem[id].removida = true;
}

sAnasem Anasin::TS_consulta(int id) {
	return _anasem[id];
}
 
int Anasin::TS_busca(string nome) {
	for (int i = 0; i < _id; i++) {
		if (_anasem[i].nome == nome &&
			_anasem[i].removida == false)
			return _anasem[i].id;
	}
	return NNUL;
}

int Anasin::TS_tipo(int id) {
	return _anasem[id].tipo;
}

void Anasin::TS_lista() {

	// -- cabecalho
	_lstsem << "id\tnome\ttipo\tcateg.\ttam.\tescopo\tdados\tremov." << endl;

	for (int i = 0; i < _anasem.size(); i++) {

		// -- id
		_lstsem << _anasem[i].id;
		
		// -- nome
		_lstsem << "\t" << _anasem[i].nome;
		
		// -- tipo
		int tipo = _anasem[i].tipo;
		switch (tipo) {
			case tstNull:
				_lstsem << "\t-";
				break;
			case tstInt:
				_lstsem << "\tInt";
				break;
			case tstReal:
				_lstsem << "\tReal";
				break;
			case tstChar:
				_lstsem << "\tChar";
				break;
			default:
				_lstsem << "\t" << tipo;
				break;
		}
		
		// -- categoria
		int cat = _anasem[i].categoria;
		switch (cat) {
			case tscNull:
				_lstsem << "\t-";
				break;
			case tscVar:
				_lstsem << "\tVar";
				break;
			case tscConst:
				_lstsem << "\tConst";
				break;
			case tscFuncao:
				_lstsem << "\tFuncao";
				break;

			case tscTipo:
				_lstsem << "\tTipo";
				break;
			default:
				_lstsem << "\t" << cat;
				break;
		}
		
		// -- tamanho
		_lstsem << "\t" << _anasem[i].tamanho;
		
		// -- escopo
		int escp = _anasem[i].escopo;
		switch (escp) {
			case tseGlobal:
				_lstsem << "\tGlobal";
				break;
			case tseLocal:
				_lstsem << "\tLocal";
				break;
			default:
				_lstsem << "\t" << escp;
				break;
		}

		// -- dados
		if (_anasem[i].dados == CNUL)
			_lstsem << "\tnull";
		else	
			_lstsem << "\t" << _anasem[i].dados;
		
		// -- removida
		if (_anasem[i].removida == false)
			_lstsem << "\tNAO";
		else 
			_lstsem << "\tSIM";
		
		_lstsem << endl;
	}
}

int Anasin::TS_qtdvar() {
	int qtd = 0;
	for (int i = 0; i < _anasem.size(); i++)
		if (_anasem[i].categoria == tscVar || _anasem[i].categoria == tscConst)
			qtd++;
	return qtd;
}

void Anasin::TS_finaliza() {
	for (int i = 0; i < _id; i++) {
		if ((_anasem[i].inicializada == false) && (i > 0))
			aviso("TS_Finaliza", "Variavel nao utilizada: ", _anasem[i].nome);
		_anasem[i].removida = true;
	}
}

vAnasem Anasin::get_TS() {
	return _anasem;
}


/* ------------------------------------
 * MEPA
 *------------------------------------ */
void Anasin::MP_insere(string instr) {
	_mepa.push_back(instr);
}

void Anasin::MP_insere(string instr, int param) {
	string str = instr;
	str.append(" ");
	str.append(to_string(param));
	_mepa.push_back(str);
}

void Anasin::MP_insere(string instr, string param) {
	string str = instr;
	str.append(" ");
	str.append(param);
	_mepa.push_back(str);
}

void Anasin::MP_insereRotulo(string nome) {
	string str = nome;
	str.append(": ");
	str.append(MP_NADA);
	_mepa.push_back(str);
}

void Anasin::MP_lista() {
	for (int i = 0; i < _mepa.size(); i++)
		_codmepa << _mepa[i] << endl;
}

string Anasin::MP_geraRotulo() {
	string r = "R";
	r.append(to_string(++_rotulo));
	return r;
}

vString Anasin::get_MEPA() {
	return _mepa;
}


/* --------------------------------
 * outras funcoes
 * -------------------------------- */
void Anasin::errosin(string funcao, string msg)
{
	_logsin << "**ERRO: funcao " << funcao << ": " << msg << endl;
	_errosin = 1;
	encerra();
}

void Anasin::errosem(string funcao, string msg, string compl)
{
	_logsem << "**ERRO: funcao " << funcao << ": " << msg << compl << endl;
	_errosem = 1;
	encerra();
}

void Anasin::errosem(string funcao, string msg, int compl)
{
	_logsem << "**ERRO: funcao " << funcao << ": " << msg << compl << endl;
	_errosem = 1;
	encerra();
}

void Anasin::errosem(string funcao, string msg, int compl1, int compl2)
{
	_logsem << "**ERRO: funcao " << funcao << ": " << msg << compl1 << " ; " << compl2 << endl;
	_errosem = 1;
	encerra();
}

void Anasin::errosem(string funcao, string msg, string compl1, int compl2)
{
	_logsem << "**ERRO: funcao " << funcao << ": " << msg << compl1 << " ; " << compl2 << endl;
	_errosem = 1;
	encerra();
}

void Anasin::aviso(string funcao, string msg, string compl)
{
	_logsem << "**AVISO: funcao " << funcao << ": " << msg << compl << endl;
	_errosem = 2;
}

void Anasin::encerra() {
	//exit(1);
}

