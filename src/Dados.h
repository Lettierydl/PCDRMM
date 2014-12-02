/*
 * Dados.h
 *
 *  Created on: 25/11/2014
 *      Author: Leo
 */

#ifndef DADOS_H_
#define DADOS_H_

class Dados {
public:
	int tipos; // m quatidades de tipos diferentes de recrusos 4
	int j; // n quatidades de atividades mais as atividades virtuais
	int D; // D data de entrega do projeto
	int * M;// Mj modos de execulcao da ativiade j
	int ** H;// H, precedencia das atividades, (linha X coluna) = 1, entao linha precede coluna
	int * S; // Sj quantidade de sucessores que j tem
	int ** d;// dji duracao da atividade j(linha) quando execultada no modo i (coluna), j X Mj
	int *** r;// rjik, quantidade de recursos, que a atividade j, utiliza no modo i, do tipo k
	int *disponibilidade;
	int *custo_recurso;// custo de cada recurso para ser utilizado na funcao c();

	Dados();
	virtual ~Dados();

};

#endif /* DADOS_H_ */
