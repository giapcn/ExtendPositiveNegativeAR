#include "StdAfx.h"
#include "Charm_business.h"

Charm_business::Charm_business(void)
{
}

Charm_business::~Charm_business(void)
{
}
int Charm_business::Charm_main()
{
	int i =0;
	i = this->Charm_extend(&this->head);
	return i;
}
int Charm_business:: init_not(db_pattern example_db, double min_sup)
{
	/*khoi tao tat ca cac tham so can thiet*/
	this->C.init();

	this->min_level= min_sup;// minimum supporting level
	//tim cac items co so lan suat hien lon hon this->min_level;
	int i = 0;//item accessing variable
	int point=0; // real branch variable
	this->total_Tid= example_db.real_Tid;
	for (i=0;i<example_db.real_item;i++)// duyet tung item
	{
		int count=0;
		head.pointer_set[point]= new(tr_charm);
		for(int j=0; j<example_db.real_Tid ;j++)
		{
			if(example_db.db_inst[j][i]!=0) 
			{
				head.pointer_set[point]->id_set[count]=j;// lay tap IDset
				count++;
			}
		}
		double sup = (double)count/(double)this->total_Tid;
		if (this->min_level <= sup)//(count/this->total_Tid))// tao cac nut con dau tien
		{
			head.pointer_set[point]->it_set [0]=i;// lay ten item
			head.pointer_set[point]->real_item=1;//luon co 1 item o muc nay
			head.pointer_set[point]->real_tid=count;
			head.pointer_set[point]->real_branch=0;// chua co nhanh con nao
			point++;
		}
		else delete head.pointer_set [point];// giai phong con tro da cap
	}
	//
	head.real_branch= point;
	head.real_item=1;// thu thuat de goi de qui ham
	head.real_tid=0;
	return 0;
}
int Charm_business::Charm_extend( tr_charm * node)
{
	// ghep cach con cung cap

	for (int i=0; i< node->real_branch;i++)
	{
		if(node->pointer_set[i]->real_item!=0)
		{
			// giai quyet cho tung nut: node->pointer[i]
			int branch_count=0;
			// bien so sanh
			tr_charm *cp_varial = new tr_charm; // bien dung de luu tru tham so so sanh
			cp_varial->real_branch= node->pointer_set[i]->real_branch;
			cp_varial->real_item= node->pointer_set[i]->real_item;
			cp_varial->real_tid= node->pointer_set[i]->real_tid;
			for (int c1=0; c1< cp_varial->real_item; c1++) cp_varial->it_set[c1]=node->pointer_set[i]->it_set[c1];
			for (int c1=0; c1< cp_varial->real_item; c1++) cp_varial->id_set[c1]=node->pointer_set[i]->id_set[c1];
			//
			for(int j=i+1; j<node->real_branch;j++)
			{
				if(node->pointer_set[j]->real_item!=0)
				{
					if(Item_compare(cp_varial,node->pointer_set[j])==0)
					{
						// hai nut co cung tap luat phia truoc (in general: f(Xj) > f(Xi))
						tr_charm *P= new tr_charm;
						*P= blend(*node->pointer_set[i],*node->pointer_set[j]);
						int status =0;
						if((double)P->real_tid/(double)this->total_Tid>=this->min_level)
						{
							// la tap pho bien
							status= Charm_property(node->pointer_set[i],node->pointer_set[j],P);
						}
						else 
						{
							delete P;//node->pointer_set[i]->pointer_set[branch_count];
						}
						if (status==1 || status==2) delete P;
					}
				}
			}
			Charm_extend( node->pointer_set[i]);
			this->C.check_subsumed(node->pointer_set[i]);
		}
	/*	for (int br=0; br<node->real_branch; br++)
		{
		}*/
	}
	return 0;
}
int Charm_business::Item_compare(tr_charm *node1,tr_charm *node2)// so sanh luat 2 nut
{
	for (int i=0;i<node1->real_item-1;i++)
		if (node1->it_set [i]!= node2->it_set[i]) 
		{
			//break;
			return 1;
		}
	return 0;
}
tr_charm Charm_business::blend(tr_charm node1, tr_charm node2)
{
	tr_charm blend = node1;
	int temp=blend.real_item;
	/*for (int i =0; i< node2.real_item;i++)
	{
		
	}*/
	blend.it_set[temp]=node2.it_set[node2.real_item-1];
	blend.real_item= temp+1;// tang so luong item trong itemset
	//xu ly tidset
	int count=0;
	for(int i =0; i< node1.real_tid ; i++)
	{
		for(int j=0; j<node2.real_tid; j++)
			if( node1.id_set[i]==node2.id_set[j])
			{
				blend.id_set[count] = blend.id_set[i];
				count++;
				break;
			}
	}
	blend.real_tid=count;
	blend.real_branch=0;
	return blend;
}
int Charm_business::Charm_property(tr_charm* node1,tr_charm*node2, tr_charm *New_node)
{
	if(New_node->real_tid== node1->real_tid)
	{
		if (New_node->real_tid== node2->real_tid)
		{
			//tinh chat 1
			node2->real_item=0; // loai bo nhanh 2 (tia nhanh);
			//copy tư new_node sang node1
			node1->real_item= New_node->real_item;
			for (int i=0; i< node1->real_item;i++) node1->it_set[i]=New_node->it_set[i];
			if(node1->real_branch>0)
			{
				for(int i=0; i<node1->real_branch; i++)
				{
					*node1->pointer_set[i]= blend(*node1->pointer_set[i],*New_node);
				}
			}
			return 1;
		}
		else
		{
			//tinh chat 2
			node1->real_item= New_node->real_item;
			for (int i=0; i< node1->real_item;i++) node1->it_set[i]=New_node->it_set[i];
			if(node1->real_branch>0)
			{
				for(int i=0; i<node1->real_branch; i++)
				{
					*node1->pointer_set[i]= blend(*node1->pointer_set[i],*New_node);
				}
			}
			return 2;
		}
	}
	else
	{
		if (New_node->real_tid== node2->real_tid)
		{
			// tinh chat 3
			node2->real_item=0; // loai bo nhanh 2 (tia nhanh);
			node1->pointer_set[node1->real_branch]=New_node;
			node1->real_branch++;
			return 3;
		}
		else
		{
			// tinh chat 4
			node1->pointer_set[node1->real_branch]=New_node;
			node1->real_branch++;
			return 4;
		}
	}
}


int Charm_business::print()
{
	this->printn(this->head);
	return 0;
}
int Charm_business::printn(tr_charm node)
{
	
	if(node.real_item!=0)
	{
		FILE *ftem;
		ftem= fopen("ketqua.txt","a+t");
		for(int j=0; j< node.real_item;j++) fprintf(ftem,"%d",node.it_set[j]);
		fprintf(ftem,",");
		fclose(ftem);
	}
	for(int i =0; i<node.real_branch;i++) printn(*node.pointer_set[i]);
	
	return 0;
}
void Charm_business::print(char filename[])
{
	this->C.print(filename);
}