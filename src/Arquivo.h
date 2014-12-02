/*
 * arquivo.h
 *
 *  Created on: 18/11/2014
 *      Author: Leo
 */

#ifndef ARQUIVO_H
#define ARQUIVO_H

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

		void lerInstancia();


		Arquivo(string nomeArquivo, int numero);
		~Arquivo();
};



#endif /* ARQUIVO_H */
