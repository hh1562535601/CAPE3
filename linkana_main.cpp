#include "linkana.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	pagerank *pr=new pagerank();
	//cout << sizeof(float) << endl; 
	
	//pr->init();
	//pr->set();
	//cout << pr->max_iterations << endl;
	pr->insert_nodes();
	pr->insert_edges();
	pr->cal_outdegree();
	//cout << pr->edges.size() << endl;
	pr->matrix_construct(); //insert_edges() has to be done
	pr->calculate();
	pr->print();
	
	return 0;
}
