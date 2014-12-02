/*
 * Solucao.h
 *
 *  Created on: 02/12/2014
 *      Author: Leo
 */

#ifndef SOLUCAO_H_
#define SOLUCAO_H_

#include "Dados.h"


class Solucao {
public:

	Dados * d;
	int* demanda;// demanda dos recursos utilizado na solucao
	int custo;
	int tempo;

	float ** S;// matriz solucao, onde S, diz que a ativida j, tem tempo de inicio (primeira linha), modo (segunda linha)
														   //tempo total da atividade (terceira linha), custo da atividade para o projeto(quarta linha)

	int calcular_custo();
	int calcular_tempo();

	void print();

	Solucao(Dados *d);
	virtual ~Solucao();


};


#endif /* SOLUCAO_H_ */
