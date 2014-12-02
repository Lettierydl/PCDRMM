/*
 * Arquivo.cpp
 *
 *  Created on: 18/11/2014
 *      Author: Leo
 */

#include "Arquivo.h"
#include "Dados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <regex>

using namespace std;

Arquivo::Arquivo(string nomeArquivo, int numero) {
	this->nomeArquivo = nomeArquivo + "/a10" + to_string(numero) + "_1";

	this->dados.open(this->nomeArquivo + ".dat", ios::in);
	this->funcao.open(this->nomeArquivo + ".tx2", ios::in);

	if (!&dados) {
		cout << "arquivo nao pode ser aberto" << endl;
	}
}

Dados * Arquivo::lerInstancia() {
	Dados *d = new Dados();
	string linha;
	int contline = 0;
	regex rx("\\d+");

	while (!this->dados.fail()) {
		getline(this->dados, linha);
		contline++;
		switch (contline) {
		case 9: {
			sregex_token_iterator j(linha.begin(), linha.end(), rx);
			d->tipos = stoi(j->str());
			break;
		}
		case 16: {
			sregex_token_iterator i(linha.begin(), linha.end(), rx);
			++i;
			d->j = stoi(i->str()) + 2;
			++i;
			++i;
			d->D = stoi(i->str());
			d->M = new int[d->j];
			d->S = new int[d->j];

			d->H = new int*[d->j];
			for (int l = 0; l < d->j; ++l) {
				d->H[l] = new int[d->j];
				for (int c = 0; c < d->j; c++) {
					d->H[l][c] = 0;
				}
			}
			break;
		}
		case 20: {
			for (int i = 0; i < d->j; i++, contline++) {
				sregex_token_iterator l(linha.begin(), linha.end(), rx);
				++l;
				d->M[i] = stoi(l++->str());
				d->S[i] = stoi(l++->str());

				for (; l != sregex_token_iterator(); l++) {
					d->H[i][stoi(l->str()) - 1] = 1;
					//d->H[stoi(l->str())-1][i]= -1;
				}

				getline(this->dados, linha);
			}

			d->d = new int*[d->j]; // declarando a duracao das atividades nos modos i
			for (int j = 0; j < d->j; ++j) {
				d->d[j] = new int[d->M[j]]; // quantidade de modos das atividades
				for (int i = 0; i < d->M[j]; ++i) {
					d->d[j][i] = 0;
				}
			}

			d->r = new int**[d->j]; // tabela de demanda de recursos k da atividade j no modo i
			for (int j = 0; j < d->j; ++j) {
				d->r[j] = new int*[d->M[j]];
				for (int i = 0; i < d->M[j]; ++i) {
					d->r[j][i] = new int[d->tipos];
					for (int k = 0; k < d->tipos; ++k) {
						d->r[j][i][k] = 0;
					}
				}
			}
			break;
		}
		case 35: { // erro
			for (int j = 0; j < d->j; j++) {
				for (int i = 0; i < d->M[j]; ++i) {
					sregex_token_iterator l(linha.begin(), linha.end(), rx);
					++l;
					if (i == 0) { // toda primeira linha tem um nuermo a mais
						++l;
					}
					d->d[j][i] = stoi(l->str());
					for (int k = 0; k < d->tipos; ++k) {
						++l;
						d->r[j][i][k] = stoi(l->str());
					}

					getline(this->dados, linha);
				}
			}

			/*imprimir r
			 for (int j = 0; j < d->j; j++) {
			 cout << endl<< "j "<< j << ":";
			 for(int i = 0; i < d->M[j]; ++i){
			 cout << endl << "\tM "<< i << " dj: "<< d->d[j][i] << endl << "\t\t R -> ";
			 for(int k = 0; k < d->tipos;++k){
			 cout << d->r[j][i][k]<< " ";
			 }
			 }
			 }
			 */
			contline = 67;
			break;
		}
		case 70: {
			sregex_token_iterator l(linha.begin(), linha.end(), rx);
			d->disponibilidade = new int[d->tipos];
			for (int k = 0; k < d->tipos; ++k, ++l) {
				d->disponibilidade[k] = stoi(l->str());
			}
			cout << endl;
			break;
		}
		}

	} // fim do while final de leitura do arquivo
	contline = 0;
	while (!this->funcao.fail()) {
		contline++;
		getline(this->funcao, linha);
		if (contline == 16) {
			d->custo_recurso = new int[d->tipos];
			sregex_token_iterator l(linha.begin(), linha.end(), rx);
			for (int k = 0; k < d->tipos; ++k, ++l) {
				d->custo_recurso[k] = stoi(l->str());
			}
		}
	}

	return d;
}

Arquivo::~Arquivo() {
	this->dados.close();
	this->funcao.close();
}

