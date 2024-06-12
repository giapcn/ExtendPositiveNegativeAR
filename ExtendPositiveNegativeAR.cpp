// ExtendPositiveNegativeAR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <time.h>
#include "time_counter.h"
#include "DA2dims.h"
#include "EPNAR_Process.h"
//#include <vector>
using namespace std;

void run_proces()
{

	cout << "CHUONG TRINH TIM TAP LUAT PHO BIEN AM DUONG MO RONG \n";

/*
	string filename;
	cout << "nhap vao ten file du lieu:";
	cin >> filename;
*/	
	// read data file 
	DA2dims data;
	
	data.bi_dataimport("testing_data\\Train_OriginalVariables_categorial_data_PNR.txt");
	cout << data.To_String();
	
	double ListSup[] = { 0.01 };//{0.05,0.1,0.2,0.25,0.3};
	double ListConf[] = { 0.9 };//{ 0.8,0.85,0.9,0.95,0.99};

	vector<double> minsuplist(ListSup,ListSup + sizeof(ListSup) / sizeof(double) );
	vector<double> minconflist(ListConf,ListConf + sizeof(ListConf) / sizeof(double) );
	
	//tru thuoc tinh cuoi cung là phan lop
	unsigned int Y=7;
	
	ofstream resultfile;
	resultfile.open("results\\Train_OriginalVariables_categorial_data_PNR_statistic.txt");
	for (unsigned int i = 0; i < minsuplist.size(); i++)
	for (unsigned int j = 0; j < minconflist.size(); j++)
	{
		cout << "minsup: " << minsuplist[i] << "; minconf: " << minconflist[j];
		clock_t start = clock();
		time_counter t;
		t.settime();
		int time = t.running_period();
		
		EPNAR_Process *pro = new EPNAR_Process(&data, data.getcolnum(), data.getrownum());
		(*pro).printData();
		double minsup = minsuplist[i];
		double minconf= minconflist[j];

		/*cout << endl;
//		cout << " Nhap vao minsup: "; cin >> minsup;
//		cout << " Nhap vao minconf: "; cin >> minconf;
//		cout << " Thu tu bien target: "; cin >> Y;*/
		bool Ruleform = true;
		bool RedunCon = false;
		(*pro).generateK1rule(minsup, minconf, Y,Ruleform,RedunCon); //y = 4 ~ item thu 5 la item muc tieu

		(*pro).printpolularRules();
		if(!RedunCon);
			(*pro).printvalidRules();
		(*pro).printvalidNotRedundantRules();

		string PRfilename = "results\\Train_OriginalVariables_categorial_data_PNR-popularrules_" + to_string(static_cast<long double>(minsup)) + "_" + to_string(static_cast<long double>(minconf)) + ".txt";
		string VLRfilename = "results\\Train_OriginalVariables_categorial_data_PNR-validrules_" + to_string(static_cast<long double>(minsup)) + "_" + to_string(static_cast<long double>(minconf)) + ".txt";
		string VLRNOTREfilename = "results\\Train_OriginalVariables_categorial_data_PNR-validNotRerules_" + to_string(static_cast<long double>(minsup)) + "_" + to_string(static_cast<long double>(minconf)) + ".txt";

		
		(*pro).fprintpolularRules(PRfilename);
		if(!RedunCon);
			(*pro).fprintvalidRules(VLRfilename);
		(*pro).fprintvalidNotRedundantRules(VLRNOTREfilename);
		// sort and print again
		cout << endl << "Luat valid duoc sap xep lai theo order." << endl;
		(*pro).rulessort();
		(*pro).printvalidRules();
		(*pro).printvalidNotRedundantRules();

		string orderPRfilename = "results\\Train_OriginalVariables_categorial_data_PNR-popularrules_" + to_string(static_cast<long double>(minsup)) + "_" + to_string(static_cast<long double>(minconf)) + ".txt";
		string orderVLRfilename = "results\\Train_OriginalVariables_categorial_data_PNR-validrules_" + to_string(static_cast<long double>(minsup)) + "_" + to_string(static_cast<long double>(minconf)) + ".txt";
		string orderVLRNOTREfilename = "results\\Train_OriginalVariables_categorial_data_PNR-validNotRerules_" + to_string(static_cast<long double>(minsup)) + "_" + to_string(static_cast<long double>(minconf)) + ".txt";

		(*pro).fprintpolularRules(orderPRfilename);
		if (!RedunCon);
			(*pro).fprintvalidRules(orderVLRfilename);
		(*pro).fprintvalidNotRedundantRules(orderVLRNOTREfilename);

//		// print statistic 
		unsigned int NumPoRules = (*pro).getNumberPopuRules();
		unsigned int NumValidRules = (*pro).getNumberVRRules();
		unsigned int NumValidNotReRules = (*pro).getNumberVRNoReRules();
		resultfile << minsup << "," << minconf << ",";
		resultfile << NumPoRules << "," << NumValidRules << "," << NumValidNotReRules <<",";

		clock_t finish = clock();
		resultfile<<(double)(finish - start) / CLOCKS_PER_SEC<< endl;// để lấy được phần nghìn của giây
		//resultfile<<t.running_period()-time<< endl;
		delete (pro);
	}
	resultfile.close();

}

int _tmain(int argc, _TCHAR* argv[])
{
	thread threadobj (run_proces);
	threadobj.join();
	return 0;
}