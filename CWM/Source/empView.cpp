#include"empView.h"
#include<windowsx.h>
#include<sstream>
#include<cstdio>
#include<cstring>
using namespace std;

long long str2ll(char* s){
	long long result = 0;
	int i;
	for(i=0;i<strlen(s);i++){
		result *= 10;
		result += (s[i] - '0');
	}
	return result;
}

void Row::Display(const Employee* e){
	stringstream tmp;
	string t;
	
	tmp<<e->GetID();
	tmp>>t;
	SetWindowText(this->IDTxt,t.c_str());
		
	char name[19];
	e->GetName(name);
	Edit_SetText(this->NameTxt,name);
	
	t="";
	t=(e->GetSex()&1)?"男":"女"; 
	Edit_SetText(this->SexTxt,t.c_str());
	
	t="";
	char type;
	type = (e->GetSex()>>6)&3;
	if(!type) t = "工人";
	if(type==1) t = "技术人员";
	if(type==2) t= "管理人员";
	Edit_SetText(this->TypeTxt,t.c_str());

	sprintf(name,"%lld",e->GetCell());
	Edit_SetText(this->CellTxt,name);

	sprintf(name,"%.2f",e->GetWage().GetBonus());
	Edit_SetText(this->BonusTxt,name);

	t="";
	tmp.clear();
	sprintf(name,"%.2f",e->GetWage().GetBonus()+e->GetWage().GetBase());
	Edit_SetText(this->TotalTxt,name);
}

void Row::ShapeEmp(Employee* e){
	e->SetWageBonus(this->GetBonusV());
	e->SetID(this->GetIDV());
	char nm[23] = {0};
	this->GetNameV(nm);
	e->SetName(nm);
	char sex = this->GetSexV();
	sex |= ((this->GetTypeV())<<6);
	e->SetSex(sex);
	e->SetCell(this->GetCellV());
}

long long Row::GetIDV() const{
	char bufin[20];
	Edit_GetText(this->IDTxt,bufin,20);
	return str2ll(bufin);
}

void Row::GetNameV(char* bufin) const{
	Edit_GetText(this->NameTxt,bufin,20);
}

char Row::GetSexV() const{
	char bufin[20];
	Edit_GetText(this->SexTxt,bufin,20);
	if(strcmp("男",bufin)){
		return 0;
	}else{
		return 1;
	}
}

char Row::GetTypeV() const{
	char bufin[20];
	Edit_GetText(this->TypeTxt,bufin,20);
	if(!strcmp(bufin,"工人")) return 0;
	if(!strcmp(bufin,"技术人员")) return 1;
	if(!strcmp(bufin,"管理人员")) return 2;
}

long long Row::GetCellV() const{
	char bufin[20];
	Edit_GetText(this->CellTxt,bufin,20);
	return str2ll(bufin); 
}

float Row::GetBonusV() const{
	char bufin[20];
	Edit_GetText(this->BonusTxt,bufin,20);
	return atof(bufin);
}

void Row::Empty(){
	Edit_SetText(this->NameTxt,"");
	Edit_SetText(this->IDTxt,"");
	Edit_SetText(this->SexTxt,"");
	Edit_SetText(this->TypeTxt,"");
	Edit_SetText(this->CellTxt,"");
	Edit_SetText(this->BonusTxt,"");
	Edit_SetText(this->TotalTxt,"");
}

void Row::Get48B(char* bufin) const{
	float base;
	
	char IDstr[20];
	Edit_GetText(this->IDTxt,IDstr,20);
	long long IDnum = str2ll(IDstr);
	*(long long *)bufin=IDnum;
	
	Edit_GetText(this->NameTxt,bufin+8,23);
	
	char Sexstr[20];
	Edit_GetText(this->SexTxt,Sexstr,20);
	if(!strcmp(Sexstr,"男")){
		bufin[31] = 1;
	}else{
		bufin[31] = 0;
	}
	
	char Typestr[20];
	Edit_GetText(this->TypeTxt, Typestr, 20);
	if(!strcmp(Typestr,"工人")){
		base = 3000;
	}else
	if(!strcmp(Typestr,"技术人员")){
		base = 7000;
		bufin[31] |= 0x40;
	}else
	if(!strcmp(Typestr,"管理人员")){
		base = 12000;
		bufin[31] |= 0x80;
	}
	
	char Cellstr[20];
	Edit_GetText(this->CellTxt,Cellstr,20);
	long long Cellnum = str2ll(Cellstr);
	*(long long *)(bufin+32)=Cellnum;

	char Bonusstr[20];
	Edit_GetText(this->BonusTxt,Bonusstr,20);
	float Bonusnum = atof(Bonusstr);
	*(float*)(bufin+40) = base;
	
	*(float*)(bufin+44) = (Bonusnum);
}
