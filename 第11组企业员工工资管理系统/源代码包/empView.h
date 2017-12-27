#ifndef EMPVIEW_H
#define EMPVIEW_H
#include <windows.h>
#include "emp.h"

class Row{
	public:
		HWND GetID() const{return IDTxt;}
		void SetID(HWND h){IDTxt=h;} 
		HWND GetName() const{return NameTxt;}
		void SetName(HWND h){NameTxt=h;}
		HWND GetSex() const{return SexTxt;}
		void SetSex(HWND h){SexTxt=h;}
		HWND GetType() const{return TypeTxt;}
		void SetType(HWND h){TypeTxt=h;}
		HWND GetCell() const{return CellTxt;}
		void SetCell(HWND h){CellTxt=h;}
		HWND GetBonus() const{return BonusTxt;}
		void SetBonus(HWND h){BonusTxt=h;}
		HWND GetTotal() const{return TotalTxt;}
		void SetTotal(HWND h){TotalTxt=h;}

		long long GetIDV() const;
		void GetNameV(char* bufin) const;
		char GetSexV() const;
		char GetTypeV() const;
		long long GetCellV() const;
		float GetBonusV() const;

		void Display(const Employee* e);
		void ShapeEmp(Employee* e);
		
		void Empty();
		void Get48B(char* bufin) const;
	private:
		HWND IDTxt;
		HWND NameTxt;
		HWND SexTxt;
		HWND TypeTxt;
		HWND CellTxt;
		HWND BonusTxt;
		HWND TotalTxt;
};

#endif
