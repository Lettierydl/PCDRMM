/*
 * Arquivo.cpp
 *
 *  Created on: 18/11/2014
 *      Author: Leo
 */

#include "Arquivo.h"
#include "Dados.h"
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Arquivo::Arquivo(string nomeArquivo, int numero) {

	this->numeroInstacia = numero;
	stringstream ss;
	ss << nomeArquivo << "/a10" << numero << "_1";

	this->nomeArquivo = ss.str();

	this->dados.open((this->nomeArquivo + ".dat").c_str(), ios::in);
	this->funcao.open((this->nomeArquivo + ".tx2").c_str(), ios::in);

	if (!&dados) {
		cout << "arquivo nao pode ser aberto" << endl;
	}
}

Dados * Arquivo::lerInstancia() {
	Dados * d;
		string linha;
		string c;// cache de palavras nao utilizadas
		int contline = 1;

		while (!this->dados.fail()) {
				int tipos;
				getline(this->dados, linha);
				contline++;

				switch (contline) {
				case 9: {
					this->dados >> c >> c >> c;

					this->dados >> tipos;
					break;
				}
				case 16: {
					int j, D;
					this->dados >> c;
					this->dados >> j;
					j+=2;//atividades virtuais

					this->dados >> c ;
					this->dados >> D;
					d = new Dados(tipos, j, D);
					break;
				}
				case 20: {
					for (int j = 0; j < d->j; j++, contline++) {
						this->dados >> c;
						this->dados >> d->M[j] ;
						int sj=0;// quantidade de sucessores que j tem
						this->dados >> sj ;


						for (int qs = 0; qs < sj; qs++) {
							int s;
							this->dados >> s;
							d->H[s - 1].push_back(j);
							d->S[j].push_back(s-1);
							d->G[j].insert(s-1);
						}

						getline(this->dados, linha);
					}

					for (int j = 0; j < d->j; ++j) {
						d->d[j] = vector<int>(d->M[j]); // quantidade de modos das atividades
					}
					for (int j = 0; j < d->j; ++j) {
						d->r[j] =  vector< vector<int> >(d->M[j]);
						for (int i = 0; i < d->M[j]; ++i) {
							d->r[j][i] =  vector<int>(d->tipos);
						}
					}
					break;
				}
				case 35: {
					for (int j = 0; j < d->j; j++) {
						for (int i = 0; i < d->M[j]; ++i) {
							if (i == 0) { // toda primeira linha tem um nuermo a mais
								this->dados >>c;
							}
							this->dados >>c;
							this->dados >> d->d[j][i];
							for (int k = 0; k < d->tipos; ++k) {

								this->dados >> d->r[j][i][k] ;
							}

							getline(this->dados, linha);
						}
					}

					contline = 67;
					break;
				}
				case 70: {

					for (int k = 0; k < d->tipos; ++k) {
						this->dados >> d->disponibilidade[k];
					}
					break;
				}

				}//fim do case

			} // fim do while final de leitura do arquivo

			contline = 1;

			while (!this->funcao.fail()) {
				contline++;
				getline(this->funcao, linha);
				if (contline == 16) {
					for (int k = 0; k < d->tipos; ++k) {
						this->funcao >> d->custo_recurso[k];
					}
				}
			}

		d->numeroInstacia = this->numeroInstacia;
		return d;
}

void Arquivo::gravarSolucao(int instancia, Solucao *s){

}

Arquivo::~Arquivo() {
	this->dados.close();
	this->funcao.close();
}

