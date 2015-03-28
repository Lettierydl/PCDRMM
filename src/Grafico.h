/*
 * Grafico.h
 *
 *  Created on: 13/03/2015
 *      Author: Leo
 */

#include <unistd.h>
#include "Solucao.h"
#include <list>
#include <vector>

#ifndef GRAFICO_H_
#define GRAFICO_H_

using namespace std;

class Grafico {
public:

	void plotarGraficoDaSolucao(Solucao *s);

	void plotarTrandOFF(list<Solucao> solucoes);
	void plotarTrandOFF(vector<float> custo, vector<int> tempo);

	Grafico();
	virtual ~Grafico();
};


#endif /* GRAFICO_H_ */
