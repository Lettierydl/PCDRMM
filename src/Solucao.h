/*
 * Solucao.h
 *
 *  Created on: 02/12/2014
 *      Author: Leo
 */

#ifndef SOLUCAO_H_
#define SOLUCAO_H_

#include "Dados.h"
#include <vector>
#include <algorithm>

class Solucao {
public:

	Dados * d;
	int* demanda; // demanda dos recursos utilizado na solucao
	float custo;
	int tempo;

	vector<int> Ti; // tempo que iniciara a atividade j
	vector<int> M; // modo em que sera executada da atividade j
	vector<int> D; // o quanto durara a atividade j

	vector<bool> alocadas; // vetor que diz se a atividade j esta alocada
	vector<vector<int> > tr; // matriz de tipo de recurso k por tempo t , onde o conteúdo int é a quantidade utilizada de recurso to tipo k no tempo t.


	/* Atributos do PSO */
	vector<int> v; // velocidade
	vector<int> v_new; // nova velocidade
	//vector<int> x_new; // nova posicao (novo tempo de incio)
	Solucao * pbest;
	Solucao * gbest;



	/* Métodos de Operações Bases */
	void alocarAtividade(int j, int ti, int mi); // aloca atividade j no tempo de inicio ti, no modo mi, ja faz toda as alteracoes na solucao
	void realocarAtividadesApartirDaAtividade(int j); // realoca atividades que iniciam depois de j, revendo seus tempos de incios mais cedos
	void desalocar(int j);

	/* Métodos de Incialização */
	void iniciarSolucaoComModosAleatorios(); // inicia a solucao alocando as atividades pela ordem de precedencia, com seus modos aleatorios
	void iniciarSolucaoComModosAleatoriosDentroDaDataLimite(); // inicia a solucao alocando as atividades pela ordem de precedencia, com seus modos aleatorios
	void iniciarSolucaoComMelhorMakespan(); // inicia a solucao alocando as atividades pela ordem de precedencia, com os modos de menor tempo
	void iniciarSolucaoComMelhorCusto(); // inicaia a solucao com menor custo sem alocar novos recursos, com modos que utilizem menos custo
	void iniciarSolucaoComMenorUtilizacao(); // inicia a solucao com a menor quantidade de utilizacao de recursos, desconsiderando o custo de utilizacao e o tempo
	void iniciarSolucaoComMenorUtilizacaoBalanceadaDeRecursos(); // balanceia a utilizacao de recursos, desconsiderando o custo de disponibilidade;

	/* Métodos para Verificacao */
	int verificarTempoInicioCedo(int j); // verifica o mais cedo tempo de inicio (ti) possivel para a atividade j, pelas suas precedencias ja cadastradas
	int verificarTempoInicioTarde(int j); // verifica o tempo de inicio mais tarde da ativiade j verificando seus sucessores e dentro do tempo limite para o projeto d->D
	int verificarTempoInicioTardeForaD(int j);
	int verificarMelhorModoPeloTempo(int j); // verifica o modo que gastará menos tempo, desconsiderando o custo do recurso
	int verificarMelhorModoPelaUtilizacao(int j); // verifica o modo que utiliza menos recursos, desconsiderando o tempo de duracao da atividade
	int verificarMelhorModoPelaMenorQuantidadeUtilizada(int j); // verifica o modo que utiliza a menor quantidade de recursos, desconsiderando o tempo e custo de utilizacao
	int verificarMelhorModoPelaMenorQuantidadeUtilizadaDeK(int j, int k); // verifica o modo que utiliza a menor quantidade do recurso k, desconsiderando o tempo e custo de utilizacao
	int verificarMelhorModoPelaMaiorQuantidadeUtilizadaDeK(int j, int k);

	/* Métodos de Atualização */
	void atualizarDemanda(int ti, int tf); //atualiada o valor da demanda dos recursos verificando apenas um determinada periodo de tepo da solucao de ti ate tf.

	/* Métodos de Calculos */
	list<int> calcularCaminhoDoFinalAteInicio(Solucao *s); //patindo do makespan e refeito o caminho ate a atividade 0
	void calcular_valores(); //calcula o custo e o tempo
	float calcular_custo();
	int calcular_tempo();

	/*Métodos de Ordenacao*/
	/*precos maiores aparecem primeiros*/
	vector<int> ordenarRecursosPorPrecos(); // retorna um veltor com os indices dos recursos ornedados pelo seu custo de utilizacao

	void print();

	Solucao(Dados *d);
	Solucao(Solucao *s);
	virtual ~Solucao();

	bool operator < (Solucao const&s1)  {
			if(s1.tempo ==  this->tempo){
				return (this->tempo < s1.tempo);
			}else{
				//return (s1.custo < s2.custo);
				return (this->custo < s1.custo);
			}

		}


};

struct SolucaoCompare {
	bool operator()(Solucao const&s1, Solucao const&s2) const {
		if(s1.tempo ==  s2.tempo){
			return (s1.tempo < s2.tempo);
		}else{
			//return (s1.custo < s2.custo);
			return (s1.custo < s2.custo);
		}

	}
};

struct SolucaoComparePorcentagem {
	bool operator()(Solucao const&s1, Solucao const&s2) const {
		//s1.calcular_valores(); s2.calcular_valores();

		int mint = std::min(s1.tempo, s2.tempo);

		int ts1 = (((mint * 90) / s1.tempo) -100) * -1; // porcentagem de quanto falta para alcancar a data limit do problema
		int ts2 = (((mint * 90) / s2.tempo) -100) * -1;

		float min = std::min(s1.custo, s2.custo);

		int cs1 = (((min * 100) / s1.custo) - 100) * -1; // porcentagem de quanto esta longe do melhor custo
		int cs2 = (((min * 100) / s2.custo) - 100) * -1;


		return ((ts1 + cs1) < (ts2 + cs2));
	}
};

struct SolucaoCompareAG {//comparacao para guiar o algoritmo genetico
	bool operator()(Solucao const&s1, Solucao const&s2) const {
		if(s1.tempo == s1.d->D && s2.tempo != s2.d->D){
			return true;
		}else if (s2.tempo == s2.d->D && s1.tempo != s1.d->D){
			return false;
		}
		//s1.calcular_valores(); s2.calcular_valores();

		float mint = std::min(s1.tempo, s2.tempo);

		int ts1 = (((mint * 90) / s1.tempo) -100) * -1; // porcentagem de quanto falta para alcancar a data limit do problema
		int ts2 = (((mint * 90) / s2.tempo) -100) * -1;

		float min = std::min(s1.custo, s2.custo);

		float cs1 = (((min * 100) / s1.custo) - 100) * -1; // porcentagem de quanto esta longe do melhor custo
		float cs2 = (((min * 100) / s2.custo) - 100) * -1;


		return ((ts1 + cs1) < (ts2 + cs2));
	}
};

#endif /* SOLUCAO_H_ */
