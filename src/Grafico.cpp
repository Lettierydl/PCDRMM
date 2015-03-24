/*
 * Grafico.cpp
 *
 *  Created on: 13/03/2015
 *      Author: Leo
 */

#include "Grafico.h"
#include "Solucao.h"
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include "include/gnuplot_i.hpp"

using namespace std;

void Grafico::plotarGraficoDaSolucao(Solucao *s) {

	Gnuplot g1("Solução");

	stringstream ss;
	ss << "Solução Nº " << s->d->numeroInstacia << "\\nTempo X Atividade";

	g1.set_title(ss.str());

	g1.set_xlabel("Tempo");
	g1.set_ylabel("Atividade");

	g1.set_xrange(0, s->tempo + (s->tempo / 3)); //espaco mostrado pelo grafico
	g1.set_yrange(0, s->d->j + 2);

	std::vector<double> x, y, dj;

	g1.set_pointsize(0.008).set_style("points");
	stringstream gridx;
	stringstream gridy;
	gridx << "set xtics (\"0 0\" 0.0 ";

	for (int j = 1; j < s->d->j; j++) {
		gridx << ",\"" << s->Ti[j] << "\" " << s->Ti[j] << ".0 ";
		gridx << ",\"" << (s->Ti[j] + s->d->d[j][s->M[j]]) << "\" "
				<< (s->Ti[j] + s->d->d[j][s->M[j]]) << ".0 ";

	}
	gridx << ")";

	g1.cmd(gridx.str());
	g1.set_grid();

	for (int j = 1; j < s->d->j; j++) {
		x.push_back((double) j);
		y.push_back((double) s->Ti[j] + (s->d->d[j][s->M[j]] / 2.0));
		dj.push_back((double) s->d->d[j][s->M[j]] / 2.0);

		stringstream st;
		if (j != s->d->j - 1) {
			st << "Atividade " << j;
		} else {
			st << "Makespan";
		}

		g1.plot_xy_err(y, x, dj, st.str());

		x.clear(), y.clear();
		dj.clear();

	}

	/*

	 Gnuplot g2("Solução");

	 stringstream sr;
	 sr << "Solução Nº " << s->d->numeroInstacia << "\\nTempo X Recursos";

	 g2.set_title(sr.str());

	 g2.set_xlabel("Tempo");
	 g2.set_ylabel("Recursos");

	 g2.set_xrange(0, s->tempo+1 ); //espaco mostrado pelo grafico
	 g2.set_yautoscale();

	 g2.cmd("set style fill transparent solid 0.5 noborder");
	 g2.cmd("set boxwidth 0.1 absolute");

	 g2.set_grid();

	 std::vector<double> x, y, dj;

	 for (int k = 0; k <= s->d->tipos; k++) {
	 switch (k) {
	 case 0:
	 g2.set_style("boxes linecolor rgb \"red\"");
	 break;

	 case 1:
	 g2.set_style("boxes linecolor rgb \"blue\"");
	 break;

	 case 2:
	 g2.set_style("boxes linecolor rgb \"green\"");
	 break;

	 case 3:
	 g2.set_style("boxes linecolor rgb \"yellow\"");
	 break;
	 default:
	 g2.set_style("boxes linecolor rgb \"black\"");
	 break;
	 }
	 for (int t = 0; t <= s->tempo; t++) {
	 x.push_back((double) t + (k*0.109) );
	 y.push_back((double) s->tr[k][t]);

	 stringstream sr;
	 sr <<""<< k;

	 g2.plot_xy(x,y,"");

	 x.clear(), y.clear();
	 dj.clear();
	 }
	 }
	 */

}

void Grafico::plotarTrandOFF(list<Solucao> solucoes) {
	vector<float> custo;
	vector<int> tempo;

	for (list<Solucao>::iterator it = solucoes.begin(); it != solucoes.end();
			it++) {
		custo.push_back(it->custo);
		tempo.push_back(it->tempo);
	}
	plotarTrandOFF(custo, tempo);
}

void Grafico::plotarTrandOFF(vector<float> custo, vector<int> tempo) {
	Gnuplot g1("Trand-OFF");

	stringstream ss;
	ss << "Trand-OFF de Soluções\\nTempo X Custo";

	g1.set_title(ss.str());

	g1.set_xlabel("Tempo");
	g1.set_ylabel("Custo");

	g1.set_xautoscale();
	g1.set_yautoscale();

	/*
	stringstream gridx;
	stringstream gridy;
	gridx << "set xtics (\"" << tempo[0] << "\"" << tempo[0];
	gridy << "set ytics (\"" << custo[0] << "\"" << custo[0];

	for (int s = 1; s < custo.size(); s++) {
		gridx << ",\"" << tempo[s] << "\" " << tempo[s] << " ";
		gridy << ",\"" << custo[s] << "\" " << custo[s] << " ";

	}
	gridx << ")";
	gridy << ")";

	g1.cmd(gridx.str());
	g1.cmd(gridy.str());*/

	g1.set_grid();

	g1.set_pointsize(3).set_style("points pt 1");

	g1.plot_xy( tempo, custo,"Soluções");

	g1.set_style("lines lt 2 lc 3");

	g1.plot_xy( tempo, custo,"Curva de Trand-OFF");

}

Grafico::Grafico(){

}

Grafico::~Grafico() {
	// TODO Auto-generated destructor stub
}

