//============================================================================
// Name        : PCDRMM.cpp
// Author      : Lettiery DLamare
// Version     :
// Copyright   : UFPB
// Description : Problema de Custo de Disponibilidade de Recruso com Multiplos Modos
//============================================================================

#include "Arquivo.h"
#include "Solucao.h"
#include "Grafico.h"
#include "Heuristicas.h"
#include <iostream>
#include "stdlib.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>

#include <vector>

using namespace std;

int main(int argc, char **argv) {

	for (int i = 1; i <= 12; i++) {

		//	cout << "instancia " << i << endl;

		string instancia = "Instancias_Denise";
		Arquivo arq(instancia, i);

		Dados *d = arq.lerInstancia();
//		d->print();

		Grafico g;

		Heuristicas h(d);

		Solucao *s = h.geneticAlgorithms(100);

		//g.plotarGraficoDaSolucao(s);

		cout << s->custo << endl;

		//s->print();

		//h.vizinhacaBalancearUtilizacaoDeRecursos(s)->print();


		//g.plotarTrandOFF(h.fronteira);
	}

	return 0;
}
