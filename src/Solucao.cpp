/*
 * Solucao.cpp
 *
 *  Created on: 02/12/2014
 *      Author: Leo
 */

#include "Solucao.h"
#include "Dados.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace std;

Solucao::Solucao(Dados * d) {
	this->d = d;
	this->custo = 0;
	this->tempo = 0;

	/*PSO*/
	this->pbest = NULL;
	this->gbest = NULL;

	this->demanda = new int[d->tipos];

	for (int i = 0; i < d->tipos; i++) {
		demanda[i] = 0;
	}

	this->D = vector<int>(d->j);
	this->M = vector<int>(d->j);
	this->Ti = vector<int>(d->j);
	this->alocadas = vector<bool>(d->j, false);

	int tempoMaximoEsperado = d->D * 30; // melhorar isso
	this->tr.resize(tempoMaximoEsperado);
	for (int t = 0; t < tempoMaximoEsperado; t++) {
		tr[t] = vector<int>(d->tipos);
	}
}

Solucao::Solucao(Solucao *s) {
	this->d = s->d;
	this->custo = s->custo;
	this->tempo = s->tempo;

	this->demanda = new int[d->tipos];

	for (int i = 0; i < d->tipos; i++) {
		demanda[i] = s->demanda[i];
	}

	this->D = vector<int>(s->D);
	this->M = vector<int>(s->M);
	this->Ti = vector<int>(s->Ti);
	this->alocadas = vector<bool>(s->alocadas);

	int tempoMaximoEsperado = s->tr.size(); // melhorar isso
	this->tr.resize(tempoMaximoEsperado);
	for (int t = 0; t < tempoMaximoEsperado; t++) {
		tr[t] = vector<int>(s->tr[t]);
	}
}


/*PSO*/

/*os modos e as duracoes ja devem estar pronta
 *a fal nao deve conter redundancia (ciclos)*/
void Solucao::reconstruirSolucaoComFAL(){
	for(int j = 1; j<d->j;j++){
		Ti[j] = -1;
		alocadas[j]=false;
	}

	alocarAtividade(0,0,0);// nem precisa mais deixa pro precaucao
	for(int j = 1; j<d->j;j++){

		int qi = fal[j];
		if(Ti[qi] != -1){// predecessora nao foi alocada ainda
			alocarQi(qi);
		}// sair do if tem que ter alocado qi

		if(Ti[qi] != -1){cout <<"erro não alocou qi= "<< qi<< endl;}

		if(Ti[j] == -1){//ainda nao foi alocada, e qi ja foi alocada
			alocarAtividade(j,Ti[qi]+D[qi],M[j]);
		}
	}
	// todas foram alocadas
	// agora devese verificar se nao estrapolou tempo final
	// e aplicar metodos para corrigir a solucao

}

// metodo recursivo
//atividade 0 ja deve ser alocada
void Solucao::alocarQi(int qi){
	if(qi == 0){
		alocarAtividade(0,0,0);
		return;
	}else if(fal[qi] == 0){
		alocarAtividade(qi,0,M[qi]);
		return;
	}

	if(Ti[fal[qi]] != -1){// pode alocar qi
		alocarAtividade(qi,Ti[fal[qi]]+D[fal[qi]],M[qi]);
	}else{
		alocarQi(fal[qi]);
		alocarAtividade(qi,Ti[fal[qi]]+D[fal[qi]],M[qi]);
	}
}

void Solucao::atualizarDemanda(int ti, int tf) {
	for (int k = 0; k < d->tipos; k++) {
		for (int t = ti; t <= tf; t++) {
			if (demanda[k] < tr[t][k]) {
				demanda[k] = tr[t][k]; // acrescentando a demanda
			}
		}
	}
}

/*antes deve ser calculado o tempo*/
void Solucao::atualizarTodaDemanda() {
	for (int k = 0; k < d->tipos; k++) {
		demanda[k] = INT_MAX;
		for (int t = 0; t <= tempo; t++) {
			if (demanda[k] < tr[t][k]) {
				demanda[k] = tr[t][k]; // acrescentando a demanda
			}
		}
	}
}

void Solucao::alocarAtividade(int j, int ti, int mj) {

	desalocar(j); //desaloca a atividade caso ela ja esteja alocada

	Ti[j] = ti;
	M[j] = mj;
	D[j] = d->d[j][mj];
	alocadas[j] = true;

	int tf = ti + D[j]; // tempo de termino da atividade

	bool verificar_demanda = false;
	for (int t = ti; t <= tf; t++) {
		for (int k = 0; k < d->tipos; k++) {
			if (d->r[j][mj][k] != 0) {
				tr[t][k] += d->r[j][mj][k]; //acrescentando a utilizacao do recurso daquele tempo
			}
		}
	}
	this->atualizarDemanda(ti, tf);

}

void Solucao::realocarAtividadesApartirDaAtividade(int j) {
	int t = Ti[j] + D[j]; //termpo de fim de j
	for (int j_aux = 0; j_aux < d->j; j_aux++) {
		//verificar esse if, so funciona se voce retroceder o tempo da atividade j antes
		//se vc atrazar essa atividade, ele pode nao funcionar
		if (Ti[j_aux] >= t) {//qualquer alteracao de j so impacta nas atividades que iniciam depois dela
			int ti_new = verificarTempoInicioCedo(j_aux);//recalcula tempo de inicio de j_aux
			if (ti_new != Ti[j_aux]) {
				alocarAtividade(j_aux, ti_new, M[j_aux]);
			}
		}
	}
}

void Solucao::desalocar(int j) {
	if (this->alocadas[j]) { // verificar se atividade ja esta alocada
		int ti_old = Ti[j]; // tempo de inicio da atividade ja alocada
		int m_old = M[j]; // modo de execucao antigo de j
		int tf_old = ti_old + d->d[j][m_old]; // tempo de fim da atividade ja alocada

		bool verificar_demanda = false; // flag para saber se ha necessario de verificar alteracao na demanda do recurso
		for (int t = ti_old; t <= tf_old; t++) {
			for (int k = 0; k < d->tipos; k++) {
				if (d->r[j][m_old][k] != 0) {
					verificar_demanda = tr[t][k] == demanda[k]; //obss....s
					tr[t][k] -= d->r[j][m_old][k]; //diminuindo a utilizacao do recurso daquele tempo
				}
			}
		}
		this->atualizarDemanda(ti_old, tf_old);
	}
	Ti[j] = 0;
	M[j] = 0;
	D[j] = 0;
	alocadas[j] = false;
}

int Solucao::verificarTempoInicioCedo(int j) {
	int possivel = 0;
	for (list<int>::iterator p = d->H[j].begin(); p != d->H[j].end(); p++) {
		if (alocadas[*p]) { //atividades que ja foram alocadas
			if ((Ti[*p] + D[*p]) > possivel) {
				possivel = (Ti[*p] + D[*p]);
			}
		}
	}
	return possivel;
}


int Solucao::verificarTempoInicioTardeForaD(int j) { //fora do limite D
	int possivel = INT_MAX;

	if(j == d->j){
		return possivel;
	}
	for (list<int>::iterator s = d->S[j].begin(); s != d->S[j].end(); s++) {
		if (alocadas[*s]) { //atividades que ja foram alocadas
			if (Ti[*s] < possivel) {
				possivel = Ti[*s];
			}
		}
	}

	return possivel;
}

int Solucao::verificarTempoInicioTarde(int j) { //dentro da data limite D
	int possivel = d->D;

	if(j == d->j){
		return possivel;
	}
	for (list<int>::iterator s = d->S[j].begin(); s != d->S[j].end(); s++) {
		if (alocadas[*s]) { //atividades que ja foram alocadas
			if (Ti[*s] < possivel) {
				possivel = Ti[*s];
			}
		}
	}
	return possivel;
}

int Solucao::verificarMelhorModoPeloTempo(int j) {
	int modo = 0;
	int duracaoMin = d->d[j][modo];
	for (int i = 0; i < d->M[j]; i++) {
		if (d->d[j][i] < duracaoMin) {
			duracaoMin = d->d[j][i];
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaUtilizacao(int j) {
	int modo = 0;

	float utilizacao = 0;
	for (int k = 0; k < d->tipos; ++k) { // mudar isso por um array que calcula esse custo apena uma vez e guarda
		utilizacao += d->r[j][modo][k] * d->custo_recurso[k];
	}

	int utilizacaoMin = utilizacao;
	for (int i = 0; i < d->M[j]; i++) {

		utilizacao = 0;
		for (int k = 0; k < d->tipos; ++k) { // mudar isso por um array que calcula esse custo apena uma vez e guarda
			utilizacao += d->r[j][i][k] * d->custo_recurso[k];
		}

		if (utilizacao < utilizacaoMin) {
			utilizacaoMin = utilizacao;
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaMenorQuantidadeUtilizada(int j) {
	int modo = 0;

	float quantidade = 0;
	for (int k = 0; k < d->tipos; ++k) { // mudar isso por um array que calcula esse custo apena uma vez e guarda
		quantidade += d->r[j][modo][k];
	}

	int quantidadeMin = quantidade;
	for (int i = 0; i < d->M[j]; i++) {

		quantidade = 0;
		for (int k = 0; k < d->tipos; ++k) { // mudar isso por um array que calcula esse custo apena uma vez e guarda
			quantidade += d->r[j][i][k];
		}

		if (quantidade < quantidadeMin) {
			quantidadeMin = quantidade;
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaMenorQuantidadeUtilizadaDeK(int j, int k) {
	int modo = 0;

	int quantidade = 0;
	quantidade += d->r[j][modo][k];

	int quantidadeMin = quantidade;
	for (int i = 0; i < d->M[j]; i++) {

		quantidade = d->r[j][i][k];

		if (quantidade < quantidadeMin) {
			quantidadeMin = quantidade;
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(int j, int k) {
	int modo = 0;

	int quantidade = 0;
	quantidade += d->r[j][modo][k];

	int quantidadeMax = quantidade;
	for (int i = 0; i < d->M[j]; i++) {

		quantidade = d->r[j][i][k];

		if (quantidade > quantidadeMax) {
			quantidadeMax = quantidade;
			modo = i;
		}
	}
	return modo;
}

void Solucao::iniciarSolucaoComModosAleatorios() {
	//srand((unsigned) time(NULL));
	for (int j = 0; j < d->j; j++) {
		int tini = verificarTempoInicioCedo(j);

		int modo = rand() % d->M[j];

		alocarAtividade(j, tini, modo);
	}
	calcular_valores();
}

void Solucao::iniciarSolucaoComModosAleatoriosDentroDaDataLimite() {
	//srand((unsigned) time(NULL));
	for (int j = d->j - 1; j >= 0; j--) {
		int tft = verificarTempoInicioTarde(j);

		int modo = rand() % d->M[j];
		int bestModo = verificarMelhorModoPeloTempo(j);
		int ti = tft - d->d[j][modo];
		int tiBest = tft - d->d[j][bestModo];

		if (d->d[j][modo] <= tft) {
			alocarAtividade(j, tft - d->d[j][modo], modo);
		} else if (d->d[j][bestModo] <= tft) { // verificar se trocando o modo dar para alocar
			alocarAtividade(j, tft - d->d[j][bestModo], bestModo);
		} else { // realoca todos os sucessores tentando diminuir o tft
			list<int>::iterator su = d->S[j].begin();
			for (; su != d->S[j].end(); su++) {
				if (d->d[j][bestModo] > Ti[*su]) {
					int bestModoSu = verificarMelhorModoPeloTempo(*su);
					int tftSu = verificarTempoInicioTarde(*su);
					alocarAtividade(*su, tftSu - d->d[*su][bestModoSu],
							bestModoSu);
				}
			}
			int new_tft = verificarTempoInicioTarde(j);
			if (d->d[j][bestModo] <= new_tft) {
				alocarAtividade(j, new_tft - d->d[j][bestModo], bestModo);
			} else {
				alocarAtividade(j, 0, bestModo);

				for (int newJ = j; newJ < d->j - 1; newJ++) {
					list<int>::iterator su = d->S[newJ].begin();
					for (; su != d->S[newJ].end(); su++) {
						if (Ti[newJ] + D[newJ] > Ti[*su]) {
							int bestModoSu = verificarMelhorModoPeloTempo(*su);
							int tiSu = verificarTempoInicioCedo(*su);
							alocarAtividade(*su, tiSu, bestModoSu);
						}

					}
				}

			}

		}
	}
	calcular_valores();
}

void Solucao::iniciarSolucaoComMelhorMakespan() {
	for (int j = 0; j < d->j; j++) {
		int tini = verificarTempoInicioCedo(j);
		int modo = verificarMelhorModoPeloTempo(j);
		alocarAtividade(j, tini, modo);
	}
	calcular_valores();
}

void Solucao::iniciarSolucaoComMelhorCusto() {
	for (int j = 0; j < d->j; j++) {
		int modo = verificarMelhorModoPelaUtilizacao(j);
		int tini = verificarTempoInicioCedo(j);

		alocarAtividade(j, tini, modo);
	}
	calcular_valores();
}

void Solucao::iniciarSolucaoComMenorUtilizacao() {
	for (int j = 0; j < d->j; j++) {
		int modo = verificarMelhorModoPelaMenorQuantidadeUtilizada(j);
		int tini = verificarTempoInicioCedo(j);

		alocarAtividade(j, tini, modo);
	}
	calcular_valores();
}

void Solucao::iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos() {
	vector<int> recs(d->tipos);
	//iniciar as primeiras atividades
	alocarAtividade(0, 0, 0);

	int modo1 = verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(1, 1);
	alocarAtividade(1, 0, modo1);

	int melhorAtualK = 0;
	int menorUtilizacaoDeRec = 1000; // intMax
	for (int k = 0; k < d->tipos; k++) {

		recs[k] += d->r[1][modo1][k];
		if (recs[k] > menorUtilizacaoDeRec) {
			menorUtilizacaoDeRec = recs[k];
			melhorAtualK = k;
		}

	}
	//atividade 0 e 1 ja alocada
	for (int j = 2; j < d->j; j++) {

		int modo = verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(j,
				melhorAtualK);
		int tini = verificarTempoInicioCedo(j);

		alocarAtividade(j, tini, modo);
		for (int k = 0; k < d->tipos; k++) {
			recs[k] += d->r[j][modo][k];
			if (recs[k] > menorUtilizacaoDeRec) {
				menorUtilizacaoDeRec = recs[k];
				melhorAtualK = k;
			}
		}
	}
	calcular_valores();
}

/*Calcula de forma recursiva a menor folga ate a origem
(folga é a diferenta do tempo de inicio da atividade j ate o fim da sua predecessora)
O primeiro a ser passado será o primeiro da lista caminho (o ultimo sera 0)
*/
int Solucao::calcularFolgaDeCaminhoAteInicio(int j, list<int> *caminho){
	list<int>::iterator pre = d->H[j].begin();
	int min_folga = INT_MAX;
	int min_pred = 0;
	for (; pre != d->H[j].end(); pre++) {
		if(min_folga > Ti[j] - (Ti[*pre] + D[*pre]) ){
			min_folga = Ti[j] - (Ti[*pre] + D[*pre]);
			min_pred = *pre;
		}
	}

	caminho->push_back(min_pred);
	if(min_pred == 0){
		return 0;
	}else{
		return min_folga + calcularFolgaDeCaminhoAteInicio(min_pred ,caminho);
	}

}


/*Calcula de forma recursiva a menor folga ate o makespan
(folga é a diferenta do tempo de inicio da atividade j ate o inicio da sua sucessora)
O primeiro a ser passado será o primeiro da lista caminho (e o ultimo sera o makespan)
*/
int Solucao::calcularFolgaDeCaminhoAteFim(int j, list<int> *caminho){
	list<int>::iterator su = d->S[j].begin();
	int min_folga = INT_MAX;
	int min_su = d->j-1;
	for (; su != d->S[j].end(); su++) {
		if(min_folga > Ti[*su] - (Ti[j] + D[j]) ){
			min_folga = Ti[*su] - (Ti[j] + D[j]);
			min_su = *su;
		}
	}

	caminho->push_back(min_su);
	if(min_su == d->j-1){
		return 0;
	}else{
		return min_folga + calcularFolgaDeCaminhoAteFim(min_su ,caminho);
	}

}


list<int> Solucao::calcularCaminhoDoFinalAteInicio(Solucao *s) {
	int uAtividade = d->j - 1; //ultima atividade processada
	list<int> caminho;

	do {
		list<int>::iterator p = d->H[uAtividade].begin();
		int uAtividadePTerminar = 0; //ultima atividade predecessora de uAtividade a terminar

		for (; p != d->H[uAtividade].end(); p++) {
			int terminoUAT = s->Ti[uAtividadePTerminar]
					+ s->D[uAtividadePTerminar];
			int terminoP = s->Ti[*p] + s->D[*p];
			if (terminoUAT < terminoP
					|| (terminoUAT == terminoP
							&& s->D[uAtividadePTerminar] < s->D[*p])) {
				uAtividadePTerminar = *p;
			}
		}
		caminho.push_front(uAtividadePTerminar);
		uAtividade = uAtividadePTerminar;
	} while (uAtividade != 0);

	/*list<int>::iterator c = caminho.begin();
	 for (; c != caminho.end(); c++) {
	 cout << *c<<" - > ";
	 }cout << endl;*/
	return caminho;

}

void Solucao::calcular_valores() {
	calcular_custo();
	calcular_tempo();
}

float Solucao::calcular_custo() {
	this->custo = 0;
	for (int k = 0; k < d->tipos; ++k) {
		custo += d->custo_recurso[k] * demanda[k];
	}
	return custo;
}

int Solucao::calcular_tempo() {
	this->tempo = 0;
	for (int j = 0; j < d->j; ++j) {
		if ((Ti[j] + D[j]) > tempo) {
			tempo = (D[j] + Ti[j]);
		}
	}
	return this->tempo;
}
vector<int> Solucao::ordenarRecursosPorPrecos() {

	vector<int> recs(d->tipos);

	vector<float> custosOrdenados(d->custo_recurso);

	std::sort(custosOrdenados.begin(), custosOrdenados.end(),
			std::greater<float>());

	for (int k = 0; k < d->tipos; k++) {
		int pk = 0;
		float custoK = d->custo_recurso[k];
		for (; pk < d->tipos; pk++) {
			if (custoK == custosOrdenados[pk])
				break;
		}
		recs[pk] = k;
	}
	return recs;
}

void Solucao::print() {
	cout << "j\t" << "Ti\t" << "M\t" << "D\t" << endl;
	for (int j = 0; j < d->j; ++j) {
		cout << j << "\t" << Ti[j] << "\t" << M[j] << "\t" << D[j] << endl;
	}

	cout << "Demanda: ";
	for (int k = 0; k < d->tipos; k++) {
		cout << this->demanda[k] << " ";
	}
	cout << endl;

	cout << "C:" << calcular_custo() << "\tT:" << this->calcular_tempo()
			<< endl;

	/*
	 cout << "Custo por Recursos"<< endl;
	 for (int k = 0; k < d->tipos; k++) {

	 cout << d->custo_recurso[k] << " | ";
	 }
	 cout << endl;


	 for (int t = 0; t <= tempo; t++) {
	 cout << t << "\t";
	 for (int k = 0; k < d->tipos; k++) {

	 cout << tr[t][k] << "\t";
	 }
	 cout << endl;
	 }
	 */

}
Dados * d;
	int* demanda; // demanda dos recursos utilizado na solucao
	float custo;
	int tempo;

	vector<int> Ti; // tempo que iniciara a atividade j
	vector<int> M; // modo em que sera executada da atividade j
	vector<int> D; // o quanto durara a atividade j

	vector<bool> alocadas; // vetor que diz se a atividade j esta alocada
	vector<vector<int> > tr; // matriz de tipo de recurso k por tempo t , onde o conteúdo int é a quantidade utilizada de recurso to tipo k no tempo t.


	/* Atributos do PSO */
	vector<int> v; // velocidade
	vector<int> v_new; // nova velocidade
	//vector<int> x_new; // nova posicao (novo tempo de incio)
	Solucao * pbest;
	Solucao * gbest;

Solucao::~Solucao() {
	//delete(demanda);
	tempo = 0 ; custo = 0;

	Ti.erase(Ti.begin(), Ti.end());
	M.erase(M.begin(), M.end());
	D.erase(D.begin(), D.end());
	alocadas.clear();
	tr.erase(tr.begin(), tr.end());

	v.clear();
	v_new.clear();

}
