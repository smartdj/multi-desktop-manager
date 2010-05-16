#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
namespace Algorythms
{
	
	int insert_sort(int Array[],int Length)
	{
		for( int i = 0 ; i < Length ; i ++ )
		{
			int p = i;
			for( int j = i ; j < Length ; j ++ )
			{
				if(Array[j] < Array[p])
				{
					p = j;
				}
			}
			if(p != i)  //Swap
			{
				int tmp = Array[p];
				Array[p] = Array[i];
				Array[i] = tmp;
			}
		}
		return 0;
	}
	
	int Merge(int Array[],int p,int q,int r)
	{
		int a = q - p + 1;
		int b = r - q;
		int *L = new int[a + 1];
		int *R = new int[b + 1];
		int i  = 0;
		int j = 0;
		for(i = 0; i < a; i ++)
		{
			L[i] = Array[p+i];
		}
		for(i = 0; i < b; i ++)
		{
			R[i] = Array[q+i+1];
		}
		L[a] = 0x7FFFFFF;
		R[b] = 0x7FFFFFF;
		i = 0;
		j = 0;
		for(int k = p; k <= r ; k ++)
		{
			if(L[i] <= R[j])
			{
				Array[k] = L[i];
				i ++;
			}
			else
			{
				Array[k] = R[j];
				j ++;
			}
		}
		return 0;
	}
	int Merge_Sort(int Array[],int p,int r)
	{
		if(p < r)
		{
			int q = (p + r) / 2;
			Merge_Sort(Array,p,q);
			Merge_Sort(Array,q+1,r);
			Merge(Array,p,q,r);
		}
		return 0;
	}
	int Reserver(int Array[],int Length)
	{
		int * e = Array + Length -1;
		int * f = Array;
		while(f < e)
		{
			int tmp = *f;
			*f = *e;
			*e = tmp;
			f ++;
			e --;
		}
		return 0;
	}
	int Exchange(int *a,int *b)
	{
		int tmp = *a;
		*a = *b;
		*b = tmp;
		return 0;
	}

	int Exchange(double *a,double *b)
	{
		double tmp = *a;
		*a = *b;
		*b = tmp;
		return 0;
	}

	int Bubble_Sort(int Array[],int Length)
	{
		for(int i = 0; i < Length - 1; i ++)
		{
			for(int j = Length - 1; j >= i + 1; j --)
			{
				if(Array[j] < Array[j - 1])
				{
					Exchange(Array + j,Array + j - 1);
				}
			}
		}
		return 0;
	}


	int Heap_Parent(int i)
	{
		return i << 2;
	}
	int Heap_Left(int i)
	{
		return i + i;
	}
	int Heap_Right(int i)
	{
		return i + i + 1;
	}
	int Max_HeapIFY(int Array[],int i,int heapsize)
	{
		int l = Heap_Left(i);
		int r = Heap_Right(i);
		int largest = 0;
		if(l < heapsize && Array[l] > Array[i])
		{
			largest = l;
		}
		else
		{
			largest = i;
		}
		if(r < heapsize && Array[r] > Array[largest])
		{
			largest = r;
		}
		if(largest != i)
		{
			Exchange(Array + i,Array + largest);
			Max_HeapIFY(Array,largest,heapsize);
		}
		return 0;
	}
	int Build_Max_Heap(int Array[],int Length)
	{
		int heapsize = Length;
		for(int i = Length/2; i >=0 ; i --)
		{
			Max_HeapIFY(Array,i,heapsize);
		}
		return 0;
	}
	int Heap_Sort(int Array[],int Length)
	{
		Build_Max_Heap(Array,Length);
		int heapsize = Length ;
		for(int i = Length - 1; i > 0 ; i --)
		{
			Exchange(Array,Array + i);
			heapsize = heapsize - 1;
			Max_HeapIFY(Array,0,heapsize);
		}
		return 0;
	}
	int Average_Minimum_Query_Time(int L[],double H[],int Result[],int Length)
	{
		double *f = (double *)malloc(Length * sizeof(double));
		for(int i = 0 ; i < Length ; i ++)
		{
			f[i] = (H[i] * 100.0) / (L[i] * 1.0);
			Result[i] = i;
		}
		for(int i = 0 ; i < Length; i ++)
		{
			int p = i;
			for(int j = i + 1; j < Length; j ++)
			{
				if(f[j] > f[p])
				{
					p = j;
				}
			}
			
			if(p != i)
			{
				Exchange(f + i,f + p);
				Exchange(Result + i,Result + p);
			}
		}
		return 0;
	}
	namespace Template
	{
		#define MAX_NODE 4096
		typedef struct NODE
		{
			int id;
		}Node,*PNode;

		typedef struct EDGE
		{
			int v;
			int u;
			int w;
		}Edge,*PEdge;

		class Graphic
		{
		public:
			Graphic()
			{
				this->edge = (EDGE*)malloc(MAX_NODE * sizeof(EDGE));
				this->edges = 0;
				this->node = (NODE*)malloc(MAX_NODE * sizeof(NODE));
				this->nodes = 0;
			}
			Graphic(int edges,int nodes)
			{
				this->edge = (EDGE*)malloc((MAX_NODE + edges) * sizeof(EDGE));
				this->edges = 0;
				this->node = (NODE*)malloc((MAX_NODE + nodes) * sizeof(NODE));
				this->nodes = 0;
			}
		private:
			NODE *node;
			EDGE *edge;
			int edges;
			int nodes;
		public:
			int AddNodes(int num)
			{
				for(int i = 0 ; i < num ; i ++ )
				{
					this->node[i].id = i;
				}
				this->nodes = num;
				return 0;
			}
			int AddEdge(int u,int v,int w)
			{
				this->edge[this->edges].u = u;
				this->edge[this->edges].v = v;
				this->edge[this->edges].w = w;
				this->edges ++;
				return 0;
			}
			int SortEdgeByWeight()
			{
				for(int i = 0; i < this->edges; i ++)
				{
					int p = i;
					for(int j = i+1; j < this->edges; j++)
					{
						if(this->edge[j].w < this->edge[p].w)
						{
							p = j;
						}
					}
					if(p != i)
					{
						EDGE tmp;
						tmp.u = this->edge[p].u;
						tmp.v = this->edge[p].v;
						tmp.w = this->edge[p].w;
						
						this->edge[p].u = this->edge[i].u;
						this->edge[p].v = this->edge[i].v;
						this->edge[p].w = this->edge[i].w;

						this->edge[i].u = tmp.u;
						this->edge[i].v = tmp.v;
						this->edge[i].w = tmp.w;
					}
				}
				return 0;
			}
			EDGE GetEdge(int index)
			{
				if(index >= this->edges)
				{
					
				}
				return this->edge[index];
			}
			int AddEdge(EDGE edge)
			{
				this->edge[this->edges].u = edge.u;
				this->edge[this->edges].v = edge.v;
				this->edge[this->edges].w = edge.w;
				this->edges ++;
				return 0;
			}
			int CountOfNode()
			{
				return this->nodes;
			}
			int CountOfEdge()
			{
				return this->edges;
			}
		};
		
		typedef struct WEIGHT
		{
			Edge edge[MAX_NODE];
			int count;
		}Weight,*PWeight;

		int Generic_MST(Graphic G,Weight W[])
		{
			/*
			A <- NULL
			while A does not form a spanning tree
				do find an edge(u,v) that is safe for A
					A <- A U {(u,v)}
			return A
			*/
			return 0;
		}
		class SET
		{
		private:
			bool Content[MAX_NODE];
		public:
			bool has(int one)
			{
				return(one < MAX_NODE)?
					 Content[one]:false;
			}
			bool insert(int one)
			{
				if(one < MAX_NODE)
					Content[one] = true;
				return (one < MAX_NODE)? Content[one]:false;
			}
			bool remove(int one)
			{
				if(one < MAX_NODE)
					Content[one] = false;
				return (one < MAX_NODE)? !Content[one]:false;
			}
		};
		class  SETS
		{
		public:
			SETS()
			{
				
				this->count = MAX_NODE;
				sets = (int *)malloc(sizeof(int)*MAX_NODE);
				for(int i = 0 ; i < count ; i ++)
				{
					sets[i] = i;
				}
			}
			SETS(int count)
			{
				this->count = count;
				if(count < MAX_NODE)
				{
					sets = (int *)malloc(sizeof(int)*MAX_NODE);
				}
				else
				{
					sets = (int *)malloc(sizeof(int)*(count + MAX_NODE));
				}
				for(int i = 0 ; i < count ; i ++)
				{
					sets[i] = i;
				}
			}
			~SETS()
			{
				free(sets);
			}
		private:
			int *sets;
			int count;
		public:
			int found(int one)
			{
				if(one >= count)
				{
					return -1;
				}
				return sets[one];
			}
			int join(int a,int b)
			{
				if(a >= count || b >= count)
				{
					return -1;
				}
				int newset = -1;
				int change = -1;
				if(sets[a]<sets[b])
				{
					newset = sets[a];
					change = sets[b];
				}
				else
				{
					newset = sets[b];
					change = sets[a];
				}
				for(int i = 0 ; i < count ; i ++)
				{
					if(sets[i] == change)
					{
						sets[i] = newset;
					}
				}
				return 1;
			}
		};
		Graphic MST_KRUSKAL(Graphic g)
		{
			Graphic A = Graphic();
			SETS sets = SETS(g.CountOfNode());
			g.SortEdgeByWeight();
			for(int i = 0; i < g.CountOfEdge(); i ++)
			{
				if(sets.found(g.GetEdge(i).u) != sets.found(g.GetEdge(i).v))
				{
					A.AddEdge(g.GetEdge(i));
					sets.join(g.GetEdge(i).u,g.GetEdge(i).v);
				}
			}
			return A;
		}
	}
}