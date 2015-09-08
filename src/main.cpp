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
#include "Teste.h"
#include <iostream>
#include <string>
#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>

#include <vector>

using namespace std;

void bateriaDeTestesComValores();

int main(int argc, char **argv) {

	//bateriaDeTestesComValores();
	//return 0;
	vector<float> tempos(12, 0);
		vector<float> valores(12, 0);

	for (int i = 1; i <= 1; i++) {

		//	cout << "instancia " << i << endl;

		string instancia = "Instancias_Denise";
		Arquivo arq(instancia, i);

		Dados *d = arq.lerInstancia();

//		d->print();


		Solucao *s1 = new Solucao(d);
		s1->iniciarSolucaoComMelhorCusto();
		d->D = s1->tempo;

		Grafico g;

		Heuristicas h(d);
		h.pso(10);

		clock_t start_time;
		start_time = clock();

//		Solucao *s = h.pso(50); // new Solucao(d);//

		double time_in_seconds = (clock() - start_time)
				/ (double) CLOCKS_PER_SEC;

		//tempos[i-1] = time_in_seconds;
	//	valores[i-1] = s->custo;


		//printf("tempo decorrido: %.2f s\n", time_in_seconds);

		Teste t(d);
		//t.testarSolucao(s);

		//s->print();

		//g.plotarGraficoDaSolucao(s);

		//cout << s->custo << endl;
		//cout << s->tempo << endl;

		//s->print();

		//h.vizinhacaBalancearUtilizacaoDeRecursos(s)->print();

		//g.plotarTrandOFF(h.fronteira);

	}

	for (int i = 0; i < 12; i++) {
		//	printf("%.2f\n" ,valores[i] );
	}

	return 0;
}

void bateriaDeTestesComValores() {
	vector<float> bestCusto(12, INT_MAX);
	vector<float> mediaCusto(12, 0);
	vector<float> piorCusto(12, 0);

	vector<float> bestTempo(12, INT_MAX);
	vector<float> mediaTempo(12, 0);
	vector<float> piorTempo(12, 0);

	int repeticoes = 100;
	vector <map<float, int> >  modaCusto(12);
	vector <map<float, int> > modaTempo(12);

	for (int rep = 0; rep < repeticoes; rep++) {
		cout << rep << endl;

		for (int i = 1; i <= 12; i++) {

			string instancia = "Instancias_Denise";
			Arquivo arq(instancia, i);

			Dados *d = arq.lerInstancia();

			Solucao *cus = new Solucao(d);
			cus->iniciarSolucaoComMelhorCusto();
			d->D = cus->tempo;

			Heuristicas h(d);

			clock_t start_time;
			start_time = clock();

			Solucao *s = h.geneticAlgorithms(20); // new Solucao(d);//
			//s->iniciarSolucaoComModosAleatoriosDentroDaDataLimite();

			float time_in_seconds = (clock() - start_time)
					/ (double) CLOCKS_PER_SEC;

			if (s->custo < bestCusto[i - 1]) {
				bestCusto[i - 1] = s->custo;
			}
			if (s->custo > piorCusto[i - 1]) {
				piorCusto[i - 1] = s->custo;
			}

			if (time_in_seconds < bestTempo[i - 1]) {
				bestTempo[i - 1] = time_in_seconds;
			}
			if (time_in_seconds > piorTempo[i - 1]) {
				piorTempo[i - 1] = time_in_seconds;
			}

			mediaCusto[i - 1] += s->custo;
			mediaTempo[i - 1] += time_in_seconds;

			modaCusto[i-1][s->custo] ++;
			modaTempo[i-1][time_in_seconds] ++;

		}
	}
	printf(
			"BestCusto\tMedioCusto\tPiorCusto\tBestTempo\tMedioTempo\tPiorTempo\n");
	for (int i = 1; i <= 12; i++) {
		mediaCusto[i - 1] /= repeticoes;
		mediaTempo[i - 1] /= repeticoes;

		printf("%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
				bestCusto[i - 1], mediaCusto[i - 1], piorCusto[i - 1],
				bestTempo[i - 1], mediaTempo[i - 1], piorTempo[i - 1]);
	}

	cout <<"Moda" << endl;
	for (int i = 0; i < 12; i++) {
		map<float, int>::iterator it = modaCusto[i].begin();
		float moda = modaCusto[i].begin()->first;
		for(; it!=modaCusto[i].end() ; it++){
			if(modaCusto[i][it->first] > modaCusto[i][moda]){
				moda = it->first;
			}
		}
		cout << moda<< endl;

	}
}

