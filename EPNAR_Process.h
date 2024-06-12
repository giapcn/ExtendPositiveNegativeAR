#pragma once
#include <iostream>
#include <fstream>
#include "DA2dims.h"

using namespace std;
class EPNAR_Process
{
	ARlist * posOutput; // luu tru cac luat voi tap dich duong
	ARlist * negOutput; // luu tru cac luat voi tap dich am
	vector<epnAR> validRules; // luu tru cac luat valid and in order
	vector<epnAR> validNotRedundantRules; // luu tru cac luat valid và không dư thừa and in order

	vector<epnAR> validPosRules; // luu tru cac luat  valid co positive target
	vector<epnAR> validNegRules; // luu tru cac luat valid  co negative target

	vector<epnAR> validNotRedundantPosRules; // luu tru cac luat  valid co positive target
	vector<epnAR> validNotRedundantNegRules; // luu tru cac luat valid  co negative target
	///////////////////
	DA2dims *data;
	unsigned int items;
	unsigned int trans;
	double minsup;
	double minconf;
	unsigned int y; // index of target output variable
	unsigned int NumberPopuRules;
public: 
	// tạo ra 2 danh sach luat voi luc luong k=1;
	// RuleForm = 0 -> luat 1 va RuleForm = 1 -> luat 2; RedunCon = 0 -> tim ca valid va nonredundant, RedunCon = 1 -> chi tim NonRedundant rules
	void generateK1rule(double Minsup, double Minconf, unsigned int Y, bool RuleForm, bool RedunCon); 
	void rulessort();
	void printpolularRules();
	void printvalidRules();
	void printvalidNotRedundantRules();
	void printData();
	bool selectedrule(vector<unsigned int> InputTrans,epnAR* selectedrule, int* Y_value);
	void fprintpolularRules(string filename);
	void fprintvalidRules(string filename);
	void fprintvalidNotRedundantRules(string filename);

	unsigned int getNumberPopuRules();
	unsigned int getNumberVRRules();
	unsigned int getNumberVRNoReRules();
private:
	void search(ARlist* LE, bool PosRule, bool RuleForm, bool RedunCon); // tim kiem các luat duoc sinh moi tu mot danh sach cac luat voi luc luong k=1
	void calculateConf(epnAR* r, epnAR* s, int* frequent, int* confident);// tinh do ho tro va do tin cay cua luat duoc tao boi luat r,s
	bool rulescompare(epnAR* r, epnAR* s, unsigned int* position, int* posnegflag); // kiem tra 2 luat co phu hop de ghep cap

	bool samerules(epnAR* r, epnAR* s);
	void pruncesamerules(ARlist *LE);
	bool samevalidrules(epnAR *Rule);
	bool samevalidnotredundantrules(epnAR *Rule);
	//void getvalidrules(ARlist* LE); // tu cac luat am duong, loc ra cac luat co gia tri, loai bo du thua va dua vao danh sach validRules theo thu tu
	void getvalidrules();
	void getvalidnotredundantrules();
	bool includerules(epnAR* r, epnAR* s);
	vector<epnAR> sortrules(vector<epnAR> Rules);
	bool ruletransmatching(epnAR* r, vector<unsigned int> InputTrans);
public:
	EPNAR_Process(DA2dims *Data,unsigned int Items, unsigned int Trans);
	~EPNAR_Process(void);
};

