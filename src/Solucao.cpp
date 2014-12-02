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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include<time.h>

using namespace std;


Solucao::Solucao(Dados * d) {
	this->d = d;
	this->custo = 0;
	this->tempo = 0;
	this->demanda = new int[d->tipos];
	for(int i=0; i< d->tipos;++i){
		demanda[i] = 0;
	}
	this->S =  new float*[d->j];
	for(int j = 0; j < d->j;++j){
		S[j] = new float[4];
		for(int i = 0; i < 4 ;++i){
			S[j][i] = 0;
		}
	}
}

int Solucao::calcular_custo() {
	custo = 0;
	for (int k = 0; k < d->tipos; ++k) {
		custo +=  d->custo_recurso[k] * demanda[k] ;
	}
	return custo;
}

int Solucao::calcular_tempo(){
	tempo = 0;
	for(int j = 0; j < d->j;++j){
		tempo += S[j][2];
	}
	return tempo;
}

void Solucao::print(){
	cout << "j\t"<< "Ti\t" << "M\t"<< "t\t" << "C"<< endl;
	for(int j = 0; j < d->j;++j){
		cout << j  << "\t" << S[j][0]  << "\t" << S[j][1] << "\t" << S[j][2] << "\t" << S[j][3]<< endl;
	}
	cout << "C:" <<calcular_custo() << "\tT:" << tempo << endl;
}

Solucao::~Solucao() {
	free(demanda);
	free(d);

}
