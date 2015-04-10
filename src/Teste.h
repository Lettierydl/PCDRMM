/*
 * Teste.h
 *
 *  Created on: 03/02/2015
 *      Author: Leo
 */

#ifndef TESTE_H_
#define TESTE_H_

#include "Solucao.h"
#include "Dados.h"

using namespace std ;

class Teste {
public:
	Dados*d;

	bool testarSolucao(Solucao * s);

	bool testePrecedencias(Solucao * s);

	bool testeValorCustoETempo(Solucao * s);

	bool testeSeTodasAtividadesEstaoAlocadas(Solucao * s);

	Teste(Dados *d);
	virtual ~Teste();
};


#endif /* TESTE_H_ */
