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

using namespace std;

Dados::Dados(){}

Dados::~Dados() {
	free(this->H);
	free(this->M);
	free(this->S);
	free(this->custo_recurso);
	free(this->d);
	free(this->disponibilidade);
	free(this->r);
}

