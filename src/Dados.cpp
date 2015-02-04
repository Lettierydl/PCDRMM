/*
 * Dados.cpp
 *
 *  Created on: 25/11/2014
 *      Author: Leo
 */

#include "Dados.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "stdlib.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

Dados::Dados(){}

void Dados::print(){
	cout << "J | D |Tipos"<< endl;
	cout << j << " | "<< D << " | "<< tipos << endl;

	cout <<"Rjik" << endl;
	for(int j = 0; j < this->j ;j++) {
		cout << j ;
		for(int i = 0; i < this->M[j] ;i++) {
			cout << "\t\t" << i << "\t\t";
			for(int k = 0; k < this->tipos ;k++) {
				cout << r[j][i][k] << "\t";
			}
			cout << endl;
		}
	}

	cout << endl;

	cout << "Duracao"<< endl;
	for(int j = 0; j < this->j ;j++) {
		cout << j ;
		for(int i = 0; i < this->M[j] ;i++) {
			cout <<"\t\t" << i << "\t "<< this->d[j][i] << endl;
		}
	}

	cout << endl;

	cout << "Precedencia"<< endl;
	for(int j = 0; j < this->j ;j++) {
		cout << j <<"\t\t{ ";
		for(list<int>::iterator it = this->H[j].begin(); it!= this->H[j].end() ;it++) {
			cout << *it << ", ";
		}
		cout <<" }"<< endl;

	}

}

Dados::~Dados() {
	free(this->M);
	free(this->S);
	free(this->custo_recurso);
	free(this->d);
	free(this->disponibilidade);
	free(this->r);
}

