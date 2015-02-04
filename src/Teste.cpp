/*
 * Teste.cpp
 *
 *  Created on: 03/02/2015
 *      Author: Leo
 */

#include "Teste.h"
#include "Solucao.h"

namespace std {

Teste::Teste() {}
Teste::~Teste() {}


/* metodo que testa se a solucao é valida */
bool Teste::testarSolucao(Solucao * s){
	//testar se as relacoes de precedencia entre as atividades estao mantidas

	//testar se os valores da demanda, custo e tempo estao calculados corretamente

	//testar se toda atividade esta alocada em um determinado tempo de inicio e um modo

	/*testar se a atividade gastar o valor (duracao e requerimento de recursos) colocado na solucao,
	ou seja, se uma atividad j esta alocada com um modo i, então ela vai requerer uma quantidade de recursos
	e gasta um determinado tempo, ver se esse tempo e essa quantidade estão sendo somados na solucao */

	// caso uma dessas regras falhe, deve ser impresso qual regra falou e se possivel detalhar onde ela falhou
	// e mesmo que ocorra uma falha, deve ser verificado todas as regas, e so no final retornar false

	return true;
}





} /* namespace std */
