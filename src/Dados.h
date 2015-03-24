/*
2 * Dados.h
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
	int numeroInstacia;//apenas para identificar qual instancia se refere

	int tipos ; // m quatidades de tipos diferentes de recrusos 4
	int j ; // n quatidades de atividades mais as atividades virtuais
	int D ; // D data maxima de entrega do projeto
	vector <int> M;// Mj modos de execulcao da ativiade j
	vector< list<int> > H;// Hj, precedencia das atividades, a atividade j, tem a lista de atividade que precedem ela
	vector <list<int> > S; // Sj sucessores de j
	vector < vector<int> > d;// dji duracao da atividade j(linha) quando execultada no modo i (coluna), j X Mj
	vector < vector < vector<int> > > r;// rjik, quantidade de recursos, que a atividade j, utiliza no modo i, do tipo k
	vector<int>disponibilidade;
	vector <float>custo_recurso;// custo de cada recurso para ser utilizado na funcao c();


	void print();


	Dados(int tipos, int j, int D);
	virtual ~Dados();

};

#endif /* DADOS_H_ */
