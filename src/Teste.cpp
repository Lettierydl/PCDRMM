/*
 * Teste.cpp
 *
 *  Created on: 03/02/2015
 *      Author: Leo
 */

#include "Teste.h"
#include "Solucao.h"
#include <iostream>
#include "stdlib.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

Teste::Teste(Dados*d) {
	this->d = d;
}
Teste::~Teste() {
}

/* metodo que testa se a solucao é valida */
bool Teste::testarSolucao(Solucao * s) {
	bool valid = true;

	valid &= testeSeTodasAtividadesEstaoAlocadas(s);

	valid &= testeValorCustoETempo(s);

	valid &= testePrecedencias(s);

	if(s->tempo != d->D){
		cout <<"Tempo untrapassa a data do projeto T: "<<s->tempo <<", D: " << d->D << endl;
		valid &= false;
	}

	if (valid) {
		//cout << "Solucao viavel" << endl;
	}
	return valid;
}

bool Teste::testePrecedencias(Solucao * s) {
	bool valid = true;
	for (int j = 0; j < d->j; j++) {
		list<int>::iterator pre = d->H[j].begin();
		for (; pre != d->H[j].end(); pre++) {
			if (s->Ti[j] < (s->Ti[*pre] + d->d[*pre][s->M[*pre]])) {
				valid = false;

				cerr << endl << "Erro de Precedencia" << endl;
				cerr << *pre << " - " << j << endl;
				cerr << s->Ti[*pre] << " - " << s->Ti[j] << endl;
			}
		}
	}
	return valid;
}

bool Teste::testeValorCustoETempo(Solucao * s) {

	vector<int> maiorUtilizacao(d->tipos, 0);

	for (int t = 0; t < s->tempo; t++) {
		for (int k = 0; k < d->tipos; k++) {

			int utilizacaoK = 0;
			for (int j = 1; j < d->j; j++) {
				if (s->Ti[j] <= t && s->Ti[j] + d->d[j][s->M[j]] >= t) { //j esta ativa no tempo t
					utilizacaoK += d->r[j][s->M[j]][k];
				}
			}
			if (maiorUtilizacao[k] < utilizacaoK) {
				maiorUtilizacao[k] = utilizacaoK;
			}
			if(maiorUtilizacao[k] != s->tr[t][k]){
			//	cerr <<"utilizacao contabilizada errada no tempo "<<t <<" do recurso " << k<< endl;
			//	cerr <<"valor correto "<<maiorUtilizacao[k] <<", valor apresentado " << s->tr[t][k]<< endl;
			}
		}
	}

	float custo = 0;
	for (int k = 0; k < d->tipos; k++) {
		custo += maiorUtilizacao[k] * d->custo_recurso[k];
	}

	if (custo != s->custo) {
		cerr << "Custo Errado" << endl << "Custo real: " << custo
				<< ", custo apresentado: " << s->custo << endl;
	}

	int tempo = s->Ti[0] + d->d[0][s->M[0]];
	for (int j = 1; j < d->j; j++) {
		if (tempo < s->Ti[j] + d->d[j][s->M[j]]) {
			tempo = s->Ti[j] + d->d[j][s->M[j]];
		}
	}
	if (tempo != s->tempo) {
		cerr << "Tempo Errado" << endl << "Tempo real: " << tempo
				<< ", tempo apresentado: " << s->tempo << endl;
	}

	return tempo == s->tempo && custo == s->custo;
}

bool Teste::testeSeTodasAtividadesEstaoAlocadas(Solucao * s) {
	bool valid = true;
	for (int j = 0; j < d->j; j++) {
		if (s->D[j] != d->d[j][s->M[j]] || s->Ti[j] > s->tempo
				|| !s->alocadas[j] || s->M[j] < 0 || s->M[j] >= d->M[j]) {
			cerr << "Atividade nao alocada " << j << endl;
			valid = false;
		}
	}
	return valid;
}
