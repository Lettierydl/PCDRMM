/*
 * Heuristicas.cpp
 *
 *  Created on: 17/03/2015
 *      Author: Leo
 */

#include "Heuristicas.h"
#include "Solucao.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

Solucao * Heuristicas::geneticAlgorithms(int geracoes) {
	vector<Solucao> populacao; //populacao sempre deve ter um numero par de individuos

	//populacao inicial
	for (int tam = 0; tam < 6; tam++) {
		populacao.push_back(new Solucao(d));
	}

	populacao[0].iniciarSolucaoComMelhorCusto();
	populacao[1].iniciarSolucaoComMelhorMakespan();
	populacao[2].iniciarSolucaoComMenorUtilizacao();
	populacao[3].iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos();
	populacao[4].iniciarSolucaoComModosAleatorios();
	populacao[5].iniciarSolucaoComModosAleatorios();

	for (int g = 0; g < geracoes; g++) {

		/*avaliacao da populacao*/
		SolucaoCompareAG scp;
		sort(populacao.begin(), populacao.end(), scp);

		/*
		for (int i = 0; i < populacao.size(); i++) {
			cout << populacao[i].tempo << " " << populacao[i].custo << "|";
		}
		cout << endl;
		*/

		/*selecao e cruzamento*/
		vector<Solucao> gerados;
		for (int ind = 0 ,ind2 = populacao.size()-1; ind < ind2 ; ind++, ind2--) {
			if(populacao[ind].tempo == populacao[ind2].tempo &&
			   populacao[ind].custo == populacao[ind].custo){
				gerados.push_back(cruzamento(populacao[0], populacao[populacao.size()-1]));
			}else{
				gerados.push_back(cruzamento(populacao[0], populacao[ind2]));
			}
		}

		/*mutacao*/
		for (int i = 0; i < gerados.size(); i++) {
			mutar(gerados[i]);
		}

		for (int i = 0; i < gerados.size(); i++) {
			populacao.push_back(gerados[i]);
		}

		sort(populacao.begin(), populacao.end(), scp);
	//	cout << "pop " << populacao.size() << endl;

		int ret = populacao.size() * 0.7;
		for(int ruin = populacao.size() - 1; ruin >= ret  ;ruin--){
			populacao.erase(--populacao.end());
		}
		if((populacao.size() % 2) != 0){//populacao impar
			populacao.erase(--populacao.end());
		}

//		cout << "---pop " << populacao.size() << endl<< endl;

		//cout << "geralcao " << g << endl<< endl;
	}
	Solucao * best = new Solucao(populacao[0]);
	return best;

}

void Heuristicas::mutar(Solucao s) {
	//usar vizinhancas para tentar melhorar a solucao
}

Solucao Heuristicas::cruzamento(Solucao s1, Solucao s2) {
	Solucao s(d);

	for (int j = 0; j < d->j - 1; j++) {
		bool acessoS1 = rand() % 2;
		if (acessoS1) {	// o cromossomo da atividade j sera herdado de s1
			int tic = calcularTempoDeInicioMaisCedoDeJ(&s1, j);
			if (s1.Ti[j] >= tic) {	//pode alocar
				s.alocarAtividade(j, s1.Ti[j], s1.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				s.alocarAtividade(j, tic, s1.M[j]);
			}
		} else {	// o cromossomo da atividade j sera herdado de s2
			int tic = calcularTempoDeInicioMaisCedoDeJ(&s1, j);
			if (s2.Ti[j] >= tic) {	//pode alocar
				s.alocarAtividade(j, s2.Ti[j], s2.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				s.alocarAtividade(j, tic, s2.M[j]);
			}
		}
	}
	s.alocarAtividade(d->j - 1, s.calcular_tempo(), 0);

	return s;
}

bool Heuristicas::addFronteiraDePareto(Solucao *s) {

	list<Solucao>::iterator it = fronteira.begin();
	for (; it != fronteira.end(); it++) {
		if (s->tempo < it->tempo && s->custo < it->custo) { // s domina it
		// coloca s e retira it
			*it = s;
			for (it++; it != fronteira.end(); it++) {//retirar as outras solucao que s domina
				if (s->tempo < it->tempo && s->custo < it->custo) {
					fronteira.erase(it);
				}
			}

			SolucaoCompare sc;
			fronteira.sort(sc);
			return true;
		}
		if (s->tempo > it->tempo && s->custo > it->custo) {		// it domina s
		// s e descartada, pois ja tem uma melhor do que s em tempo e em custo na fronteira
			return false;
		}

	}
	// caso nao ocoreu nenhuma dominacao entao s deve ser colocado na fronteira
	fronteira.push_back(s);
	SolucaoCompare sc;
	fronteira.sort(sc);

	return true;
}

Solucao * Heuristicas::vizinhacaBalancearUtilizacaoDeRecursos(Solucao * s) {
	s->calcular_valores();
	Solucao * s1 = new Solucao(s);	//copia a solucao atual
	Solucao * best = new Solucao(s);	//melhor solucao encontrada

	vector<int> recursosMaisCaros = s->ordenarRecursosPorPrecos();
	for (int k = 0; k < recursosMaisCaros.size(); k++) {
		int recAtual = recursosMaisCaros[k];

		for (int t = 0; t <= s1->tempo; t++) {
			if (s1->tr[t][recAtual] == s->demanda[recAtual]) {// ponto de maior utilizacao desse recurso
				for (int j = 0; j < d->j; j++) {
					if (s1->Ti[j] == t) {// a atividade j inicia no ponto crito, ela deve ser melhorada
						int tic = calcularTempoDeInicioMaisCedoDeJ(s1, j);
						int tft = calcularTempoFimMaisTardeDeJ(s1, j);
						int m_old = s->M[j];

						int timt = tft - d->d[j][m_old];
						for (int m = 0; m < d->M[j]; m++) {	//testa se variando o modo funciona
							for (int new_t = tic; new_t <= timt; new_t++) {

								s1->alocarAtividade(j, new_t, m);
								if (s1->calcular_custo() < best->custo) {//melhorou
									delete best;
									cout << "Melhorou" << endl;
									best = s1;	//melhorou
								} else {	// volta a alocacao como estava
									s1->alocarAtividade(j, t, m_old);
								}

							}

						}

					}

				}
			}
		}

	}

	if (s1 != best) {
		delete s1;
	}
	if (s->custo == best->custo) {
		delete best;
		return s;
	}
	return best;
}

Solucao* Heuristicas::vizinhacaDeslocarParaDireita(Solucao *s) {
	s->calcular_valores();
	Solucao * s1 = new Solucao(s);		//copia a solucao atual

	for (int j = d->j - 1; j > 0; j--) {

		int tic = calcularTempoDeInicioMaisCedoDeJ(s1, j);

		int tft = calcularTempoFimMaisTardeDeJ(s1, j);

		if (tic + s->D[j] == tft) {	//a atividade nao pode ser deslocada, sua janela temporal ja esta preencida por ela
			continue;
		}

		int timt = tft - s1->D[j];		//tempo de inicio mais tarde nesse modo

		int ti_old = s1->Ti[j];
		s1->alocarAtividade(j, timt, s1->M[j]);
		if (s1->calcular_custo() > s->custo) {
			s1->alocarAtividade(j, ti_old, s1->M[j]);
		}

	}
	return s1;
}

/*tentativa de melhorar o custo sem alterar o tempo*/
Solucao* Heuristicas::vizinhacaDeslocamentoTemporal(Solucao*s) {
	s->calcular_valores();
	Solucao * s1 = new Solucao(s);		//copia a solucao atual
	Solucao * best = new Solucao(s);

	for (int j = 0; j < d->j - 1; j++) {

		int tic = calcularTempoDeInicioMaisCedoDeJ(s1, j);

		int tft = calcularTempoFimMaisTardeDeJ(s1, j);

		if (tic + s->D[j] == tft) {	//a atividade nao pode ser deslocada, sua janela temporal ja esta preencida por ela
			continue;
			/*int m_old = d->M[j];
			 for (int m = 0; m < d->M[j]; m++) {
			 if (m_old != m) {
			 s1->alocarAtividade(j, tic, m);
			 tic = calcularTempoDeInicioMaisCedoDeJ(s1, j);
			 tft = calcularTempoFimMaisTardeDeJ(s1, j);
			 if(tic + s->D[j] != tft){
			 break;
			 }
			 }
			 }*/
		}

		int timt = tft - s1->D[j];		//tempo de inicio mais tarde nesse modo
		int ti_old = s1->Ti[j];
		int m_old = d->M[j];
		for (int t = tic; t <= timt; t++) {
			if (ti_old != t) {	// nao repete verificacao da solucao atual
				for (int m = 0; m < d->M[j]; m++) {
					if (m_old != m) {
						s1->alocarAtividade(j, t, m);
						if (s1->calcular_custo() < best->custo) {
							cout << "\n\n\n\n melhorou de " << s1->custo
									<< " para " << best->custo << endl;
							delete best;
							best = s1;
						} else {
							s1->alocarAtividade(j, ti_old, s->M[j]);
						}
					}
				}

			}
		}

	}
	if (s1 != best) {
		delete s1;
	}
	if (s->custo == best->custo) {
		delete best;
		return s;
	}
	return best;
}

Solucao* Heuristicas::vizinhacaDeModos(Solucao *s) {

	Solucao * s2 = new Solucao(s);
	Solucao * melhorTempo = new Solucao(s2); //melhor solucao por tempo
	Solucao * melhorCusto = new Solucao(s2); //melhor solucao por custo

	for (int j = 1; j < d->j; j++) {
		int m_aux = s2->M[j];
		for (int m = 0; m < d->M[j]; m++) {
			if (m == m_aux) { //desconsidera o modo atual
				continue;
			}

			s2->alocarAtividade(j, s2->Ti[j], m);
			s2->realocarAtividadesApartirDaAtividade(j);
			s2->calcular_valores();
			if (s2->custo < melhorCusto->custo) {
				delete melhorCusto;
				melhorCusto = s2;
			}
			if (s2->tempo < melhorTempo->tempo) {
				delete melhorTempo;
				melhorTempo = s2;
			}
		}

	}
	if (s2 != melhorCusto && s2 != melhorTempo) {
		delete s2;
	}
	if (melhorTempo->tempo < s->tempo || melhorCusto->custo < s->custo) {

		if (melhorTempo->tempo == melhorCusto->tempo) {
			delete melhorTempo;
			return melhorCusto;
		} else if (melhorCusto->custo == melhorTempo->custo) {
			delete melhorCusto;
			return melhorTempo;
		} else { //salvar a solucao que sera descartada em um array de boas solucoes!!!
			float reducaoDeCusto = ((s->custo - melhorCusto->custo) * 100)
					/ s->custo; //porcentagem de reducao de custo
			float reducaoDeTempo = ((s->tempo - melhorTempo->tempo) * 100)
					/ s->tempo; //porcentagem de reducao de tempo

			if (reducaoDeCusto > reducaoDeTempo) {
				delete melhorTempo;
				return melhorCusto;
			} else {
				delete melhorCusto;
				return melhorTempo;
			}
		}
	} else {
		delete melhorTempo;
		delete melhorCusto;
		if (s2 == NULL) {
			delete s2;
		}
		return new Solucao(*s);
	}
}

/*Cauculos*/
int Heuristicas::calcularTempoDeInicioMaisCedoDeJ(Solucao *s, int j) {
	int tic = 0;		//tempo de inicio mais cedo de j
	list<int>::iterator pr = d->H[j].begin();		//predecessores de j
	for (; pr != d->H[j].end(); pr++) {
		if ((s->Ti[*pr] + s->D[*pr]) > tic) {
			tic = (s->Ti[*pr] + s->D[*pr]);
		}
	}
	return tic;
}

int Heuristicas::calcularTempoFimMaisTardeDeJ(Solucao *s, int j) {
	int tft = s->tempo;		//tempo de fim mais tarde de j
	list<int>::iterator su = d->S[j].begin();		//sucessores de j
	for (; su != d->S[j].end(); su++) {
		if (s->Ti[*su] < tft) {
			tft = s->Ti[*su];
		}
	}
	return tft;
}

Heuristicas::Heuristicas(Dados *d) :
		fronteira() {
	this->d = d;
}

Heuristicas::~Heuristicas() {
	// TODO Auto-generated destructor stub
}

