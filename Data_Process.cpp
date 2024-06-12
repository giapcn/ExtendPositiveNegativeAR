#include "StdAfx.h"
#include "Data_Process.h"

Data_Process::Data_Process(void)
{
}

Data_Process::~Data_Process(void)
{
}
db_pattern Data_Process::bi_blind(char filename[])
{
		char tilte[256];
		int item, tid;
		FILE *fPopu;
		fPopu=fopen(filename,"r");
		fscanf(fPopu,"%d",&item);
		fscanf(fPopu,"%d",&tid);
		for (int i=0; i<tid; i++)
		{
			for (int j=0; j<item;j++)
			{
				fscanf(fPopu," %d %1s ",&this->bi_db.db_inst[i][j],tilte);
		
			}
			//fscanf(fPopu,"%s",tilte);
		}
		fclose(fPopu);
		this->bi_db.real_item=item;
		this->bi_db.real_Tid=tid;
		return this->bi_db;
}
db_pattern Data_Process:: gl_blind(char filename[])
{
	attr_str attr[Branch];
	int at_num[Branch]; // chua co thuoc tinh nao
	for (int i =0; i<Branch; i++) 
	{
		attr[i].name = "";
		attr[i].next = NULL;
		at_num[i]=0;
	}
	ifstream fin;
    char buffer[255];
    size_t found;
    string s;
    string str;
   // cout <<"Word to search for: ";
   // cin >> buffer;
	/*Duyet file lan thu nhat de dem so luong thuoc tinh */
	int attr_num=0;// so luong thuoc tinh
	int tran_num =0;// so luong transaction
    fin.open(filename);
    if (fin.good())
    {
		while(!fin.eof())
		{
			getline(fin,s);
			s=s+',';
			tran_num++;
			int main_level=0;
			while(s.length()>0)
			{
				found = s.find_first_of (",");
				str.clear();
				str.append (s,0,found);
				attr_str *temp;
				temp= &attr[main_level];
			/* xu ly mot thuoc tinh*/
				if(temp->name!="")
				{ 
					if(str.compare(temp->name)!=0)
					{
						int status=0;
						while (temp->next!=NULL)
						{
							temp = temp->next;
							if(str.compare(temp->name)==0)
							{
								status =1;
								break;
							}
						}
						if(status==0)
						{ 
							// them thuoc tinh
							at_num[main_level]++;
							attr_num++;
							temp->next = new attr_str;
							temp = temp->next;
							temp->name= str;
							temp->next=NULL;
						}
					}
				}
				else
				{
					// them thuoc tinh
					at_num[main_level]++;
					attr_num++;
					// gan chuoi
					temp->name= str;
					temp->next=NULL;	
				}
				s.erase(0,found+1);
				// thuoc tinh tiep theo
				main_level ++;
			}
		  //  cout << s << endl;
		}
	}
    fin.close();    
	if(attr_num >Item_size)
	{
		cout<< "so luong thuoc tinh qua lon.";
		system("pause");
	}
	//else
	//{
	//	fin.open(filename);
	//	if (fin.good())
	//	{
	//		int tr_count=0;//bien luu tru transaction dang duoc xu ly
	//		while(!fin.eof())
	//		{
	//			getline(fin,s);
	//			s=s+',';
	//			int main_level=0;
	//			while(s.length()>0)
	//			{
	//				found = s.find_first_of (",");
	//				str.clear();
	//				str.append (s,0,found);
	//				attr_str *temp;
	//				temp= &attr[main_level];
	//				/* xu ly mot thuoc tinh*/
	//				int br_count=1;//bien kiem tra xem thuoc tinh la thuoc tinh thu may
	//				if(str.compare(temp->name)!=0)
	//				{
	//					while (temp->next!=NULL)
	//					{
	//						temp = temp->next;
	//						br_count++;
	//						if(str.compare(temp->name)==0)
	//						{
	//							break;
	//						}
	//					}
	//					
	//				}
	//				//nhap du lieu vao mang du lieu
	//				int start=0;
	//				for(int run_1=0;run_1<main_level;run_1++) start=start + at_num[run_1];
	//				for(int run_1=start;run_1<start+at_num[main_level]; run_1++) this->bi_db.db_inst[tr_count][run_1]=0;
	//				this->bi_db.db_inst[tr_count][start+br_count]=1;
	//				s.erase(0,found+1);
	//				// thuoc tinh tiep theo
	//				main_level ++;
	//			}
	//			tr_count++;
	//		}
	//	}
	//	fin.close(); 
	//}
	//bi_db.real_item=attr_num;
	//bi_db.real_Tid = tran_num;
	return this->bi_db;
}
void Data_Process::print_db()
{
	for (int i=0; i<this->bi_db.real_Tid; i++)
	{
		for (int j=0; j<this->bi_db .real_item;j++)
			{
				printf(" %d ,",this->bi_db.db_inst[i][j]);
			}
			printf("\n");
	}
}