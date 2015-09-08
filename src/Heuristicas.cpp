/*
 * Heuristicas.cpp
 *
 *  Created on: 17/03/2015
 *      Author: Leo
 */

#include "Heuristicas.h"
#include "Solucao.h"
#include "AlgAux.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

list<Solucao*> Heuristicas::pso(int epocas) {
	int tamanhoPopulacao = 30;
	int maxVparticula = 5;
	int minVpartucula = -5;
	float w = 0.2; // inercia
	float c1 = 0.1; // esperiencia local
	float c2 = 0.9; // esperiencia global
	list<Solucao*> gbests; // gbests com a ideia da fronteira de Pareto
	vector<Solucao> enxame;
	srand(time(NULL));

	//populacao inicial
	iniciarEnxame(tamanhoPopulacao, maxVparticula, minVpartucula, &enxame);

	//preencher gbestes com a fronteira de pareto
	gbests.push_front(&enxame[0]);
	addFronteiraDeParetoPSO(&enxame, &gbests);

	for (int e = 0; e < epocas; e++) {
		cout << e << endl;
		//atualizar velecidade (pela formula)

		atualizarVelocidades(tamanhoPopulacao, &enxame, w, c1, c2, maxVparticula, minVpartucula);


		atualizarPosicoes(tamanhoPopulacao, &enxame);


		cout << " _____ " << endl;
		gbests.clear();
		SolucaoCompare sc;
		sort(enxame.begin(), enxame.end(), sc);
		addFronteiraDeParetoPSO(&enxame, &gbests);
		cout << " _____ " << endl;



		//verificar novo gbest e pbests

	}//fim for epocas

	list<Solucao*>::iterator gb = gbests.begin();
	for (; gb != gbests.end(); gb++) {
		cout << (*gb)->custo << " " << (*gb)->tempo  << "   |    ";
	}cout << endl;

	return gbests;

}

void Heuristicas::atualizarVelocidades(int tamanhoPopulacao,
		vector<Solucao>* enxame, float w, float c1, float c2,
		int maxVparticula, int minVpartucula) {
	//atualizar posicao e velecidade (pela formula)
	for (int p = 0; p < tamanhoPopulacao; p++) {
		float r1 = (float) rand() / RAND_MAX + (rand() % 1);
		float r2 = (float) rand() / RAND_MAX + (rand() % 1);
		for (int pos = 1; pos < d->j * 2; pos++) { // atualizando velocidades
			int pbest, gbest;
			if (pos == d->j || pos == d->j-1) {continue;}

			if (pos < d->j) {
				pbest = (*enxame)[p].pbest->Ti[pos];
				gbest = (*enxame)[p].gbest->Ti[pos];
			} else {
				pbest = (*enxame)[p].pbest->M[pos - d->j];
				gbest = (*enxame)[p].gbest->M[pos - d->j];
			}

			int v = (*enxame)[p].v[pos];
			(*enxame)[p].v_new[pos] = w * v + (c1 * r1 * (pbest - v)) + //c1 r1 (pbest - v)
					(c2 * r2 * (gbest - v));		//c2 r2 (pbest - v)
			/*
			cout << "v_new = " << w << " * " << v << " + " << c1 << " * " << r1
					<< " * (" << pbest << " - " << v << ") + " << c2 << " * "
					<< r2 << "(" << gbest << " - " << v << ") " << endl;//c1 r1 (pbest - v)c2 << " * " << r2 << " * (" << gbest << " - " << v<< ")" << endl;
			 */
			//cout << pos <<"\t" << pbest <<" | "<< gbest  <<" \t\t " << (*enxame)[p].v[pos]<<" -> " << (*enxame)[p].v_new[pos] << endl;

			if ((*enxame)[p].v_new[pos] > maxVparticula) { // limites de velocidade
				(*enxame)[p].v_new[pos] = maxVparticula;
			} else if ((*enxame)[p].v_new[pos] < minVpartucula) {
				(*enxame)[p].v_new[pos] = minVpartucula;
			}

		}

	}
}

void Heuristicas::atualizarPosicoes(int tamanhoPopulacao, vector<Solucao>* enxame) {
	// atualiza solucao verificando se precedencia é respeirada

	for (int p = 0; p < tamanhoPopulacao; p++) {
		for (int pos = 1; pos < d->j; pos++) {

			//atualizar modos primeiro
			(*enxame)[p].M[pos] = (*enxame)[p].M[pos] + (*enxame)[p].M[pos+d->j];
			(*enxame)[p].D[pos] = d->d[pos][(*enxame)[p].M[pos]]; //nova duracao


			list<int>::iterator su = d->S[pos].begin();
			for (; su != d->S[pos].end(); su++) {
				// empurra todos susessores que devem ser empurrados para permanecer a precedencia
				int prec = ((*enxame)[p].Ti[pos] + (*enxame)[p].D[pos] + (*enxame)[p].v_new[pos])
						- (*enxame)[p].Ti[*su]; //v1 + Ti1 + D1 - Ti2

				if ( ((*enxame)[p].Ti[*su]+(*enxame)[p].v[*su]) < prec) {
					// precedencia falha
					(*enxame)[p].v_new[*su] = prec; // altera valor para manter precedencia
				}
			}
			(*enxame)[p].Ti[pos] = (*enxame)[p].Ti[pos] + (*enxame)[p].v_new[pos];
		}
		//atualizar velocidade e limpar vetores (new velocidade, new posicao)
	}


	for (int p = 0; p < tamanhoPopulacao; p++) {
		for (int pos = 1; pos < d->j  * 2; pos++) {
			(*enxame)[p].v[pos] = (*enxame)[p].v_new[pos];
			(*enxame)[p].v_new[pos] = 0;
		}

		(*enxame)[p].atualizarDemanda(0, (*enxame)[p].calcular_tempo());
		(*enxame)[p].calcular_custo();
	}
}


void Heuristicas::addFronteiraDeParetoPSO(vector<Solucao> *enxame, list<Solucao*> *gbests) {

	for (int p = 0; p < enxame->size(); p++) {
		bool inGbest = false;
		list<Solucao*>::iterator gb = gbests->begin();
		for (; gb != gbests->end(); gb++) {

			if ((*enxame)[p].tempo < (*gb)->tempo && (*enxame)[p].custo < (*gb)->custo) {

				(*gb)->pbest = &(*enxame)[p];
				gb = gbests->erase(gb);
				gbests->insert(gb, &(*enxame)[p]);

				inGbest = true;

				for (; gb != gbests->end(); gb++) {
					if ((*enxame)[p].tempo < (*gb)->tempo && (*enxame)[p].custo < (*gb)->custo) {
						(*gb)->pbest = &(*enxame)[p];
						gb = gbests->erase(gb);
					}
				}
				break;

			} else if ((*gb)->tempo < (*enxame)[p].tempo && (*gb)->custo < (*enxame)[p].custo) {
				(*enxame)[p].pbest = *gb;
				break;
			}else if((*gb)->tempo == (*enxame)[p].tempo && (*gb)->custo == (*enxame)[p].custo ){
				inGbest = true;
				break;
			}

		}

		if (gb == gbests->end() && !inGbest) { // nao foi adicionado e nem foi dominado por ninquem
			gbests->push_back(&(*enxame)[p]);
		}
		SolucaoCompare sc;
		gbests->sort(sc);
	}

	//segunda parte, pbest dos que estao na fronteira
	list<Solucao*>::reverse_iterator ft = gbests->rbegin();
	list<Solucao*>::reverse_iterator ft2 = gbests->rbegin();
	++ft;
	for (; ft != gbests->rend(); ft++, ft2++) { // setar pbest dos que estao na fronteira
		(*ft2)->pbest = *ft;
	}
	--ft2;
	(*gbests->begin())->pbest = *ft2;

	list<Solucao*>::iterator best = gbests->begin();

	for (int p = 0; p < enxame->size(); p++) {
		(*enxame)[p].gbest = *best; //seta o gbest de todos incluseve o do gbest como ele mesmo
	}

}

void Heuristicas::iniciarEnxame(int tamanhoPopulacao, int maxVparticula,
		int minVpartucula, vector<Solucao>* enxame) {
	//populacao inicial
	for (int tam = 0; tam < tamanhoPopulacao; tam++) {
		(*enxame).push_back(new Solucao(d));
	}
	(*enxame)[0].iniciarSolucaoComMelhorCusto();
	(*enxame)[1].iniciarSolucaoComMelhorMakespan();
	(*enxame)[2].iniciarSolucaoComMenorUtilizacao();
	(*enxame)[3].iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos();
	(*enxame)[4].iniciarSolucaoComModosAleatoriosDentroDaDataLimite();
	for (int tam = 5; tam < tamanhoPopulacao; tam++) {
		(*enxame)[tam].iniciarSolucaoComModosAleatoriosDentroDaDataLimite();
	}
	for (int p = 0; p < tamanhoPopulacao; p++) {
		(*enxame)[p].v = vector<int>(d->j * 2);
		(*enxame)[p].v_new = vector<int>(d->j * 2, 0); // nova velocidade
		(*enxame)[p].v[0] = 0;
		(*enxame)[p].v[d->j] = 0;
		(*enxame)[p].v[d->j - 1] = 0;
		(*enxame)[p].v[(d->j - 1) * 2] = 0;
		for (int pos = 1; pos < d->j - 1; pos++) {
			// velocidades iniciais aleatorias
			int max, min, mmax, mmin, vpos, vmod;
			min = (*enxame)[p].verificarTempoInicioCedo(pos) - (*enxame)[p].Ti[pos];
			max = (*enxame)[p].verificarTempoInicioTardeForaD(pos)
					- (*enxame)[p].Ti[pos];
			vpos = rand() % ((max - min) + 1) + min;
			if (d->M[pos] > 1) {
				mmin = 0 - (*enxame)[p].M[pos];
				mmax = (d->M[pos] - 1) - (*enxame)[p].M[pos];
				vmod = rand() % ((mmax - mmin) + 1) + mmin;
			} else {
				vmod = 0;
			}
			if (vpos > maxVparticula) {
				// limites de velocidade
				vpos = maxVparticula;
			} else if (vpos < minVpartucula) {
				vpos = minVpartucula;
			}

			(*enxame)[p].v[pos] = vpos;
			(*enxame)[p].v[pos + d->j] = vmod;
		}
	}
}

Solucao * Heuristicas::geneticAlgorithms(int geracoes) {
	vector<Solucao> populacao; //populacao sempre deve ter um numero par de individuos
	int tamanhoPopulacao = 20;

	//populacao inicial
	for (int tam = 0; tam < tamanhoPopulacao; tam++) {
		populacao.push_back(new Solucao(d));
	}

	populacao[0].iniciarSolucaoComMelhorCusto();
	populacao[1].iniciarSolucaoComMelhorMakespan();
	populacao[2].iniciarSolucaoComMenorUtilizacao();
	populacao[3].iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos();
	populacao[4].iniciarSolucaoComModosAleatoriosDentroDaDataLimite();

	for (int tam = 5; tam < tamanhoPopulacao; tam++) {
		populacao[tam].iniciarSolucaoComModosAleatoriosDentroDaDataLimite();
		populacao[tam].iniciarSolucaoComModosAleatorios();
	}

	for (int g = 0; g < geracoes; g++) {
		//criterio de parada por populacao identica
		if (populacao[0].custo == populacao[populacao.size() - 1].custo
				&& populacao[0].tempo
						== populacao[populacao.size() - 1].tempo) {
			//cout << "parou na geracao " << g << endl;
			//break;
		}

		/*
		 cout << "Geracao: " << g << ", populacao " << populacao.size();
		 cout << ", Best: " << populacao[0].custo << "|" << populacao[0].tempo
		 << ", Pior: " << populacao[populacao.size() - 1].custo << "|"
		 << populacao[populacao.size() - 1].tempo << endl;
		 */

		/*avaliacao da populacao*/
		SolucaoCompareAG sAG;
		SolucaoComparePorcentagem scp;

		sort(populacao.begin(), populacao.end(), sAG);

		/*selecao e cruzamento*/
		vector<Solucao> gerados = selecaoECruzamento(populacao);

		/*

		 for (int i = 0; i < populacao.size(); i++) {
		 if (populacao[i].tempo == 0) {
		 cerr << "llll" << endl;
		 }
		 }
		 */

		/*mutacao*/
		for (int i = 0; i < gerados.size(); i++) {
			//mutar(gerados[i]);
		}

		for (int i = 0; i < gerados.size(); i++) {
			populacao.push_back(gerados[i]);
		}

		sort(populacao.begin(), populacao.end(), sAG);

		//tamanho populacional crescente
		/*
		 int ret = populacao.size() * 0.7;
		 for (int ruin = populacao.size() - 1; ruin >= ret; ruin--) {
		 populacao.erase(--populacao.end());
		 }
		 */

		//populcao  com tamanho fixo tamanhoPopulacao
		for (int ruin = populacao.size() - 1;
				populacao.size() != tamanhoPopulacao; ruin--) {
			populacao.erase(--populacao.end());
		}

		if ((populacao.size() % 2) != 0) { //populacao impar
			populacao.erase(--populacao.end());
		}

		sort(populacao.begin(), populacao.end(), sAG);

//		cout << "---pop " << populacao.size() << endl<< endl;

		//cout << "geralcao " << g << endl<< endl;
		for (int i = 0; i < populacao.size(); i++) {
			addFronteiraDePareto(&populacao[i]);
			//	cout << populacao[i].tempo << " " << populacao[i].custo << " | ";
		}
	}

	for (int i = 0; i < populacao.size(); i++) {
		addFronteiraDePareto(&populacao[i]);
		//	cout << populacao[i].tempo << " " << populacao[i].custo << " | ";
	}
	//cout << endl;

	Solucao * best = new Solucao(populacao[0]);
	return best;

}

vector<Solucao> Heuristicas::selecaoECruzamento(vector<Solucao> & populacao) {
	/*selecao e cruzamento*/
	vector<Solucao> gerados;

	switch (1) {
	case 2: //cruzamento 2
		for (int ind2 = populacao.size() - 1; ind2 > 0; ind2--) {
			if (populacao[0].tempo == populacao[ind2].tempo
					&& populacao[0].custo == populacao[ind2].custo) {
				if (populacao[0].tempo == populacao[populacao.size() - 1].tempo
						&& populacao[0].custo
								== populacao[populacao.size() - 1].custo) {
					break; // populacao identica
				}
				vector<Solucao> filhos = cruzamento2(populacao[0],
						populacao[populacao.size() - 1]);
				gerados.push_back(filhos[0]);
				gerados.push_back(filhos[1]);
			} else {
				vector<Solucao> filhos = cruzamento2(populacao[0],
						populacao[ind2]); // o erro esta aqui
				gerados.push_back(filhos[0]);
				gerados.push_back(filhos[1]);
			}
		}
		break;
	default: //cruzamento aleatorio
		for (int ind = 0, ind2 = populacao.size() - 1; ind < ind2;
				ind++, ind2--) {
			if (populacao[ind].tempo == populacao[ind2].tempo
					&& populacao[ind].custo == populacao[ind].custo) {
				gerados.push_back(
						cruzamento(populacao[0],
								populacao[populacao.size() - 1]));
			} else {
				gerados.push_back(cruzamento(populacao[0], populacao[ind2]));
			}
		}
		break;
	}

	return gerados;
}

void Heuristicas::mutar(Solucao s) {
	//usar vizinhancas para tentar melhorar a solucao
	Solucao * vizinha = vnd(&s);
	if (vizinha->tempo <= s.tempo && vizinha->custo < s.custo) { // vizinha melhorou
	//delete &s;
		cout << "mutou de " << s.tempo << " | " << s.custo;
		cout << ", para " << vizinha->tempo << " | " << vizinha->custo << endl
				<< endl;
		s = new Solucao(vizinha);

	} else {
		delete vizinha;
	}
}

Solucao* Heuristicas::vnd(Solucao* inicial) {
	SolucaoCompareAG sAG;
	Solucao * best = new Solucao(inicial);
	Solucao * v;
	while (true) {
		v = this->vizinhacaBalancearUtilizacaoDeRecursos(inicial);
		if (v->tempo > best->tempo || v->custo >= best->custo) {
			v = this->vizinhacaDeModos(inicial);
			if (v->tempo > best->tempo || v->custo >= best->custo) {
				v = this->vizinhacaDeslocamentoTemporal(inicial);
				if (v->tempo > best->tempo || v->custo >= best->custo) {
					v = this->vizinhacaDeslocarParaDireita(inicial);
					if (v->tempo > best->tempo || v->custo >= best->custo) {
						delete v;
						break;
					} else {
						delete best;
						best = v;
						continue;
					}
				} else {
					delete best;
					best = v;
					continue;
				}
			} else {
				delete best;
				best = v;
				continue;
			}
		} else {
			delete best;
			best = v;
			continue;
		}

	}

	return best;

}

Solucao Heuristicas::cruzamento(Solucao s1, Solucao s2) {
	Solucao s(d);
	//bool acessoS1 = rand() % 2;
	for (int j = 0; j < d->j - 1; j++) {
		bool acessoS1 = rand() % 2;
		if (acessoS1) {	// o cromossomo da atividade j sera herdado de s1
			int tic = calcularTempoDeInicioMaisCedoDeJ(&s, j);
			if (s1.Ti[j] >= tic) {	//pode alocar
				s.alocarAtividade(j, s1.Ti[j], s1.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				s.alocarAtividade(j, tic, s1.M[j]);
			}
		} else {	// o cromossomo da atividade j sera herdado de s2
			int tic = calcularTempoDeInicioMaisCedoDeJ(&s, j);
			if (s2.Ti[j] >= tic) {	//pode alocar
				s.alocarAtividade(j, s2.Ti[j], s2.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				s.alocarAtividade(j, tic, s2.M[j]);
			}
		}
	}
	s.alocarAtividade(d->j - 1, s.calcular_tempo(), 0);
	s.calcular_custo();

	return s;
}

vector<Solucao> Heuristicas::cruzamento2(Solucao s1, Solucao s2) {
	Solucao f1(d);
	Solucao f2(d);

	bool acessoS1 = rand() % 2;
	for (int j = 0; j < d->j - 1; j++) {

		if (acessoS1) {	// o cromossomo da atividade j sera herdado de s1
			int ticF1 = calcularTempoDeInicioMaisCedoDeJ(&f1, j);
			int ticF2 = calcularTempoDeInicioMaisCedoDeJ(&f2, j);
			if (s1.Ti[j] >= ticF1) {	//F1 receberar cromossomo de S1
				f1.alocarAtividade(j, s1.Ti[j], s1.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				f1.alocarAtividade(j, ticF1, s1.M[j]);
			}

			if (s2.Ti[j] >= ticF2) {	//F2 receberar cromossomo de S2
				f2.alocarAtividade(j, s2.Ti[j], s2.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				f2.alocarAtividade(j, ticF2, s2.M[j]);
			}

		} else {	// o cromossomo da atividade j sera herdado de s2

			int ticF1 = calcularTempoDeInicioMaisCedoDeJ(&f1, j);
			int ticF2 = calcularTempoDeInicioMaisCedoDeJ(&f2, j);
			if (s2.Ti[j] >= ticF1) {	//F1 receberar cromossomo de S2
				f1.alocarAtividade(j, s2.Ti[j], s2.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				f1.alocarAtividade(j, ticF1, s2.M[j]);
			}

			if (s1.Ti[j] >= ticF2) {	//F2 receberar cromossomo de S1
				f2.alocarAtividade(j, s1.Ti[j], s1.M[j]);
			} else {// nao pode alocar j em Ti por que vai quebrar a precedencia de j, por isso aloca no menor tempo possivel tic
				f2.alocarAtividade(j, ticF2, s1.M[j]);
			}
		}
		acessoS1 = !acessoS1;
	}
	f1.alocarAtividade(d->j - 1, f1.calcular_tempo(), 0);
	f2.alocarAtividade(d->j - 1, f2.calcular_tempo(), 0);

	f1.calcular_custo();
	f2.calcular_custo();

	vector<Solucao> filhos;
	filhos.push_back(f1);
	filhos.push_back(f2);

	return filhos;
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
									//cout << "Melhorou" << endl;
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

		for (int m = 0; m < d->M[j]; m++) {
			int timt = tft - s1->D[j];	//tempo de inicio mais tarde nesse modo
			int ti_old = s1->Ti[j];
			int m_old = d->M[j];
			for (int t = tic; t <= timt; t++) {

				s1->alocarAtividade(j, t, m);
				if (s1->calcular_custo() < best->custo) {
					cout << "\n\n\n\n melhorou de " << s1->custo << " para "
							<< best->custo << endl;
					delete best;
					best = s1;
				} else {
					s1->alocarAtividade(j, ti_old, s->M[j]);
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

