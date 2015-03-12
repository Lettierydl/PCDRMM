/*
 * Solucao.cpp
 *
 *  Created on: 02/12/2014
 *      Author: Leo
 */

#include "Solucao.h"
#include "Dados.h"
#include <iostream>
#include "stdlib.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>

using namespace std;

Solucao::Solucao(Dados * d) {
	this->d = d;
	this->custo = 0;
	this->tempo = 0;

	this->demanda = new int[d->tipos];

	for (int i = 0; i < d->tipos; i++) {
		demanda[i] = 0;
	}

	this->D = vector<int>(d->j);
	this->M = vector<int>(d->j);
	this->Ti = vector<int>(d->j);
	this->alocadas = vector<bool>(d->j,false);

	int tempoMaximoEsperado = d->D*30;// melhorar isso
	this->tr.resize(tempoMaximoEsperado);
	for(int t = 0; t< tempoMaximoEsperado; t++){
		tr[t] = vector<int>(d->tipos);
	}
}

void Solucao::atualizarDemanda(int ti, int tf){
	for(int k = 0; k < d->tipos;k++){
		for(int t = ti;t <= tf;t++){
			if(demanda[k] < tr[t][k]){
				demanda[k] = tr[t][k];// acrescentando a demanda
			}
		}
	}
}

void Solucao::alocarAtividade(int j, int ti, int mj) {

	if ( this->alocadas[j] ) { // verificar se atividade ja esta alocada
		int ti_old = Ti[j];// tempo de inicio da atividade ja alocada
		int m_old = M[j];// modo de execucao antigo de j
		int tf_old = ti_old + d->d[j][m_old];// tempo de fim da atividade ja alocada


		bool verificar_demanda = false; // flag para saber se ha necessario de verificar alteracao na demanda do recurso
		for(int t = ti_old;t <= tf_old;t++){
			for(int k = 0; k < d->tipos;k++){
				if(d->r[j][m_old][k] != 0){
					verificar_demanda = tr[t][k] == demanda[k];//obss....s
					tr[t][k] -= d->r[j][m_old][k];//diminuindo a utilizacao do recurso daquele tempo
				}
			}
		}

		if(verificar_demanda){// atualizando a demanda pela retirada da atividade
			this->atualizarDemanda(ti_old, tf_old);
		}
	}// ao fim desse if a atividade ja foi retirarda e os valores de demanda da solucao estao atualizados

	Ti[j] = ti;
	M[j] = mj;
	D[j] = d->d[j][mj];
	alocadas[j] = true;

	int tf = ti + D[j];// tempo de termino da atividade

	bool verificar_demanda = false;
	for(int t = ti;t <= tf;t++){
		for(int k = 0; k < d->tipos; k++){
			if(d->r[j][mj][k] != 0){
				verificar_demanda = (tr[t][k]+ d->r[j][mj][k]) > demanda[k];// flag para saber se e necessario verificar alteracao na demanda do recurso
				tr[t][k] += d->r[j][mj][k];//acrescentando a utilizacao do recurso daquele tempo
			}
		}
	}

	if(verificar_demanda){// atualizando a demanda pela insercao da atividade
		this->atualizarDemanda(ti, tf);
	}

}

int Solucao::verificarTempoInicioCedo(int j){
	int possivel = 0;
	for(list<int>::iterator p = d->H[j].begin(); p != d->H[j].end() ;p++ ){
		if(alocadas[*p]){//atividades que ja foram alocadas
			if( (Ti[*p]+D[*p]) > possivel ){
				possivel = (Ti[*p]+D[*p]);
			}
		}
	}
	return possivel;
}

int Solucao::verificarMelhorModoPeloTempo(int j){
	int modo = 0;
	int duracaoMin = d->d[j][modo];
	for(int i = 0; i< d->M[j]; i++){
		if(d->d[j][i] < duracaoMin){
			duracaoMin = d->d[j][i];
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaUtilizacao(int j){
	int modo = 0;

	float utilizacao = 0;
	for (int k = 0; k < d->tipos; ++k) {// mudar isso por um array que calcula esse custo apena uma vez e guarda
		utilizacao += d->r[j][modo][k] * d->custo_recurso[k];
	}

	int utilizacaoMin = utilizacao;
	for(int i = 0; i< d->M[j]; i++){

		utilizacao = 0;
		for (int k = 0; k < d->tipos; ++k) {// mudar isso por um array que calcula esse custo apena uma vez e guarda
			utilizacao += d->r[j][i][k] * d->custo_recurso[k];
		}

		if(utilizacao < utilizacaoMin){
			utilizacaoMin = utilizacao;
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaMenorQuantidadeUtilizada(int j){
	int modo = 0;

	float quantidade = 0;
	for (int k = 0; k < d->tipos; ++k) {// mudar isso por um array que calcula esse custo apena uma vez e guarda
		quantidade += d->r[j][modo][k];
	}

	int quantidadeMin = quantidade;
	for(int i = 0; i< d->M[j]; i++){

		quantidade = 0;
		for (int k = 0; k < d->tipos; ++k) {// mudar isso por um array que calcula esse custo apena uma vez e guarda
			quantidade += d->r[j][i][k];
		}

		if(quantidade <quantidadeMin){
			quantidadeMin = quantidade;
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaMenorQuantidadeUtilizadaDeK(int j, int k){
	int modo = 0;

	int quantidade = 0;
	quantidade += d->r[j][modo][k];

	int quantidadeMin = quantidade;
	for(int i = 0; i< d->M[j]; i++){

		quantidade = d->r[j][i][k];

		if(quantidade < quantidadeMin){
			quantidadeMin = quantidade;
			modo = i;
		}
	}
	return modo;
}

int Solucao::verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(int j, int k){
	int modo = 0;

	int quantidade = 0;
	quantidade += d->r[j][modo][k];

	int quantidadeMax = quantidade;
	for(int i = 0; i < d->M[j]; i++){

		quantidade = d->r[j][i][k];

		if(quantidade > quantidadeMax){
			quantidadeMax = quantidade;
			modo = i;
		}
	}
	return modo;
}



void Solucao::iniciarSolucaoComModosAleatorios(){
	for(int j = 0 ; j < d->j ; j++){
		int tini = verificarTempoInicioCedo(j);

		srand( (unsigned)time(NULL) );

		int modo = rand() % d->M[j];



		alocarAtividade(j, tini, modo);
	}
}

void Solucao::iniciarSolucaoComMelhorMakespan(){
	for(int j = 0 ; j < d->j ; j++){
		int tini = verificarTempoInicioCedo(j);
		int modo = verificarMelhorModoPeloTempo(j);
		alocarAtividade(j, tini, modo);
	}
}

void Solucao::iniciarSolucaoComMelhorCusto(){
	int tfj = 0;// tempo de termino da ultima atividade
	for(int j = 0 ; j < d->j ; j++){
		int modo = verificarMelhorModoPelaUtilizacao(j);
		int tini = tfj;
		tfj = tfj + d->d[j][modo];

		alocarAtividade(j, tini, modo);
	}
}

void Solucao::iniciarSolucaoComMenorUtilizacao(){
	for(int j = 0 ; j < d->j ; j++){
		int modo = verificarMelhorModoPelaMenorQuantidadeUtilizada(j);
		int tini = verificarTempoInicioCedo(j);

		alocarAtividade(j, tini, modo);
	}
}

void Solucao::iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos(){
	vector<int> recs (d->tipos);
	//iniciar as primeiras atividades
	alocarAtividade(0, 0, 0);

	int modo1 = verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(1, 1);
	alocarAtividade(1, 0, modo1);

	int melhorAtualK = 0;
	int menorUtilizacaoDeRec = 1000;// intMax
	for(int k=0; k < d->tipos ; k++){

		recs[k] += d->r[1][modo1][k];
		cout << recs[k] << "|";
		if(recs[k] > menorUtilizacaoDeRec){
			menorUtilizacaoDeRec = recs[k];
			melhorAtualK = k;
		}

	}
	cout << endl << endl;

	for(int j = 0 ; j < d->j ; j++){

		int modo = verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(j, melhorAtualK);
		int tini = verificarTempoInicioCedo(j);

		alocarAtividade(j, tini, modo);
		for(int k=0; k < d->tipos ; k++){
			recs[k] += d->r[j][modo][k];
			cout <<recs[k] << "|";
			if(recs[k] > menorUtilizacaoDeRec){
				menorUtilizacaoDeRec = recs[k];
				melhorAtualK = k;
			}
		}
		cout << endl << endl;
	}

}




vector<int> Solucao::ordenarRecursosPorPrecos(){
	vector<int> recs (d->tipos);

	vector <float> custosOrdenados(d->custo_recurso);

	for(int k = 0 ;k < d->tipos ;k++){
		//custosOrdenados[k] =
		cout << custosOrdenados[k]<<"|";
	}cout <<endl << endl;


	for(int k = 0 ;k < d->tipos ;k++){
		int pk = 0;
		float custoK = d->custo_recurso[k];
		for(  ; pk < d->tipos ;pk++){
			if(custoK == custosOrdenados[pk]) break;
		}
		recs[pk] = k;
	}
	return recs;
}


float Solucao::calcular_custo() {
	custo = 0;
	for (int k = 0; k < d->tipos; ++k) {
		custo += d->custo_recurso[k] * demanda[k];
	}
	return custo;
}

int Solucao::calcular_tempo() {
	tempo = 0;
	for (int j = 0; j < d->j; ++j) {
		if((Ti[j]+D[j]) > tempo){
			tempo = (D[j]+Ti[j]);
		}
	}
	return tempo;
}

void Solucao::print() {
	cout << "j\t" << "Ti\t" << "M\t" << "D\t" << endl;
	for (int j = 0; j < d->j; ++j) {
		cout << j << "\t" << Ti[j] << "\t" << M[j] << "\t" << D[j]
				<<  endl;
	}

	cout << "Demanda: ";
	for (int k = 0; k< d->tipos; k++){
		cout << this->demanda[k] << " ";
	}cout << endl;

	cout << "C:" << calcular_custo() << "\tT:" << this->calcular_tempo() << endl;
}

Solucao::~Solucao() {
	free(demanda);
	free(d);
}
