/*
 * arquivo.h
 *
 *  Created on: 18/11/2014
 *      Author: Leo
 */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "Dados.h"
#include "Solucao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

class Arquivo{

	public:
		ifstream dados;
		ifstream funcao;
		string nomeArquivo;
		int numeroInstacia;

		Dados * lerInstancia();

		void gravarTabelaDeResultados(int instacia, float custo, int makespan);
		void gravarSolucao(int instancia, Solucao * s);


		Arquivo(string nomeArquivo, int numero);
		~Arquivo();
};



#endif /* ARQUIVO_H */
