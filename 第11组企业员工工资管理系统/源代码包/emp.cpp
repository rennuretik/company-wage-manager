#include "emp.h"
#include <windows.h>
Employee::~Employee(){}
int Worker ::count = 0;
int Tech   ::count = 0;
int Manager::count = 0;

EmpNode::EmpNode(const char* txt48B ){
	long long IDnum, Cellnum;
	char name[30];
	int i;
	for(i=0;i<23;i++){
		name[i] = txt48B[i+8];
	}
	name[i] = 0;
	this->lall = this->lshow = this->nall = this->nshow = 0;
	char type = (txt48B[31]>>6) & 3 ;
	if(type==0){
		this->data = new Worker(*(long long*)txt48B,name,txt48B[31],*(long long*)(txt48B+32));
		this->data->SetWageBonus(*(float*)(txt48B + 44));
	}else
	if(type == 1){
		this->data = new Tech(*(long long*)txt48B,name,txt48B[31],*(long long*)(txt48B+32));
		this->data->SetWageBonus(*(float*)(txt48B + 44));
	}else
	if(type == 2){
		this->data = new Manager(*(long long*)txt48B,name,txt48B[31],*(long long*)(txt48B+32));
		this->data->SetWageBonus(*(float*)(txt48B + 44));
	}
}

void EmpNode::Refresh(const char* txt48B ){
	delete this->data;
	
	long long IDnum, Cellnum;
	char name[30];
	int i;
	for(i=0;i<23;i++){
		name[i] = txt48B[i+8];
	}
	name[i] = 0;
	char type = (txt48B[31]>>6) & 3 ;
	if(type==0){
		this->data = new Worker(*(long long*)txt48B,name,txt48B[31],*(long long*)(txt48B+32));
		this->data->SetWageBonus(*(float*)(txt48B + 44));
	}else
	if(type == 1){
		this->data = new Tech(*(long long*)txt48B,name,txt48B[31],*(long long*)(txt48B+32));
		this->data->SetWageBonus(*(float*)(txt48B + 44));
	}else
	if(type == 2){
		this->data = new Manager(*(long long*)txt48B,name,txt48B[31],*(long long*)(txt48B+32));
		this->data->SetWageBonus(*(float*)(txt48B + 44));
	}

}

void EmpList::SetNodes(const char* txt, int size){
	const char* maker = txt;
	while(size){
		size-=48;
		int i;
		char name[24];
		EmpNode* nemp = new EmpNode;
		nemp->lall = nemp->nall = nemp->lshow = nemp->lall = 0;
		for(i=0;i<23;i++){
			name[i] = maker[i+8];
		}
		name[i] = 0;
		char type = (maker[31]>>6) & 3 ;
		if(type==0){
			nemp->data = new Worker(*(long long*)maker,name,maker[31],*(long long*)(maker+32));
			nemp->data->SetWage(Wage(3000,*(float*)(maker+44)));
		}else
		if(type == 1){
			nemp->data = new Tech(*(long long*)maker,name,maker[31],*(long long*)(maker+32));
			nemp->data->SetWage(Wage(7000,*(float*)(maker+44)));
		}else
		if(type == 2){
			nemp->data = new Manager(*(long long*)maker,name,maker[31],*(long long*)(maker+32));
			nemp->data->SetWage(Wage(12000,*(float*)(maker+44)));
		}
		this->Insert(nemp);
		maker += 48;
	}
}

void EmpList::GetNodes(char* txt){
	EmpNode* maker;
	char* writer = txt;
	maker = this->GetFirstAll();
	while(maker){
		*(long long*)writer = maker->GetData()->GetID();
		maker->GetData()->GetName(writer+8);
		writer[31] = maker->GetData()->GetSex();
		*(long long*)(writer+32) = maker->GetData()->GetCell();
		*(float*)(writer+44) = maker->GetData()->GetWage().GetBonus();
		int t = (writer[31]>>6) & 3;
		*(float*)(writer+40) = (t==2)?12000:(t==1?7000:3000);
		 
		writer += 48;
		maker = maker->nall;
	}
}

void EmpList::Insert(Employee* emp){
	EmpNode *nemp = new EmpNode;
	nemp->nall = nemp->lall = nemp->nshow = nemp->lshow=0;
	nemp->data = emp;
	
	if(this->head.nall){
		this->head.nall->lall = nemp;
	}
	nemp->nall=this->head.nall;
	this->head.nall = nemp;
	nemp->lall = &(this->head);

	if(this->head.nshow){
		this->head.nshow->lshow = nemp;
	}
	nemp->nshow=this->head.nshow;
	this->head.nshow = nemp;
	nemp->lshow = &(this->head);
}

void EmpList::Insert(EmpNode* nemp){
	if(this->head.nall){
		this->head.nall->lall = nemp;
	}
	nemp->nall=this->head.nall;
	this->head.nall = nemp;
	nemp->lall = &(this->head);

	if(this->head.nshow){
		this->head.nshow->lshow = nemp;
	}
	nemp->nshow=this->head.nshow;
	this->head.nshow = nemp;
	nemp->lshow = &(this->head);	
}

void EmpList::Delete(EmpNode* en){
	if(en->GetNextAll()){
		en->GetNextAll()->lall = en->GetLastAll();
	}
	en->GetLastAll()->nall = en->GetNextAll();
	
	if(en->GetNextShow()){
		en->GetNextShow()->lshow = en->GetLastShow();
	}
	en->GetLastShow()->nshow = en->GetNextShow();
	
	delete en->GetData();
	delete en;
}

void EmpList::Clear(){
	EmpNode* en;
	while(en = GetFirstAll()){
		Delete(en);
	}
}

void EmpList::SelectShow(char* txt){
			interpreter it;
			it.translate(txt);

			it.interpret(this);
}
		
void EmpList::Show(){
		this->head.nshow = this->head.nall;
		EmpNode* t = this->GetFirstAll();
		while(t){
			t->lshow = t->lall;
			t->nshow = t->nall;
			t = t->GetNextAll();
		}
}
		
		

void interpreter::translate(char *text){
	char str[300];
	char op[20]={0};
	int i;
	int j=0;
	for(i=0;*(text+i)!=' ';i++){
		*(str+i)=*(text+i);
	}
	str[i++]='\0';
	while(text[i]==' ')
		i++;
	if(!strcmp(str,"姓名"))
		attribute= NAME;
	else if(!strcmp(str,"工号"))
		attribute= MY_IDNO;
	else if(!strcmp(str,"性别"))
		attribute= SEX;
	else if(!strcmp(str,"手机"))
		attribute= CELL;
	else if(!strcmp(str,"工资"))
		attribute= WAGE;
	else if(!strcmp(str,"人员类别"))
		attribute= TYPE;
	while(text[i]!=' ')
		op[j++]=text[i++];
	if(!strcmp(op,">"))
			  operation=B;
	else if(!strcmp(op,"="))
			  operation=E;
	else if(!strcmp(op,"<"))
			  operation=S;
	else if(!strcmp(op,">="))
			  operation=BE;
	else if(!strcmp(op,"<="))
			  operation=SE;
	else if(!strcmp(op,"!="))
			  operation=NE;
	
	i++;
	for(int j=0;*(text+i)!='\0';j++){
	 constrain[j]=text[i++];
	}
}

void interpreter::interpret(EmpList* el){
	long long num=0;
	int i;
	EmpNode *present;
	if(constrain[0]>='0' && constrain[0]<='9')
	for(i=0;constrain[i]!='\0';i++){
		num*=10;
		num+=constrain[i]-'0';
	}
	present=el->GetFirstShow();
	switch(attribute){
	case NAME:
		while(present){
			if(strcmp(constrain,present->GetData()->name)){
			present->lshow->nshow=present->nshow;
			if(present->nshow)
			present->nshow->lshow=present->lshow;
			}
			present=present->nshow;
		}break;

	case MY_IDNO:
		switch(operation){
		case B:
			while(present){
				if(num>=present->data->ID){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case E:
			while(present){
				if(num!=present->data->ID){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case S:
			while(present){
				if(num<=present->data->ID){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case NE:
			while(present){
				if(num==present->data->ID){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case SE:
			while(present){
				if(num<present->data->ID){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case BE:
			while(present){
				if(num>present->data->ID){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
			
		}break;
	case SEX:
		while(present){
			char sexstr[20];
			if(present->GetData()->GetSex() & 1){
				strcpy(sexstr,"男");
			}else{
				strcpy(sexstr,"女");
			}
			if(strcmp(constrain,sexstr)){
			present->lshow->nshow=present->nshow;
			if(present->nshow)
			present->nshow->lshow=present->lshow;
			}
			present=present->nshow;
		}break;
	case CELL:
		while(present){
			if(num!=present->data->cell){
			present->lshow->nshow=present->nshow;
			if(present->nshow)
			present->nshow->lshow=present->lshow;
			}
			present=present->nshow;
		}break;
	case WAGE:
		switch(operation){
		case B:
			while(present){
				if(num>=(float)(present->data->wage.GetBase()+present->data->wage.GetBonus())){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case E:
			while(present){
				if(num!=(float)(present->data->wage.GetBase()+present->data->wage.GetBonus())){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case S:
			while(present){
				if(num<=(float)(present->data->wage.GetBase()+present->data->wage.GetBonus())){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case NE:

			while(present){
				if(num==(float)(present->data->wage.GetBase()+present->data->wage.GetBonus())){

				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case SE:
			while(present){
				if(num<(float)(present->data->wage.GetBase()+present->data->wage.GetBonus())){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		case BE:
			while(present){
				if(num>(float)(present->data->wage.GetBase()+present->data->wage.GetBonus())){
				present->lshow->nshow=present->nshow;
			if(present->nshow)
				present->nshow->lshow=present->lshow;
				}
				present=present->nshow;
			}break;
		}break;

	case TYPE:

		if(!strcmp(constrain,"技术人员")){
		while(present){
			if(strcmp(typeid(*(present->data)).name(),"4Tech")){
			present->lshow->nshow=present->nshow;
			if(present->nshow)
			present->nshow->lshow=present->lshow;
			}
			present=present->nshow;
		}
		}
		else if(!strcmp(constrain,"管理人员")){
		while(present){
			if(strcmp(typeid(*(present->data)).name(),"7Manager")){
			present->lshow->nshow=present->nshow;
			if(present->nshow)
			present->nshow->lshow=present->lshow;
			}
			present=present->nshow;
		}
		}
		else if(!strcmp(constrain,"工人")){
//		MessageBox (NULL, constrain , "About...", 0);

		while(present){
			if(strcmp(typeid(*(present->data)).name(),"6Worker")){
			present->lshow->nshow=present->nshow;
			if(present->nshow)
			present->nshow->lshow=present->lshow;
			}
			present=present->nshow;
		}
		}
	}
}

