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
	//list<Solucao> gbests;// fronteira de pareto gerada, tambem pode ser usada como trandogg

	bool addFronteiraDePareto(Solucao *s);//metodo que analiza solucao, e adiciona a fronteira com a ideia da fronteira de pareto

	/*PSO*/
	void iniciarEnxame(int tamanhoPopulacao, int maxVparticula, int minVpartucula, vector<Solucao>* enxame);
	void addFronteiraDeParetoPSO(vector<Solucao> *enxame, list<Solucao*> *gbests);//metodo que analiza solucao, e adiciona a variavel gbests com a ideia da fronteira de pareto
	void atualizarPosicoes(int tamanhoPopulacao, vector<Solucao>* enxame);
	void atualizarVelocidades(int tamanhoPopulacao, vector<Solucao>* enxame, float w, float c1, float c2, int maxVparticula, int minVpartucula);
	void atualizarPbestGbest(int tamanhoPopulacao, vector<Solucao>* enxame);

	/*algoritmos de construcao*/
	Solucao* geneticAlgorithms(int geracoes);//Algoritmos Geneticos, constroe populacoes ate acabar as geracoes

	Solucao* grasp(int iteracoes, float alfa );//GRASP recebe o número de construcoes que ele faz e o fator aleatorio alfa 0..1

	list<Solucao*> pso(int epocas);//PSO recebe o número de epocas e retorna a fronteira de pareto das melhores solucoes


	/*algoritmos de busca local*/
	Solucao* vnd(Solucao *inicial);// VND que recebe a solucao inicial e realiza uma busca local com as vizinhancas tentando melhorar a solucao


	/*vizinhancas*/

	Solucao* vizinhacaDeslocamentoTemporal(Solucao *s);//vizinhanca que altera o tempo de incio das atividades (dentro da janela possivel) tentando minimizar o custo
	Solucao* vizinhacaDeslocarParaDireita(Solucao *s);//vizinhanca que altera o tempo de incio das atividades (dentro da janela possivel) tentando minimizar o custo
	Solucao* vizinhacaBalancearUtilizacaoDeRecursos(Solucao *s);//vizinhanca procura os maiores valores de utilizacao de recursos e tenta realocar atividades que causaram isso
	Solucao* vizinhacaDeModos(Solucao *s);//vizinhanca que altera dos modos das atividade tentando melhorar o tempo e o custo da solucao (tirada de Gildasio, 2009)


	/*Algoritmos Geneticos*/
	Solucao cruzamento(Solucao s1, Solucao s2);
	vector<Solucao> cruzamento2(Solucao s1, Solucao s2);

	void mutar(Solucao s1);

	/*Cauculos*/
		/*janela temporal de J */
	int calcularTempoDeInicioMaisCedoDeJ(Solucao *s, int j);//tempo de inicio mais cedo que j pode ser alocada sem alterar em outras atividades
	int calcularTempoFimMaisTardeDeJ(Solucao *s, int j);//tempo de fim mais Tarde possivel que j pode finalizar sem alterar as outras atividades



	Heuristicas(Dados *d);
	virtual ~Heuristicas();


private:
	vector<Solucao> selecaoECruzamento(vector<Solucao>& populacao);

};

