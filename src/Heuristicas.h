/*
 * Heuristicas.h
 *
 *  Created on: 17/03/2015
 *      Author: Leo
 */

#include "Dados.h"
#include "Solucao.h"
#include <list>

using namespace std;

/*Heuristicas e Meta-Heuristicas que serao implementadas no trabalho*/
class Heuristicas {
public:
	Dados *d;
	list<Solucao> fronteira;// fronteira de pareto gerada, tambem pode ser usada como trandogg

	bool addFronteiraDePareto(Solucao *s);//metodo que analiza solucao, e adiciona ao fronteira com a ideia da fronteira de pareto

	/*algoritmos de construcao*/
	list<Solucao> geneticAlgorithms(int geracoes);//Algoritmos Geneticos, constroe populacoes ate acabar as geracoes

	Solucao* grasp(int iteracoes, float alfa );//GRASP recebe o número de construcoes que ele faz e o fator aleatorio alfa 0..1

	/*algoritmos de busca local*/
	Solucao* vnd(Solucao *inicial, list<Solucao (*)(Solucao *)> vizinhancas);// VND que recebe a solucao inicial, e uma lista de metodos de vizinhacas

	/*vizinhancas*/

	Solucao* vizinhacaDeModos(Solucao *s);//vizinhanca que altera dos modos das atividade tentando melhorar o tempo e o custo da solucao (tirada de Gildasio, 2009)



	Heuristicas(Dados *d);
	virtual ~Heuristicas();
};

