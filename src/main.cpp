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

	for (int i = 1; i <= 1; i++) {

		cout << "instancia " << i << endl;

		string instancia = "Instancias_Denise";
		Arquivo arq(instancia, i);

		Dados *d = arq.lerInstancia();
//		d->print();

		Heuristicas h(d);

		Solucao *s = new Solucao(d);


		s->iniciarSolucaoComMelhorMakespan();
		h.addFronteiraDePareto(s);
	//	s->print();




		s = new Solucao(d);
		s->iniciarSolucaoComMelhorCusto();
		h.addFronteiraDePareto(s);
		//s->print();

		s = new Solucao(d);
		s->iniciarSolucaoComMenorUtilizacao();
		h.addFronteiraDePareto(s);
		//s->print();


		s = new Solucao(d);
		s->iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos();
		h.addFronteiraDePareto(s);
//		s->print();

		for (int i = 0; i < 10; i++) {
			s = new Solucao(d);
			s->iniciarSolucaoComModosAleatorios();
			h.addFronteiraDePareto(s);
		}


		//h.vizinhacaDeModos(s)->print();

		//testar solucao, apenas algumas vezes
		Grafico g;
		//g.plotarGraficoDaSolucao(s);
		g.plotarTrandOFF(h.fronteira);

	}


	return 0;
}
