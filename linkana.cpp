#include "linkana.h"


pagerank::pagerank(double a, double c, size_t i):
			       alpha(a), convergence(c), max_iterations(i){};
		
bool pagerank::matrix_construct()	
{
	cout << "matrix_construct()" << endl;
	
	multimap<size_t, size_t>::iterator it1;
	map<size_t, size_t>::iterator it2;
	size_t num_outgoing;
	
	for(it1 = edges.begin(); it1 != edges.end(); it1++)
	{
		//enlarge the size of edges map if not enough
		if(it1->second > matrix.size())
		{
			matrix.resize(it1->second);			
		}
		
		triple tp;
		tp.from = it1->first;
		tp.to = it1->second;
		
		//set probability of triple
		it2=outdegree.find(it1->first);
		if(it2 != outdegree.end())
		{
			num_outgoing=it2->second;														
			tp.probability = 1.0/num_outgoing; //the probability equals 100% divides the outdegree of node
		}
		else
		{
			tp.probability = 0;
		}
			
		matrix[it1->second-1].push_back(tp);	//the tp.to -1 is the row index while the tp.from is the col index
	}
}
    
bool pagerank::pagerank::calculate()    
{
	double diff = 1.0, sum = 0.0;
	//cout << diff << endl;
	size_t num_iterations = 0;
	size_t i, j, num_outgoing;
	
	//initialize
	pr.reserve(nodes_to_idx.size());
	
	for(i=0;i<nodes_to_idx.size();i++)
	{
		pr.push_back(1/nodes_to_idx.size());
	}
	
	while (diff > convergence && num_iterations < max_iterations) 
	{	
		cout << "num_iterations:" << num_iterations << endl;
		diff=0;
		
		map<size_t, size_t>::iterator it1;
		map<size_t, size_t>::iterator it2;
		
		for(i=0;i<matrix.size();i++)
		{
				sum = 0;			
				//struct timeval start, end; 
  				//gettimeofday(&start, NULL);
  				
  				//the row of probability matrix multiplies pagerank vector 
				for(j=0;j<matrix[i].size();j++)
				{
						size_t col=matrix[i][j].from - 1;
						
						sum+=matrix[i][j].probability*pr[col];															
				}
				//gettimeofday(&end, NULL);
				//int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec; 
				//cout << "runtime:" << timeuse << endl;	
				
				double old_pr=pr[i];
				//cout << "sum:" << sum << endl;
				pr[i]=sum*(1-alpha)+alpha/nodes_to_idx.size();  //randomize
				
				diff+=fabs(pr[i]-old_pr);   //used to test whether it's convergent
				
				/*if(fabs(pr[i]-old_pr) < 0 || diff < 0)
				cout << "diff:" << diff << endl
				     << "fabs(pr[i]-old_pr):" << fabs(pr[i]-old_pr) << endl;*/								
		}
		
		num_iterations++;
		//cout << "while_num_iterations:" << num_iterations << endl;
		//cout << "while_diff:" << diff << endl;
	}
	//cout << "leave calculate()" << endl;
	return true;
}

/*bool pagerank::pagerank::calculate()
{
	double diff = 1.0, sum = 0.0;
	//cout << diff << endl;
	size_t num_iterations = 0;
	size_t i, j, num_outgoing;
	
	pr.reserve(nodes_to_idx.size());
	
	for(i=0;i<nodes_to_idx.size();i++)
	{
		pr.push_back(1/nodes_to_idx.size());
	}
	
	while (diff > convergence && num_iterations < max_iterations) 
	{		
		diff=0;
		
		map<size_t, size_t>::iterator it1;
		map<size_t, size_t>::iterator it2;
		
		for(i=1;i<=nodes_to_idx.size();i++)
		{
				sum = 0;			
				struct timeval start, end; 
  				gettimeofday(&start, NULL);
				for(j=1;j<=nodes_to_idx.size();j++)
				{
						
  						
						if((it1 = edges.find(j) )!= edges.end())
						{
							while(it1->second != i)
							{
								it1++;
								
								if(it1->first != j)
									break;
							}
							
							if(it1->first != j)
								continue;
								
							it2=outdegree.find(j);
							num_outgoing=it2->second;
							sum+=pr[i]/num_outgoing;
						}
						
					
				}
				gettimeofday(&end, NULL);
				int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec; 
				cout << "runtime:" << timeuse << endl;	
				
				double old_pr=pr[i];
				
				pr[i]=sum*(1-alpha)+alpha/nodes_to_idx.size();
				
				diff+=fabs(pr[i]-old_pr);
				if(fabs(pr[i]-old_pr) < 0 || diff < 0)
				cout << "diff:" << diff << endl
				     << "fabs(pr[i]-old_pr):" << fabs(pr[i]-old_pr) << endl;								
		}
		
		num_iterations++;
		cout << "while_num_iterations:" << num_iterations << endl;
		cout << "while_diff:" << diff << endl;
	}
	//cout << "leave calculate()" << endl;
	return true;
}
*/

bool pagerank::insert_nodes()
{
	cout << "insert_nodes()" << endl;
	
	string url;
	size_t id;
	ifstream in("id_url.txt"); 
	 
	//in.open("matrix.txt", ios::in) ;
	
	if(!in.is_open())
	{
		cout << "file open error!" << endl;
		exit(0);
	}

	while(!in.eof())
	{
		in >> id >> url;
		nodes_to_idx[url] = id; //may rewrite!
		idx_to_nodes[id] = url;
		//cout << "url:" << url << "   " << "id:" << id << endl;
	}	
	
	in.close();
	//cout << "leave insert_nodes()" << endl;
	return true;
}

bool pagerank::insert_edges()
{
	cout << "insert_edges()" << endl;
		
	char c;
	size_t id1, id2;
	ifstream in("matrix.txt"); 
	 
	//in.open("matrix.txt", ios::in) ;
	
	if(!in.is_open())
	{
		cout << "file open error!" << endl;
		exit(0);
	}

	while(!in.eof())
	{
		in >> c >> id1 >> c >> id2 >> c;
		edges.insert(make_pair(id1, id2));
		//cout << "id1:" << id1 << "   " << "id2:" << id2 << endl;
	}	
	
	in.close();
	//cout << "leave insert_edges()" << endl;
	return true;
}

bool pagerank::cal_outdegree()
{
	cout << "cal_outdegree()" << endl;
	
	map<size_t, size_t>::iterator it;
	
	for(it=edges.begin();it!=edges.end();it++)
	{
		outdegree[it->first]++; 
		//cout << "id:" << it->first << "   " << "outdegree:" << outdegree[it->first] << endl;
	}
	//cout << "leave add_outdegree()" << endl;
	return true;
}
    	   	
bool pagerank::print()
{
	int i, max_n = 0;
	size_t id;
	double max = 0.0;
	map<string, size_t>::iterator it;
	ofstream of("result.txt");
	
	if(!of.is_open())
	{
		cout << "file open error!" << endl;
		exit(0);
	}
	
	cout << "alpha:" << alpha << endl
		 << "convergence:" << convergence << endl
		 << "max_iterations:" << max_iterations << endl;
	
	/*for(i = 0; i < 5; i++)
	{
		max[i] = 0.0;
	}
	cout << "pr[10]:" << pr[10] << endl;*/
	
	//find the maximum value of the first five
	while(max_n < 5) 
	{
		max = 0.0;
		
		for(i=0;i<pr.size();i++)
		{
			if(pr[i] > max)
			{
				max = pr[i];
				id = i+1;
			}
		}
		
		cout << idx_to_nodes.find(id)->second << endl;
		of << id << "   " << pr[id-1] << "  " << idx_to_nodes.find(id)->second << endl;
		pr[id-1] = 0; //so that we can find the next maximum value
		
		max_n++;
	}
	
	/*	cout << "The top 10 pagerank values are:" << endl;
	
	sort(pr.begin(), pr.end());
	
	for(i = pr.size()-1; i >= pr.size() - 5; i--)
	{
			max[i]=pr[i];
	}*/
	
	return true;
}

bool pagerank::set(double a, double c, size_t i)
{
	alpha = a;
	convergence = c;
	max_iterations = i;
	
	return true;
}

bool pagerank::init(double, double, size_t)
{
	
}

Orthlist& Orthlist::matrix_mutiply(Orthlist &poll, Orthlist &polr)
{
	int i, j;
	
	for(i=0;i<poll.row_count;i++)
	{
		Orthnode *onl=poll.prow[i];
		int value=0;
		
		for(j=0;j<polr.col_count;j++)
		{
			Orthnode *onr=polr.pcol[j];
			
			while(onl && onr)
			{
				if(onl->col == onr->row)
				{
					value+=((onl->value)*(onr->value));
				}
				else if(onl->col > onr->row)
				{
					onl=onl->right;
				}
				else
				{
					onr=onr->down;
				}
			}
			
			if(value != 0)
			{
				Orthnode p(value,onl->row,onr->col,NULL,NULL);
			}
		}
	}
}

int insert(Orthlist &matrix, Orthnode &node)
{
	int i;
	
	for(i=0;i<matrix.row_count;i++)
	{
		
	}
}
