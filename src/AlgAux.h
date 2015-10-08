/*
 * AlgAux.h
 *
 *  Created on: 24/08/2015
 *      Author: Lettiery
 */

#include "Dados.h"
#include "Solucao.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef ALGAUX_H_
#define ALGAUX_H_

using namespace std;

class Alg_Aux {


public:
	/*retorna a posicao que deve ser inserido, e aplica o shift para liberar essa posicao se for necessario*/
	static int insertion(int insert, vector<int> vetor, int null) {
		if (vetor[vetor.size() - 1] != null) {
			//nao pode ser inserido, o vetor esta cheio
			//apaga o ultimo
			vetor[vetor.size() - 1] = null;
		}
		int j, i, pos;
		for (j = 0; j < vetor.size(); j++) {
			if (vetor[j] > insert) {
				pos = j;			//var ser inserido em j
				//aplicar shift no vetor
				for (i = vetor.size(); i >= j; i--) {
					if (vetor[i] != null) {
						vetor[i + 1] = vetor[i];
					}
					vetor[j + 1] = vetor[i];
				}
				return pos;
			}
		}
		return vetor.size() - 1;
	}


	/*retorna uma lista com todas as distancia de folga do inicio a te o vertice
	 * e preenche o parametro caminho com os vertices do menor caminho*/
	/*o ultimo da lista é o fim */
	static list<int> * diskjtra_folga(Solucao * s, int inicio, int fim, list<int > *caminho) {

		vector<bool> vertces_existentes (s->d->j, false);
		vector<bool> vertces_visitados (s->d->j, false);// que ja foram visitados todos os sucessores

		vector<int> pai (s->d->j, -1);
		vector<int> dist (s->d->j, INT_MAX);

		vertces_existentes[inicio] = true;
		pai[inicio] = inicio;
		dist[inicio] = 0;

		int u  = inicio;
		while(u != -1 && u != fim ){

			list<int>::iterator su = s->d->S[u].begin();
			for(;su!=s->d->S[u].end();su++){

				if(!vertces_existentes[*su]){// nao contabilizo nenhum custo ate *su
					pai[*su] = u;
					dist[*su] = dist[u] +  s->Ti[*su] - (s->Ti[u] + s->D[u]);
					vertces_existentes[*su] = true;
				}else{// *su ja tem um valor de dist, então agora deve ver qual valor é o menor, o atual ou o novo
					if(dist[*su] > dist[u] +  s->Ti[*su] - (s->Ti[u] + s->D[u])){// deve ser trocado
						pai[*su] = u;
						dist[*su] = dist[u] +  s->Ti[*su] - (s->Ti[u] + s->D[u]);
					}// caso contrario nao faz nada
				}

			}

			vertces_visitados[u] = true;// visitei o vertice u


			int menor_atual = -1;
			int menor_dist = INT_MAX;
			for(int v = 0; v < s->d->j ; v++){
				if(vertces_existentes[v] && !vertces_visitados[v]){//esse vertice existe e ainda nao foi visitado
					if(menor_atual == -1){
						menor_atual = v;
						menor_dist = dist[v];
						continue;
					}

					// então ele é um possivel candicado a ser visitado agora
					if(dist[v] < menor_dist){
						menor_dist = dist[v];
						menor_atual = v;
					}
				}
			}
			// ao fim desse for tenho o novo vertice a ser visitado
			u = menor_atual;
		}

		list<int> * distance = new list<int>();
		if(u == -1){
			cout << "dijkstra encontrou um ciclo nas precedências, ou não existe caminho de "<<inicio << " até "<< fim<< endl;
		}else if(u == fim){// existe caminho de inicio ate fim e a folga total é dist[fim]

			int vt = fim;

			while(vt != inicio){

				(*caminho).push_front(vt);// ver se da pra colocar o vertice e a folga até ele (dist[vt])
				distance->push_front(dist[vt]);
				vt = pai[vt];
			}
		}

		for(int at = 0;at < s->d->j;at++){
			if(vertces_existentes[at]){
				cout << at+1 << "| ";
			}

		}cout <<endl;

		return distance;
	}


	/*retorna um vetor de bool com todas atividades sucessoras diretas e indiretas (true) até a  atividade ficticia d->j-1
	 * a atividade inicio e fim do caminho tambem entram
	 * */
	static vector<bool> * atividades_posteriores(Dados * d, int atividade) {


			vector<bool> * vertces_existentes = new vector<bool> (d->j, false);
			vector<bool> vertces_visitados (d->j, false);// que ja foram visitados todos os sucessores

			vector<int> pai (d->j, -1);

			(*vertces_existentes)[atividade] = true;
			pai[atividade] = atividade;

			int u  = atividade;
			while(u != -1 && u != d->j-1 ){

				list<int>::iterator su = d->S[u].begin();
				for(;su!= d->S[u].end();su++){
					pai[*su] = u;
					(*vertces_existentes)[*su] = true;

				}

				vertces_visitados[u] = true;// visitei o vertice u


				int nao_visitado = -1;
				for(int v = 0; v < d->j ; v++){
					if((*vertces_existentes)[v] && !vertces_visitados[v]){//esse vertice existe e ainda nao foi visitado
						nao_visitado = v;
						break;
					}
				}
				// ao fim desse for tenho o novo vertice a ser visitado
				u = nao_visitado;
			}

			return vertces_existentes;
		}

};

#endif /* ALGAUX_H_ */
