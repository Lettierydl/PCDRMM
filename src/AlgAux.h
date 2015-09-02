/*
 * AlgAux.h
 *
 *  Created on: 24/08/2015
 *      Author: Lettiery
 */

#ifndef ALGAUX_H_
#define ALGAUX_H_

using namespace std;

class Alg_Aux {


	/*retorna a posicao que deve ser inserido, e aplica o shift para liberar essa posicao se for necessario*/
	static int inserction(int insert, vector <int> vetor, int null ) {
		if(vetor[vetor.size()-1] != null){
			//nao pode ser inserido, o vetor esta cheio
			//apaga o ultimo
			vetor[vetor.size()-1] = null;
		}
		int j, i, pos;
		for (j = 0; j < vetor.size(); j++) {
			if(vetor[j] > insert){
				pos = j;//var ser inserido em j
				//aplicar shift no vetor
				for(i = vetor.size(); i >= j ; i--){
					if(vetor[i] != null){
						vetor[i+1] = vetor[i];
					}
					vetor[j + 1] = vetor[i];
				}
				return pos;
			}
		}
		return vetor.size()-1;
	}






};

#endif /* ALGAUX_H_ */
