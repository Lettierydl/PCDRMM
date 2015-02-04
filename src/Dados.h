/*
 * Dados.h
 *
 *  Created on: 25/11/2014
 *      Author: Leo
 */

#ifndef DADOS_H_
#define DADOS_H_

#include <list>
#include <vector>

using namespace std;

class Dados {
public:
	int tipos; // m quatidades de tipos diferentes de recrusos 4
	int j; // n quatidades de atividades mais as atividades virtuais
	int D; // D data maxima de entrega do projeto
	int * M;// Mj modos de execulcao da ativiade j
	vector< list<int> > H;// Hj, precedencia das atividades, a atividade j, tem a lista de atividade que precedem ela
	int * S; // Sj quantidade de sucessores que j tem
	int ** d;// dji duracao da atividade j(linha) quando execultada no modo i (coluna), j X Mj
	int *** r;// rjik, quantidade de recursos, que a atividade j, utiliza no modo i, do tipo k
	int *disponibilidade;
	float *custo_recurso;// custo de cada recurso para ser utilizado na funcao c();

	void print();


	Dados();
	virtual ~Dados();

};

#endif /* DADOS_H_ */
