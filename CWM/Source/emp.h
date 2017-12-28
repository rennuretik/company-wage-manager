#ifndef EMP_H
#define EMP_H
#include <string.h>
#include <typeinfo.h>

#define WB 3000
#define TB 7000
#define MB 12000

class Wage{
	public:
		Wage(){};
		Wage(float b, float bo = 0):base(b),bonus(bo){};
		~Wage(){};
		void SetBase(float b){base=b;};
		float GetBase() const{return base;};
		void SetBonus(float b){bonus=b;};
		float GetBonus() const{return bonus;};
		void operator= (const Wage& w){base=w.base;bonus=w.bonus;}
	private:
		float base;
		float bonus;

	friend class Employee;
};

class Employee{
	public:
		Employee(){};
		Employee(long long id, char n[23],char s, char t, long long c, Wage w):ID(id),sex(s),cell(c),wage(w){strcpy(name,n);t<<=6;sex|=t;};
		virtual ~Employee() = 0; 

		void SetID(long long id){ID=id;};
		long long GetID() const{return ID;};
		void SetName(char n[23]){strcpy(name,n);};
		char* GetName(char* bufin) const{strcpy(bufin,name);};
		void SetSex(char s){sex=s;};
		char GetSex() const{return sex;};
		void SetCell(long long c){cell=c;};
		long long GetCell() const{return cell;};
		void SetWage(Wage w){wage=w;};
		Wage GetWage() const{return wage;};
		void SetWageBonus(float bo){wage.SetBonus(bo);};
		virtual void Reward() = 0;
		virtual void Punish() = 0;
		
		friend class EmpNode;
		friend class EmpList;
		friend class interpreter;		
	protected:
		long long ID;
		char name[23];
		char sex;
		long long cell;
		Wage wage;
};


class Worker : public Employee{
	public:
		Worker(long long ID, char name[23],char sex, long long cell):Employee(ID,name,sex,0,cell,Wage(3000,0)){count++;}; // 初始化列表 
		~Worker(){count--;};
		void Reward(){wage.SetBonus(wage.GetBonus()+400);};
		void Punish(){wage.SetBonus(wage.GetBonus()-400);};
		static int GetCount(){return count;};
	private:
		static int count;
};

class Tech : public Employee{
	public:
		Tech(){count++;}
		Tech(long long ID, char name[23],char sex, long long cell):Employee(ID,name,sex,1,cell,Wage(7000,0)){count++;};;
		~Tech(){count--;};
		void Reward(){wage.SetBonus(wage.GetBonus()+1000);};
		void Punish(){wage.SetBonus(wage.GetBonus()-1000);};
		static int GetCount(){return count;};
	private:
		static int count;
};

class Manager: public Employee{
	public:
		Manager(long long ID, char name[23],char sex, long long cell):Employee(ID,name,sex,2,cell,Wage(12000,0)){count++;};;
		~Manager(){count--;};
		void Reward(){wage.SetBonus(wage.GetBonus()+1800);};
		void Punish(){wage.SetBonus(wage.GetBonus()-1800);};
		static int GetCount(){return count;};
	private:
		static int count;
};

class EmpNode{
	public:
		EmpNode(){}
		EmpNode(const char* txt48B );
		EmpNode* GetNextShow() const{return nshow;};
		EmpNode* GetNextAll() const{return nall;};
		EmpNode* GetLastShow() const{return lshow;};
		EmpNode* GetLastAll() const{return lall;};
		Employee* GetData() const{return data;};
		
		void Refresh(const char* txt48B );
		
		friend class EmpList;
		friend class interpreter;
	private:
		EmpNode* nshow;
		EmpNode* nall;
		EmpNode* lshow;
		EmpNode* lall;
		Employee* data;
};

class EmpList{
	public:
		EmpNode* GetFirstShow() const{return head.GetNextShow();};
		EmpNode* GetFirstAll() const{return head.GetNextAll();};
		void SetNodes(const char* txt, int size);
		void GetNodes(char* txt);
		void Show();			
		void SelectShow(char* txt);

		void Insert(Employee* emp);
		void Insert(EmpNode* en);
		void Delete(EmpNode* emp);
		void Clear();
		
		friend class EmpNode;
		friend class interpreter;
	private:
		EmpNode head; // 不使用的头节点 
};
//////////////

enum ATT{NAME,MY_IDNO,SEX,CELL,WAGE,TYPE};
enum OP{B,E,S,BE,SE,NE};//B-->,E--=,S--<,BE-->=,SE--<=,NE--!=

class interpreter
{
	friend class EmpList;
public:
	void translate(char *);
	void interpret(EmpList *emp);
private:
	ATT attribute;
	OP operation;
	char* constrain;
};


#endif

