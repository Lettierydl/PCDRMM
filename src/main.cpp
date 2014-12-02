//============================================================================
// Name        : PCDRMM.cpp
// Author      : Lettiery DLamare
// Version     :
// Copyright   : UFPB
// Description : Problema de Custo de Disponibilidade de Recruso com Multiplos Modos
//============================================================================

#include "Arquivo.h"

#include <iostream>
#include "stdlib.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include<time.h>

using namespace std;

int main(int argc, char **argv) {


	for(int i = 1; i<=1;i++){

		string instancia = "Instancias_Denise";
		Arquivo arq(instancia, i);
		arq.lerInstancia();

	}

	return 0;
}
