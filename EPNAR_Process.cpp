#include "StdAfx.h"
#include "EPNAR_Process.h"


EPNAR_Process::EPNAR_Process(DA2dims *Data,unsigned int Items, unsigned int Trans)
{
	data= Data;
	this->items = Items;
	this->trans = Trans;
	posOutput = new ARlist();
	negOutput = new ARlist();

};
EPNAR_Process::~EPNAR_Process(void)
{
}

// in cac luat pho bien da tim duoc ra man hinh 
void EPNAR_Process::printpolularRules()
{
	if(this->posOutput !=NULL)
	{
		ARlist* p = this->posOutput;
		while(p!= NULL)
		{
			cout<< endl<< "Cac luat co luc luong "<<p->length <<endl;
			epnAR* q = p->head; 
			while (q!=NULL)
			{
				// in luat
				if(q->precedent) cout<< "{Z";
				else 
					cout<< "{-Z";
				for(int i=0; i<q->pos.size(); i++) cout<< q->pos[i] <<",";
				for(int i=0; i<q->neg.size(); i++) cout<< "-"<< q->neg[i];
				cout<< "|" << q->frequent<<";"<<q->confident << "}";
				cout << endl;
				q=q->next;
			}
			p= p->next;
		}
	}
	if(this->negOutput !=NULL)
	{
		ARlist* p = this->negOutput;
		while(p!= NULL)
		{
			cout<< "cac luat co luc luong "<<p->length <<endl;
			epnAR* q = p->head; 
			while (q!=NULL)
			{
					// in luat
				if(q->precedent) cout<< "{Z";
				else 
					cout<< "{-Z";
				for(int i=0; i<q->pos.size(); i++) cout<< q->pos[i] <<",";
				for(int i=0; i<q->neg.size(); i++) cout<< "-"<< q->neg[i];
				cout<< "|" << q->frequent<<";"<<q->confident << "}";
				cout << endl;
				q=q->next;
			}
			p= p->next;
		}
	}
}

// in cac luat valide da tim duoc ra man hinh 
void EPNAR_Process::printvalidRules()
{
	cout << endl << "Cac luat valid: " << endl;
	for (unsigned int i = 0; i < validRules.size(); i++)
	{
		{
			epnAR* q = &validRules[i];
			if (q != NULL)
			{
				// in luat
				if (q->precedent) cout << "{Z";
				else
					cout << "{-Z";
				for (int i = 0; i < q->pos.size(); i++) cout << q->pos[i] << ",";
				for (int i = 0; i < q->neg.size(); i++) cout << "-" << q->neg[i];
				cout << "|" << q->frequent << ";" << q->confident << "}";
				cout << endl;
				q = q->next;
			}
		}
	}
}
void EPNAR_Process::printvalidNotRedundantRules()
{
	cout << endl << "Cac luat valid and not redundant: " << endl;
	for (unsigned int i = 0; i < validNotRedundantRules.size(); i++)
	{
		{
			epnAR* q = &validNotRedundantRules[i];
			if (q != NULL)
			{
				// in luat
				if (q->precedent) cout << "{Z";
				else
					cout << "{-Z";
				for (int i = 0; i < q->pos.size(); i++) cout << q->pos[i] << ",";
				for (int i = 0; i < q->neg.size(); i++) cout << "-" << q->neg[i];
				cout << "|" << q->frequent << ";" << q->confident << "}";
				cout << endl;
				q = q->next;
			}
		}
	}
}

// in du lieu (dung de kiem tra du lieu vao)
void EPNAR_Process::printData()
{
	if(this->data !=NULL)
	{
		cout<<endl<< "Du lieu vao: " <<endl;
		cout << (*data).To_String();
	}
	else
		cout<< endl<< "Du lieu vao NULL. " << endl;
}

void EPNAR_Process:: generateK1rule(double Minsup, double Minconf, unsigned int Y, bool RuleForm, bool RedunCon)
{
	// tạo ra 2 danh sach luat voi luc luong k=1;
	// RuleForm = 0 -> luat 1 va RuleForm = 1 -> luat 2; RedunCon = 0 -> tim ca valid va nonredundant, RedunCon = 1 -> chi tim NonRedundant rules
	this->NumberPopuRules = 0; 
	///////////////////////
	this->minsup = Minsup;
	this->minconf = Minconf; 
	this->y= Y;
	// tạo ra 2 danh sach luat voi luc luong k=1
	unsigned int * pos_True_fres = new unsigned int[items];
	unsigned int *pos_False_fres = new unsigned int[items];
	unsigned int *neg_True_fres= new unsigned int[items];
	for(unsigned int i=0; i<this->items; i++)
	{
		pos_True_fres[i] =0; pos_False_fres[i]=0; neg_True_fres[i] =0;
	}
	for(unsigned int i=0; i< this->trans; i++)
	{
		if((*data).GetSpecEle(y,i) ==1)
		{
			// positive target
			for(unsigned int j=0; j< this->items; j++)
			{
				if((*data).GetSpecEle(j,i)==1)
					pos_True_fres[j] ++;
				else
					pos_False_fres[j]++;
			}
		}
		else
		{
			// negative target
			for(unsigned int j=0; j< this->items; j++)
			{
				if((*data).GetSpecEle(j,i)==1)
					neg_True_fres[j] ++;
			}
		}
	}
	/////////////////////
	// xet cac tap I1, I2, I3 va khoi tao cac luat co luc luong k=1;
	this->posOutput = new ARlist;
	this->negOutput = new ARlist;
	(*posOutput).length = 1; (*posOutput).head = NULL; (*posOutput).next = NULL;
	(*negOutput).length = 1; (*negOutput).head = NULL; (*negOutput).next = NULL;
	epnAR * posCurr = (*posOutput).head;
	epnAR * negCurr = (*negOutput).head;
	if(RuleForm)
	{
		for( unsigned int i=0; i<this->items; i++)
		{
			if(i!=this->y)
			{
				double supp_ruleZX = (double) (pos_True_fres[i])/(double) trans;
				double conf_ruleZX = (double) (pos_True_fres[i])/(double) (pos_True_fres[i]+neg_True_fres[i]);
				if(this->minsup <= supp_ruleZX) // & this->minconf<= conf_ruleZX )
				{
					NumberPopuRules++;
					///////////////////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = true;// positive target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = pos_True_fres[i];
					newrule->confident=(pos_True_fres[i]+neg_True_fres[i]);
					newrule->pos.push_back(i);
					if(posCurr==NULL)
					{
						(*posOutput).head= newrule; posCurr= newrule;
					}
					else
					{
						posCurr->next = newrule; posCurr= newrule;
					}
				
					// xet luat valid
					if (this->minconf <= conf_ruleZX)
					{
						// valid rule
						validPosRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantPosRules.push_back(*newrule);
					}
				}
				double supp_ruleZ_X = (double)(pos_False_fres[i]) /(double)trans;
				double conf_ruleZ_X = (double)(pos_False_fres[i])/(double)(trans-(pos_True_fres[i]+neg_True_fres[i]));
				double supp_X=((double)(pos_True_fres[i])+(double)(neg_True_fres[i]) ) /(double)trans;
				if((this->minsup <= supp_ruleZ_X) & (this->minsup <= supp_X )) // dang luat 2
				{
					NumberPopuRules++; 
					////////////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = true;// positive target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = (pos_False_fres[i]);
					newrule->confident= trans- (pos_True_fres[i]+neg_True_fres[i]);
					newrule->neg.push_back(i);
					if(posCurr==NULL)
					{
						(*posOutput).head= newrule; posCurr= newrule;
					}
					else
					{
						posCurr->next = newrule; posCurr= newrule;
					}
					// xet luat valid
					if (this->minconf <= conf_ruleZ_X)
					{
						// valid rule
						validPosRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantPosRules.push_back(*newrule);
					}
				}
				double supp_rule_ZX = (double)(neg_True_fres[i])/(double) trans;
				double conf_rule_ZX = (double)(neg_True_fres[i])/(double)(pos_True_fres[i]+neg_True_fres[i]);
				if(this->minsup <= supp_rule_ZX)// & this->minconf <= conf_rule_ZX)
				{
					NumberPopuRules++;
					///////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = false;// negative target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = neg_True_fres[i];
					newrule->confident=(pos_True_fres[i]+neg_True_fres[i]);
					newrule->pos.push_back(i);
					if(negCurr==NULL)
					{
						(*negOutput).head= newrule; negCurr= newrule;
					}
					else
					{
						negCurr->next = newrule; negCurr= newrule;
					}
					// xet luat valid
					if (this->minconf <= conf_rule_ZX)
					{
						// valid rule
						validNegRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantNegRules.push_back(*newrule);
					}
				}
				double supp_rule_Z_X = (double)(trans-(pos_True_fres[i]+neg_True_fres[i]+pos_False_fres[i]))/(double) trans;
				double conf_rule_Z_X = (double)(trans-(pos_True_fres[i]+neg_True_fres[i]+pos_False_fres[i]))/(double)(trans-(pos_True_fres[i]+neg_True_fres[i]));
				supp_X=((double)(pos_True_fres[i])+(double)(neg_True_fres[i]) ) /(double)trans;
				if ((this->minsup <= supp_rule_Z_X) & (this->minsup <= supp_X)) // dang luat 2
				{
					NumberPopuRules++;
					//////////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = false;// negative target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = trans-(pos_True_fres[i]+neg_True_fres[i]+pos_False_fres[i]);
					newrule->confident= trans-(pos_True_fres[i]+neg_True_fres[i]);
					newrule->neg.push_back(i);
					if(negCurr==NULL)
					{
						(*negOutput).head= newrule; negCurr= newrule;
					}
					else
					{
						negCurr->next = newrule; negCurr= newrule;
					}
					// xet luat valid
					if (this->minconf <= conf_rule_Z_X)
					{
						// valid rule
						validNegRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantNegRules.push_back(*newrule);
					}
				}
			}
		}
	}
	else
	{
		for( unsigned int i=0; i<this->items; i++)
		{
			if(i!=this->y)
			{
				double supp_ruleZX = (double) (pos_True_fres[i])/(double) trans;
				double conf_ruleZX = (double) (pos_True_fres[i])/(double) (pos_True_fres[i]+neg_True_fres[i]);
				if(this->minsup <= supp_ruleZX) // & this->minconf<= conf_ruleZX )
				{
					NumberPopuRules++;
					///////////////////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = true;// positive target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = pos_True_fres[i];
					newrule->confident=(pos_True_fres[i]+neg_True_fres[i]);
					newrule->pos.push_back(i);
					if(posCurr==NULL)
					{
						(*posOutput).head= newrule; posCurr= newrule;
					}
					else
					{
						posCurr->next = newrule; posCurr= newrule;
					}
				
					// xet luat valid
					if (this->minconf <= conf_ruleZX)
					{
						// valid rule
						validPosRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantPosRules.push_back(*newrule);
					}
				}
				double supp_ruleZ_X = (double)(pos_False_fres[i]) /(double)trans;
				double conf_ruleZ_X = (double)(pos_False_fres[i])/(double)(trans-(pos_True_fres[i]+neg_True_fres[i]));
				if(this->minsup <= supp_ruleZ_X)
				{
					NumberPopuRules++; 
					////////////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = true;// positive target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = (pos_False_fres[i]);
					newrule->confident= trans- (pos_True_fres[i]+neg_True_fres[i]);
					newrule->neg.push_back(i);
					if(posCurr==NULL)
					{
						(*posOutput).head= newrule; posCurr= newrule;
					}
					else
					{
						posCurr->next = newrule; posCurr= newrule;
					}
					// xet luat valid
					if (this->minconf <= conf_ruleZ_X)
					{
						// valid rule
						validPosRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantPosRules.push_back(*newrule);
					}
				}
				double supp_rule_ZX = (double)(neg_True_fres[i])/(double) trans;
				double conf_rule_ZX = (double)(neg_True_fres[i])/(double)(pos_True_fres[i]+neg_True_fres[i]);
				if(this->minsup <= supp_rule_ZX)
				{
					NumberPopuRules++;
					///////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = false;// negative target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = neg_True_fres[i];
					newrule->confident=(pos_True_fres[i]+neg_True_fres[i]);
					newrule->pos.push_back(i);
					if(negCurr==NULL)
					{
						(*negOutput).head= newrule; negCurr= newrule;
					}
					else
					{
						negCurr->next = newrule; negCurr= newrule;
					}
					// xet luat valid
					if (this->minconf <= conf_rule_ZX)
					{
						// valid rule
						validNegRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantNegRules.push_back(*newrule);
					}
				}
				double supp_rule_Z_X = (double)(trans-(pos_True_fres[i]+neg_True_fres[i]+pos_False_fres[i]))/(double) trans;
				double conf_rule_Z_X = (double)(trans-(pos_True_fres[i]+neg_True_fres[i]+pos_False_fres[i]))/(double)(trans-(pos_True_fres[i]+neg_True_fres[i]));
				if(this->minsup <= supp_rule_Z_X) // & this->minconf <= conf_rule_Z_X)
				{
					NumberPopuRules++;
					//////////////////
					epnAR * newrule = new epnAR;
					newrule->precedent = false;// negative target
					newrule->far = NULL; newrule->mot = NULL;
					newrule->pre = posCurr; newrule->next = NULL;
					newrule->frequent = trans-(pos_True_fres[i]+neg_True_fres[i]+pos_False_fres[i]);
					newrule->confident= trans-(pos_True_fres[i]+neg_True_fres[i]);
					newrule->neg.push_back(i);
					if(negCurr==NULL)
					{
						(*negOutput).head= newrule; negCurr= newrule;
					}
					else
					{
						negCurr->next = newrule; negCurr= newrule;
					}
					// xet luat valid
					if (this->minconf <= conf_rule_Z_X)
					{
						// valid rule
						validNegRules.push_back(*newrule); 
						// avoid redundant
						validNotRedundantNegRules.push_back(*newrule);
					}
				}
			}
		}
	}
	// tu dong search voi cac luat ket hop khac 
	search(this->posOutput,true,RuleForm, RedunCon);
	search(this->negOutput, false, RuleForm, RedunCon);
	//// ghep cac luat am va luat duong vao mot mang duy nhat
	getvalidrules();
	getvalidnotredundantrules();
}

void EPNAR_Process::search(ARlist* LE, bool PosRule, bool RuleForm, bool RedunCon)
{
	// tim kiem các luat duoc sinh moi tu mot danh sach cac luat voi luc luong k=1
	/* sử dụng kỹ thuật giải đệ quy cho phàm search */
	if(!RedunCon) // tim kiem binh thuong
	{
		ARlist * p = LE;
		while (p!= NULL)
		{
			// thong bao man hinh, dam bao nguoi dung biet khi dang xu ly du lieu lon
			cout<< "searching for rules have length "<< p->length << endl;
			ARlist *K1rules=NULL; // luat moi co luc luong k+1
			epnAR * search = p->head;
			int count_forcheck = 0;
			while(search!=NULL)
			{
				epnAR* next = search->next;
				while (next != NULL)
				{
					//count_forcheck++;
					//cout << "\n tim kiem thu " << count_forcheck << endl;
					// ghep 2 luat tro boi search va next;
					// chỉ ghép 2 luật có sự khác biệt bởi 1 ký tự
					/*	co 3 kha nang
						1. Ax-B hop voi Ay-B thành Axy-B
						2. Ax-B hop voi A-By thành Ax-By
						3. A-Bx hop voi A-By thành A-Bxy
						*/
					unsigned int position; int posnegflag;
					bool combFlag = rulescompare(search, next, &position, &posnegflag);
					if (combFlag)
					{
						// ghep 2 luat 
						// ghep mot ky tu luat duong cua next vao search.
						vector<unsigned int> newPos = search->pos; 
						vector<unsigned int> newNeg = search->neg;
						if (posnegflag == 1)
						{
							// truong hop 1
							unsigned int item = next->pos[position];
							newPos.push_back(item);
						}
						else
						{
							// truong hop 2,3
							unsigned int item = next->neg[position];
							newNeg.push_back(item);
						}
			
						unsigned int supp, conf;
						if (search->precedent)
							this->data->get_support_confident_negative_positive_rule(newPos, newNeg,this->y,1,&supp,&conf);
						else
							this->data->get_support_confident_negative_positive_rule(newPos, newNeg, this->y, 0, &supp, &conf);		
						bool flagNegSupp = true;
						if (RuleForm)
						{
							unsigned int NegSupp;
							if (newNeg.size() > 0)
							{
								if (newNeg[0] == 2)
									double s = 2;
								NegSupp = this->data->get_support_positive_frequence(newNeg);
								double negsupp = (double)(NegSupp) / (double)trans;
								if (negsupp < minsup) 
									flagNegSupp = false; // chi kiem tra voi luat dang 2
							}
						}
						/// xet do pho bien va do tin cay
						double supp_rule = (double)(supp) / (double)trans;
						double conf_rule = (double)(supp) / (double)(conf);
						if (this->minsup <= supp_rule && flagNegSupp)
						{
							// add to equivalent class 
							epnAR * newrule = new epnAR;
							newrule->precedent = search->precedent;
							newrule->pos = newPos;
							newrule->neg = newNeg;
							newrule->far = search; newrule->mot = next;
							newrule->pre = NULL; newrule->next = NULL;
							newrule->frequent = supp;
							newrule->confident = conf;
							ARlist * q = p->next;
							if (q == NULL)
							{
								q = new ARlist;
								q->length = p->length + 1;
								q->head = NULL;
								q->next = NULL;
								p->next = q;
							}
							epnAR * tail = q->head;
							if (tail == NULL) q->head = newrule;
							else
							{
								while (tail->next != NULL)
									tail = tail->next;
								newrule->pre = tail;
								tail->next = newrule;
							}
							this->NumberPopuRules++;
							// valid rule
							if(this->minconf <= conf_rule)
							{
								count_forcheck++;
								//cout << "\n tim kiem thu " << count_forcheck << endl;
								cout << "confident rule: " << conf_rule << " number: " << count_forcheck << endl;
								if (PosRule)
								{
									// valid rule
									if (!samevalidrules(newrule))
									{

										validPosRules.push_back(*newrule);
										// avoid redundant
										bool redundentflag = false;
										for (unsigned int i = 0; i < validNotRedundantPosRules.size(); i++)
										{
											if (includerules(newrule, &validNotRedundantPosRules[i]))
											{
												//// rat quan trong: dieu kien luat du thua ko chi la luat r.entecedent thuoc s.entecedent ma conf(r)<=conf(s)
												//if (rule_conf <= (double)validNotRedundantRules[i].frequent / (double)validNotRedundantRules[i].confident)
												//{
												//	redundentflag = true;
												//}
												redundentflag = true;
												break;
											}
										}
										if (!redundentflag)
										{
											if (!samevalidnotredundantrules(newrule))
												validNotRedundantPosRules.push_back(*newrule);
										}
									}
								}
								else
								{
									// valid rule
									if (!samevalidrules(newrule))
									{
										validNegRules.push_back(*newrule);
										// avoid redundant
										bool redundentflag = false;
										for (unsigned int i = 0; i < validNotRedundantNegRules.size(); i++)
										{
											if (includerules(newrule, &validNotRedundantNegRules[i]))
											{
												//// rat quan trong: dieu kien luat du thua ko chi la luat r.entecedent thuoc s.entecedent ma conf(r)<=conf(s)
												//if (rule_conf <= (double)validNotRedundantRules[i].frequent / (double)validNotRedundantRules[i].confident)
												//{
												//	redundentflag = true;
												//}
												redundentflag = true;
												break;
											}
										}
										if (!redundentflag)
											if (!samevalidnotredundantrules(newrule))
												validNotRedundantNegRules.push_back(*newrule);
									}
								}
							}
						}
					}
					next = next ->next; 
				}
				search = search ->next;// set ket hop tiep theo
			}
			pruncesamerules(p->next);
			p = p->next;
		}
	}
	else // check dieu kien redundant truoc khi tinh do ho tro
	{
		ARlist * p = LE;
		while (p!= NULL)
		{
			// thong bao man hinh, dam bao nguoi dung biet khi dang xu ly du lieu lon
			cout<< "searching for rules have length "<< p->length << endl;
			ARlist *K1rules=NULL; // luat moi co luc luong k+1
			epnAR * search = p->head;
			while(search!=NULL)
			{
				epnAR* next = search->next;
				while (next != NULL)
				{
					// ghep 2 luat tro boi search va next;
					// chỉ ghép 2 luật có sự khác biệt bởi 1 ký tự
					/*	co 3 kha nang
						1. Ax-B hop voi Ay-B thành Axy-B
						2. Ax-B hop voi A-By thành Ax-By
						3. A-Bx hop voi A-By thành A-Bxy
						*/
					unsigned int position; int posnegflag;
					bool combFlag = rulescompare(search, next, &position, &posnegflag);
					if (combFlag)
					{
						// ghep 2 luat 
						// ghep mot ky tu luat duong cua next vao search.
						vector<unsigned int> newPos = search->pos; 
						vector<unsigned int> newNeg = search->neg;
						if (posnegflag == 1)
						{
							// truong hop 1
							unsigned int item = next->pos[position];
							newPos.push_back(item);
						}
						else
						{
							// truong hop 2,3
							unsigned int item = next->neg[position];
							newNeg.push_back(item);
						}
						epnAR * newrule = new epnAR;
						newrule->precedent = search->precedent;
						newrule->pos = newPos;
						newrule->neg = newNeg;
						// check redundant
						bool redundentflag = false;
						if(PosRule)
						{					
							for (unsigned int i = 0; i < validNotRedundantPosRules.size(); i++)
							{
								if (includerules(newrule, &validNotRedundantPosRules[i]))
								{
									redundentflag = true;
									break;
								}
							}
						}
						else
						{
							for (unsigned int i = 0; i < validNotRedundantNegRules.size(); i++)
							{
								if (includerules(newrule, &validNotRedundantNegRules[i]))
								{
									redundentflag = true;
									break;
								}
							}
						}
						if (redundentflag)
							delete newrule;
						else
						{
							unsigned int supp, conf;
							if (search->precedent)
								this->data->get_support_confident_negative_positive_rule(newPos, newNeg,this->y,1,&supp,&conf);
							else
								this->data->get_support_confident_negative_positive_rule(newPos, newNeg, this->y, 0, &supp, &conf);
							//rule form 2
							bool flagNegSupp = true;
							if (RuleForm)
							{
								unsigned int NegSupp;
								if (newNeg.size() > 0)
								{
									NegSupp = this->data->get_support_positive_frequence(newNeg);
									double negsupp = (double)(NegSupp) / (double)trans;
									if (negsupp < minsup) flagNegSupp = false; // chi kiem tra voi luat dang 2
								}
							}
							/// xet do pho bien va do tin cay
							double supp_rule = (double)(supp) / (double)trans;
							double conf_rule = (double)(supp)/(double)(conf);
							if (this->minsup <= supp_rule && flagNegSupp)
							{
								// them thong tin vao luat
								newrule->far = search; newrule->mot = next;
								newrule->pre = NULL; newrule->next = NULL;
								newrule->frequent = supp;
								newrule->confident = conf;
								ARlist * q = p->next;
								if (q == NULL)
								{
									q = new ARlist;
									q->length = p->length + 1;
									q->head = NULL;
									q->next = NULL;
									p->next = q;
								}
								epnAR * tail = q->head;
								if (tail == NULL) q->head = newrule;
								else
								{
									while (tail->next != NULL)
										tail = tail->next;
									newrule->pre = tail;
									tail->next = newrule;
								}
								// valid rule
								if(this->minconf <= conf_rule)
								{
									if(PosRule)
									{
										this->validNotRedundantPosRules.push_back(*newrule);
									}
									else
									{
										this->validNotRedundantNegRules.push_back(*newrule);
									}
								}
							}
						}
					}
					next = next ->next; 
				}
				search = search ->next;// set ket hop tiep theo
			}
			pruncesamerules(p->next);
			p = p->next;
		}
	}
	
}

bool EPNAR_Process::rulescompare(epnAR* r, epnAR* s, unsigned int* position, int* posnegflag)
{
	// kiem tra 2 luat co phu hop de ghep cap
	int diffcount = 0; // so ky tu khac
	unsigned int s_index = -1; // vi tri ky tu khac trong r
	int posneg = -1; // =1 ky tu khac o positive, =0 ky tu khac o negative
	for (unsigned int i = 0; i < s->pos.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < r->pos.size(); j++)
		{

			if (s->pos[i] == r->pos[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			s_index = i; posneg = 1;  diffcount++;
		}
	}
	for (unsigned int i = 0; i < s->neg.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < r->neg.size(); j++)
		{

			if (s->neg[i] == r->neg[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			s_index = i; posneg = 0; diffcount++;
		}
	}
	if (diffcount == 1) 
	{
		*position = s_index;
		*posnegflag = posneg;
		return true;
	}
	else return false;
}
bool EPNAR_Process::samerules(epnAR* r, epnAR* s)
{
	// kiem tra 2 luat co phu hop de ghep cap
	int diffcount = 0; // so ky tu khac
	unsigned int s_index = -1; // vi tri ky tu khac trong r
	int posneg = -1; // =1 ky tu khac o positive, =0 ky tu khac o negative
	for (unsigned int i = 0; i < s->pos.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < r->pos.size(); j++)
		{

			if (s->pos[i] == r->pos[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			s_index = i; posneg = 1;  diffcount++;
		}
	}
	for (unsigned int i = 0; i < s->neg.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < r->neg.size(); j++)
		{

			if (s->neg[i] == r->neg[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			s_index = i; posneg = 0; diffcount++;
		}
	}
	if (diffcount == 0)
	{
		return true;
	}
	else return false;
}
void EPNAR_Process::pruncesamerules(ARlist *LE)
{
	// prunce same rules
	if (LE != NULL)
	{
		epnAR * p = LE->head;
		while (p != NULL)
		{
			epnAR * q = p->next;
			while (q != NULL)
			{
				if (samerules(q, p))
				{
					epnAR* temp = q->pre; 
					temp->next = q->next;
					if (q->next!= NULL)
						q->next->pre = temp;
					delete q;
					this->NumberPopuRules--;
					q = temp->next;
				}
				else 
					q = q->next;
			}
			p = p->next;
		}
	}
}
bool EPNAR_Process::samevalidrules(epnAR *Rule)
{
	// check if the same rule is exist
	for (int i = 0; i < this->validPosRules.size(); i++)
		if (samerules(&validPosRules[i], Rule))
		{
			return true;
		}
	for (int i = 0; i < this->validNegRules.size(); i++)
		if (samerules(&validNegRules[i], Rule))
		{
			return true;
		}
	return false;
}
bool EPNAR_Process::samevalidnotredundantrules(epnAR *Rule)
{
	// check if the same rule is exist
	for (int i = 0; i < this->validNotRedundantPosRules.size(); i++)
		if (samerules(&validNotRedundantPosRules[i], Rule))
		{
			return true;
		}
	for (int i = 0; i < this->validNotRedundantNegRules.size(); i++)
		if (samerules(&validNotRedundantNegRules[i], Rule))
		{
			return true;
		}
	return false;
}
//void EPNAR_Process::getvalidrules(ARlist* LE)
//{
//	//validRules.clear();
//	// loc cac luat valid va loai bo luat du thua
//	ARlist * p = LE;
//	while (p != NULL)
//	{
//		// thong bao man hinh, dam bao nguoi dung biet khi dang xu ly du lieu lon
//		cout << "searching for rules have length " << p->length << endl;
//		ARlist *K1rules = NULL; // luat moi co luc luong k+1
//		epnAR * search = p->head;
//		while (search != NULL)
//		{
//			NumberPopuRules++; 
//			/////
//			unsigned int supp = search->frequent;
//			unsigned int conf = search->confident;
//			double rule_conf = (double)(supp)/(double)conf;
//			if (this->minconf <= rule_conf)
//			{
//				// valid rule
//				validRules.push_back(*search); 
//				// avoid redundant
//				bool redundentflag = false;
//				for (unsigned int i = 0; i < validNotRedundantRules.size(); i++)
//				{
//					if (includerules(search, &validNotRedundantRules[i]))
//					{
//						// rat quan trong: dieu kien luat du thua ko chi la luat r.entecedent thuoc s.entecedent ma conf(r)<=conf(s)
//						if (rule_conf <= (double)validNotRedundantRules[i].frequent / (double)validNotRedundantRules[i].confident)
//						{
//							redundentflag = true;
//						}
//						//redundentflag = true;
//					}
//				}
//				if (!redundentflag)
//					validNotRedundantRules.push_back(*search);
//			}
//			search = search->next;
//		}
//		p = p->next;
//	}
//}

void EPNAR_Process::getvalidrules()
{
	// gep positive and negative valid rules
	for (unsigned int i = 0; i < validPosRules.size(); i++)
	{
		this->validRules.push_back(validPosRules[i]);
	}
	for (unsigned int i = 0; i < validNegRules.size(); i++)
	{
		this->validRules.push_back(validNegRules[i]);
	}			
}

void EPNAR_Process::getvalidnotredundantrules()
{
	// gep positive and negative valid rules
	for (unsigned int i = 0; i < validNotRedundantPosRules.size(); i++)
	{
		this->validNotRedundantRules.push_back(validNotRedundantPosRules[i]);
	}
	for (unsigned int i = 0; i < validNotRedundantNegRules.size(); i++)
	{
		this->validNotRedundantRules.push_back(validNotRedundantNegRules[i]);
	}			
}

bool EPNAR_Process::includerules(epnAR* r, epnAR* s)
{
	// kiem tra entecedent cuar r co bao chua s
	int diffcount = 0; // so ky tu khac
	unsigned int s_index = -1; // vi tri ky tu khac trong r
	int posneg = -1; // =1 ky tu khac o positive, =0 ky tu khac o negative
	for (unsigned int i = 0; i < s->pos.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < r->pos.size(); j++)
		{

			if (s->pos[i] == r->pos[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			s_index = i; posneg = 1;  diffcount++;
		}
	}
	for (unsigned int i = 0; i < s->neg.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < r->neg.size(); j++)
		{

			if (s->neg[i] == r->neg[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			s_index = i; posneg = 0; diffcount++;
		}
	}
	if (diffcount == 0)
	{
		return true;
	}
	else return false;
}

//bool EPNAR_Process::includeEntecedent(epnAR* r, epnAR* s)
//{
//	// kiem tra entecedent cuar r co bao chua s
//	int diffcount = 0; // so ky tu khac
//	unsigned int s_index = -1; // vi tri ky tu khac trong r
//	int posneg = -1; // =1 ky tu khac o positive, =0 ky tu khac o negative
//	for (unsigned int i = 0; i < s->pos.size(); i++)
//	{
//		bool diff = true;
//		for (unsigned int j = 0; j < r->pos.size(); j++)
//		{
//
//			if (s->pos[i] == r->pos[j])
//			{
//				diff = false; break;
//			}
//		}
//		if (diff)
//		{
//			s_index = i; posneg = 1;  diffcount++;
//		}
//	}
//	for (unsigned int i = 0; i < s->neg.size(); i++)
//	{
//		bool diff = true;
//		for (unsigned int j = 0; j < r->neg.size(); j++)
//		{
//
//			if (s->neg[i] == r->neg[j])
//			{
//				diff = false; break;
//			}
//		}
//		if (diff)
//		{
//			s_index = i; posneg = 0; diffcount++;
//		}
//	}
//	if (diffcount == 0)
//	{
//		return true;
//	}
//	else return false;
//}
void EPNAR_Process::rulessort()
{
	this->validRules= sortrules(validRules);
	this->validNotRedundantRules = sortrules(validNotRedundantRules);
}

vector<epnAR> EPNAR_Process::sortrules(vector<epnAR> Rules)
{
	/* sap xep cac luat trong vetor<> Rules theo dinh nghia ve thu tu*/
	// su dung giai thuat noi bot
	for (unsigned int i = 0; i < Rules.size(); i++)
	{
		for (unsigned int j = Rules.size() - 1; j>i; j--)
		{
			unsigned int r_supp = Rules[j].frequent;
			unsigned int r_conf = Rules[j].confident;
			unsigned int s_supp = Rules[j-1].frequent;
			unsigned int s_conf = Rules[j-1].confident;

			if ((double)r_supp / double(r_conf) > (double)s_supp / double(s_conf))
			{
				// doi cho
				epnAR temp = Rules[j - 1];
				Rules[j - 1] = Rules[j];
				Rules[j] = temp;
			}
			else
			{
				if (((double)r_supp / double(r_conf) == (double)s_supp / double(s_conf)) & r_supp > s_supp)
				{
					//doi cho
					epnAR temp = Rules[j - 1];
					Rules[j - 1] = Rules[j];
					Rules[j] = temp;
				}
			}
		}
	}
	return Rules;
}

bool EPNAR_Process::selectedrule(vector<unsigned int> InputTrans, epnAR* selectedrule, int* Y_value)
{
	for (unsigned int i = 0; i < this->validRules.size(); i++)
	{
		if (ruletransmatching(&validRules[i], InputTrans))
		{
			selectedrule = &validRules[i];
			if (validRules[i].precedent)
				*Y_value = 1;
			else
				*Y_value = 0;
			return true;
		}
	}
	return false;
}
bool EPNAR_Process::ruletransmatching(epnAR* r, vector<unsigned int> InputTrans)
{
	// kiem tra entecedent cuar r co bao chua InputTrans
	int diffcount = 0; // so ky tu khac
	
	for (unsigned int i = 0; i < r->pos.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < InputTrans.size(); j++)
		{

			if (r->pos[i] == InputTrans[j])
			{
				diff = false; break;
			}
		}
		if (diff)
		{
			return false;
		}
	}
	for (unsigned int i = 0; i < r->neg.size(); i++)
	{
		bool diff = true;
		for (unsigned int j = 0; j < InputTrans.size(); j++)
		{

			if (r->neg[i] == r->neg[j])
			{
				diff = false; break;
			}
		}
		if (!diff)
		{
			return false;
		}
	}
	return true;
}

void EPNAR_Process::fprintpolularRules(string filename)
{
	ofstream file;
	file.open(filename);
	if (file)
	{
		if (this->posOutput != NULL)
		{
			ARlist* p = this->posOutput;
			while (p != NULL)
			{
				file << endl << "Cac luat co luc luong " << p->length << endl;
				epnAR* q = p->head;
				while (q != NULL)
				{
					// in luat
					if (q->precedent) file << "Z";
					else
						file << "-Z";
					for (int i = 0; i<q->pos.size(); i++) file << q->pos[i] << ";";
					for (int i = 0; i<q->neg.size(); i++) file << "-" << q->neg[i] <<";";
					file << ";|;" << q->frequent << ";" << q->confident;
					file << endl;
					q = q->next;
				}
				p = p->next;
			}
		}
		if (this->negOutput != NULL)
		{
			ARlist* p = this->negOutput;
			while (p != NULL)
			{
				file << "cac luat co luc luong " << p->length << endl;
				epnAR* q = p->head;
				while (q != NULL)
				{
					// in luat
					if (q->precedent) file << "Z";
					else
						file << "-Z";
					for (int i = 0; i<q->pos.size(); i++) file << q->pos[i] << ";";
					for (int i = 0; i<q->neg.size(); i++) file << "-" << q->neg[i]<<";";
					file << ";|;" << q->frequent << ";" << q->confident;
					file << endl;
					q = q->next;
				}
				p = p->next;
			}
		}
	}
	file.close();
}

void EPNAR_Process::fprintvalidRules(string filename)
{
	ofstream file;
	file.open(filename);
	{
		file << "Cac luat valid: " << endl;
		for (unsigned int i = 0; i < validRules.size(); i++)
		{
			{
				epnAR* q = &validRules[i];
				if (q != NULL)
				{
					// in luat
					if (q->precedent) file << "Z";
					else
						file << "-Z";
					for (int i = 0; i < q->pos.size(); i++) file << q->pos[i] << ";";
					for (int i = 0; i < q->neg.size(); i++) file << "-" << q->neg[i]<<";";
					file << ";|;" << q->frequent << ";" << q->confident ;
					file << endl;
					q = q->next;
				}
			}
		}
	}
	file.close();
}

void EPNAR_Process::fprintvalidNotRedundantRules(string filename)
{
	ofstream file;
	file.open(filename);
	{
		file << "Cac luat valid and not redundant: " << endl;
		for (unsigned int i = 0; i < validNotRedundantRules.size(); i++)
		{
			{
				epnAR* q = &validNotRedundantRules[i];
				if (q != NULL)
				{
					// in luat
					if (q->precedent) file << "Z";
					else
						file << "-Z";
					for (int i = 0; i < q->pos.size(); i++) file << q->pos[i] << ";";
					for (int i = 0; i < q->neg.size(); i++) file << "-" << q->neg[i]<<";";
					file << ";|;" << q->frequent << ";" << q->confident;
					file << endl;
					q = q->next;
				}
			}
		}
	}
	file.close();
}

unsigned int EPNAR_Process::getNumberPopuRules()
{
	return this->NumberPopuRules;
	
}
unsigned int EPNAR_Process::getNumberVRRules()
{
	return validRules.size();
}
unsigned int EPNAR_Process::getNumberVRNoReRules()
{
	return validNotRedundantRules.size();
}