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
					for (int i = 0; i < d->j; i++, contline++) {
						this->dados >> c;
						this->dados >> d->M[i] ;
						this->dados >> d->S[i] ;


						for (int qs = 0; qs < d->S[i]; qs++) {
							int s;
							this->dados >> s;
							d->H[s - 1].push_back(i);
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


		return d;
}

/*


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

			d->H = vector<list<int> >(d->j);

			break;
		}
		case 20: {
			for (int i = 0; i < d->j; i++, contline++) {
				sregex_token_iterator l(linha.begin(), linha.end(), rx);
				++l;
				d->M[i] = stoi(l++->str());
				d->S[i] = stoi(l++->str());


				for (; l != sregex_token_iterator(); l++) {
					d->H[stoi(l->str()) - 1].push_back(i);
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
			d->custo_recurso = new float[d->tipos];
			regex rx("\\d+(\\.\\d{1,2})");
			sregex_token_iterator l(linha.begin(), linha.end(), rx);
			for (int k = 0; k < d->tipos; ++k, ++l) {
				d->custo_recurso[k] = stof(l->str());
			}
		}
	}


	return d;
}
*/

Arquivo::~Arquivo() {
	this->dados.close();
	this->funcao.close();
}

