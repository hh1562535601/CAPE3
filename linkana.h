#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <stdlib.h>
//#include <algorithm>
//#include <time.h>
//#include <sys/time.h>

#define DEFAULT_ALPHA 0.15
#define DEFAULT_CONVERGENCE 0.0001
#define DEFAULT_MAX_ITERATIONS 10

using namespace std;

/*
typedef struct Orthnode
{
	int value;
	int row,col;
	struct Orthnode *right,*down;
}Orthnode;

typedef struct
{
	int count;
	Orthnode *row,head;
}Orthlist;
*/

typedef struct 
{
	size_t from, to;
	double probability;
}triple;

class Orthnode
{
	friend class Orthlist;
	
	public:
	//private:
		int value;
		int row, col;
		Orthnode *right, *down;

	public:
		Orthnode():value(0), row(-1), col(-1), right(NULL), down(NULL){}
		Orthnode(int avalue, int arow, int acol, Orthnode *aright, Orthnode *adown):value(avalue),row(arow),col(acol),right(aright),down(adown){}
				
};

class Orthlist
{
	public:
		int row_count, col_count, total_count;
		Orthnode **prow, **pcol;
		
	public:
		Orthlist():row_count(0),col_count(0),total_count(0),prow(NULL), pcol(NULL){}
		
		Orthlist& matrix_mutiply(Orthlist &poll, Orthlist &polr);	
		
		int insert(Orthlist &matrix, Orthnode &node);		
};


class pagerank
{
	private:	
	//public:
    	double convergence;  
    	size_t max_iterations;
    	double alpha; // the pagerank damping factor
    	map<string, size_t> nodes_to_idx; // mapping from string node IDs to numeric
    	map<size_t, string> idx_to_nodes; // mapping from numeric node IDs to string
    	map<size_t,size_t> outdegree;//the outdegree of each edge
    	//multimap<string,string> arcs;
    	multimap<size_t,size_t> edges; //the edges between nodes
    	//vector<size_t> num_outgoing; // number of outgoing links per column
    	//vector<double> vf;
		//vector<vf> vvf;		
    	vector<double> pr; // the pagerank table
    	vector<triple> row;  //the row of matrix
    	vector<vector<triple> > matrix; //the probability matrix
    	
    public:
    	pagerank(double a = DEFAULT_ALPHA, double c = DEFAULT_CONVERGENCE,
          	  size_t i = DEFAULT_MAX_ITERATIONS);
          	  
    	bool insert_nodes();   //insert nodes into map named nodes_to_idx
    	bool insert_edges();   //insert edges into map named edges
    	bool cal_outdegree();  //calculate the outdgeree of each node
    	
    	bool calculate();  //calculate the pagerank value of each node
    	
    	bool print();  //print some information to the standard output and files
    	bool set(double a = DEFAULT_ALPHA, double c = DEFAULT_CONVERGENCE,
          	    size_t i = DEFAULT_MAX_ITERATIONS);
        bool init(double a = DEFAULT_ALPHA, double c = DEFAULT_CONVERGENCE,
          	      size_t i = DEFAULT_MAX_ITERATIONS);    
		bool matrix_construct();

};


