#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include <cstring>
#include "main.h"
#include "emp.h"
#include "empView.h"
#include <typeinfo.h>

EmpList emps;
EmpNode* displayer = emps.GetFirstShow();
EmpNode* restore;
EmpNode* traveller;
HWND gh;

char chosen[15];
char query[300];
Row rows[15];
HWND but[7];
HWND sb[15];	
HWND refresh;
HWND search;
HWND filter[2];
char butn[7][20]={"上页","下页","全选此页","删除选中","确认更新","奖","惩"};
char fbutn[2][20]={"重置为所有","当前结果中搜索"};
char left[12][20];
char right[12][20];
int c1[4];
float c2[4];
float c3[4];
int c4[4];
float c5[4];
float c6[4];

BOOL LoadFile(HWND hEdit, LPSTR pszFileName) {
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if(hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSize;
		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != 0xFFFFFFFF) {
			LPSTR pszFileText;
			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if(pszFileText != NULL) {
				DWORD dwRead;
				if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL)) {
					pszFileText[dwFileSize] = 0; // Null terminator
					emps.Clear();

					emps.SetNodes(pszFileText, dwFileSize);
					displayer = emps.GetFirstShow();
					traveller = displayer;
					int i;
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					
					for(i;i<15;i++){
						rows[i].Empty();
					}

						bSuccess = TRUE; // It worked!
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL SaveFile(HWND hEdit, LPSTR pszFileName) {
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile != INVALID_HANDLE_VALUE) {
		DWORD dwTextLength;
		dwTextLength = GetWindowTextLength(hEdit);
		if(dwTextLength > 0 || 1) {
					DWORD dwWritten;
					int lth = 48 * (Worker::GetCount()+Tech::GetCount()+Manager::GetCount());
					char* myText = (char*)malloc(lth+1);
					memset(myText,0,lth+1);
				emps.GetNodes(myText);
					WriteFile(hFile,myText,lth,&dwWritten,NULL);
					bSuccess = TRUE;
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL DoFileOpenSave(HWND hwnd, BOOL bSave) {
	gh=hwnd;
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	szFileName[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Bin Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";

	if(bSave) {
		ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
		if(GetSaveFileName(&ofn)) {
			if(!SaveFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName)) {
				MessageBox(hwnd, "Save file failed.", "Error",MB_OK|MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	} else {
		ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		if(GetOpenFileName(&ofn)) {
			if(!LoadFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName)) {
				MessageBox(hwnd, "Load of file failed.", "Error",MB_OK|MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT Ps;  
	char str[50];
	char unit[48] = {0};
	int i,j;
	int creation;
	switch(Message) {
		HDC hDC;
		
		case WM_CREATE:
			for(i=0;i<15;i++){
				sb[i] = CreateWindow("Button","选择",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,265,45+25*i,45,20,hwnd,(HMENU)((301+i)),GetModuleHandle(0),0); 
				rows[i].SetID( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,320,45+25*i,100,20,hwnd,(HMENU)((i+1)*10+1),GetModuleHandle(NULL),0));
				rows[i].SetName( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,430,45+25*i,120,20,hwnd,(HMENU)((i+1)*10+2),GetModuleHandle(NULL),0));
				rows[i].SetSex( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,560,45+25*i,25,20,hwnd,(HMENU)((i+1)*10+3),GetModuleHandle(NULL),0));
				rows[i].SetType( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,595,45+25*i,100,20,hwnd,(HMENU)((i+1)*10+4),GetModuleHandle(NULL),0));
				rows[i].SetCell( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,705,45+25*i,100,20,hwnd,(HMENU)((i+1)*10+5),GetModuleHandle(NULL),0));
				rows[i].SetBonus( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,815,45+25*i,80,20,hwnd,(HMENU)((i+1)*10+6),GetModuleHandle(NULL),0));
				rows[i].SetTotal( CreateWindow ("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,905,45+25*i,80,20,hwnd,(HMENU)((i+1)*10+7),GetModuleHandle(NULL),0));
			}
			for(i=0;i<7;i++){
				but[i] = CreateWindow("Button",butn[i],WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,320+96*i,450,80,50,hwnd,(HMENU)((i+1)),GetModuleHandle(0),0); 
			}
			refresh = CreateWindow("Button","更新统计信息",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,1020,450,160,50,hwnd,(HMENU)REFRESHBTN,GetModuleHandle(0),0); 
			for(i=0;i<2;i++){
				filter[i] = CreateWindow("Button",fbutn[i],WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,20+130*i,450,120,50,hwnd,(HMENU)((201+i)),GetModuleHandle(0),0); 
			}
			search = CreateWindow("EDIT","",WS_BORDER|WS_CHILD|WS_VISIBLE,20,395,220,20,hwnd,(HMENU)(401),GetModuleHandle(NULL),0);
			
			traveller = displayer;

			for(i=0;i<15;i++){

				if(traveller){
					rows[i].Display(traveller->GetData());	
					traveller = traveller->GetNextShow();
				}
				else
					break;
				}
			break;
		case WM_SIZE:
			if(wParam != SIZE_MINIMIZED)
				MoveWindow(GetDlgItem(hwnd, IDC_MAIN_TEXT), 0, 0, LOWORD(lParam),HIWORD(lParam), TRUE);
			break;
		case WM_SETFOCUS:
			SetFocus(GetDlgItem(hwnd, IDC_MAIN_TEXT));
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case CM_FILE_OPEN:
					DoFileOpenSave(hwnd, FALSE);
					break;
				case CM_FILE_SAVEAS:
					DoFileOpenSave(hwnd, TRUE);
					break;
				case CM_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				case CM_ABOUT:
					MessageBox (NULL, "Company Wage Manager for Windows!\nCreated using the Win32 API" , "About...", 0);
					break;
				case FB1: // 全局搜索
					memset(chosen,0,15);
					emps.Show();
					displayer = emps.GetFirstShow();
					traveller = displayer;
		
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					
					for(i;i<15;i++){
						rows[i].Empty();
					}

					 ::InvalidateRect(hwnd,NULL,TRUE);
					break;
				case FB2: // 当前结果中搜索
					memset(chosen,0,15);
					GetDlgItemText(hwnd,SRCH,query,300);
					
					emps.SelectShow(query);

					displayer = emps.GetFirstShow();
					traveller = displayer;
		
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					
					for(i;i<15;i++){
						rows[i].Empty();
					}
					
					 ::InvalidateRect(hwnd,NULL,TRUE);
					break;
				case B1: // 上页
					memset(chosen,0,15);
					restore = displayer;
					if(emps.GetFirstShow()==0){
							displayer = restore;
							MessageBox(hwnd, "无更多信息", "Error",MB_OK|MB_ICONEXCLAMATION);
							return 0; 
						}
					for(i=0;i<14;i++){
						if(!(displayer->GetData())){
							displayer = restore;
							MessageBox(hwnd, "无更多信息", "Error",MB_OK|MB_ICONEXCLAMATION);
							return 0; 
						}
						displayer = displayer->GetLastShow();
						if(!(displayer->GetData())){
							displayer = restore;
							MessageBox(hwnd, "无更多信息", "Error",MB_OK|MB_ICONEXCLAMATION);
							return 0;		
						}
					}
					traveller = displayer;
		
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					
				
					 ::InvalidateRect(hwnd,NULL,TRUE);
					break;
				case B2: // 下页
					memset(chosen,0,15);
					restore = displayer;
					for(i=0;i<14;i++){
						if(!displayer){
							displayer = restore;
							MessageBox(hwnd, "无更多信息", "Error",MB_OK|MB_ICONEXCLAMATION);
							return 0; 
						}
						displayer = displayer->GetNextShow();
						if(!displayer){
							displayer = restore;
							MessageBox(hwnd, "无更多信息", "Error",MB_OK|MB_ICONEXCLAMATION);
							return 0;		
						}
					}
					traveller = displayer;
		
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					
					for(i;i<15;i++){
						rows[i].Empty();
					}
				
					 ::InvalidateRect(hwnd,NULL,TRUE);
					break;
				case B3: // 全选此页 
					for(i=0;i<15;i++)if(!chosen[i])break;
					
					if(i==15)for(i=0;i<15;i++)chosen[i]=0;
					else for(i=0;i<15;i++)chosen[i]=1;
					 ::InvalidateRect(hwnd,NULL,TRUE);
					break;
				case B4: // 删除选中 
					traveller=displayer;
					for(i=0;i<15;i++){
						if(!traveller) break;
						restore = traveller->GetNextShow();
						if(chosen[i]){
							emps.Delete(traveller);
						}
						traveller = restore;
					}
					
					displayer=emps.GetFirstShow();
					traveller = displayer;
					memset(chosen,0,15);
					for(i=0;i<15;i++){
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					for(i;i<15;i++){
						rows[i].Empty();
					}
					 ::InvalidateRect(hwnd,NULL,TRUE);
					
					break;
				case B5: // 确认更新 
					creation = 0;
					traveller = displayer;
					
					for(i = 0;i<15;i++){
						rows[i].Get48B(unit);
						if(strlen(unit)==0)break;
						if(creation){
							EmpNode* nemp = new EmpNode(unit);
							emps.Insert(nemp);
							continue;
						}
						if(traveller){
							traveller->Refresh(unit);
							traveller = traveller->GetNextShow();
							continue;
						}else{
							creation = 1;
							EmpNode* nemp = new EmpNode(unit);
							emps.Insert(nemp);
						}
					}
					displayer = emps.GetFirstShow();
					traveller = displayer;
					for(i=0;i<15;i++){
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					 ::InvalidateRect(hwnd,NULL,TRUE);
					break;
				case B6: // 奖 
					traveller=displayer;
					for(i=0;i<15;i++){
						if(!traveller) break;
						restore = traveller->GetNextShow();
						if(chosen[i]){
							traveller->GetData()->Reward();
						}
						traveller = restore;
					}
					memset(chosen,0,15);

					traveller = displayer;
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					 ::InvalidateRect(hwnd,NULL,TRUE);
					
					break;
				case B7: // 惩 
					traveller=displayer;
					for(i=0;i<15;i++){
						if(!traveller) break;
						restore = traveller->GetNextShow();
						if(chosen[i]){
							traveller->GetData()->Punish();
						}
						traveller = restore;
					}
					memset(chosen,0,15);
					traveller = displayer;
					for(i=0;i<15;i++){
		
						if(traveller){
							rows[i].Display(traveller->GetData());	
							traveller = traveller->GetNextShow();
						}
						else
							break;
					}
					 ::InvalidateRect(hwnd,NULL,TRUE);
					
					break;
				case REFRESHBTN: // 更新统计信息
					 ::InvalidateRect(hwnd,NULL,TRUE);
           			break;
           			
           		case SB1:
           			chosen[0]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);
            			break;
          		case SB2:
            		chosen[1]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           		case SB3:
            		chosen[2]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           		
           		case SB4:
            		chosen[3]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB5:
            		chosen[4]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB6:
            		chosen[5]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           		
           		case SB7:
            		chosen[6]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB8:
            		chosen[7]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB9:
            		chosen[8]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB10:
            		chosen[9]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB11:
            		chosen[10]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB12:
            		chosen[11]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB13:
            		chosen[12]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB14:
            		chosen[13]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
           			
           		case SB15:
            		chosen[14]^=1;
					::InvalidateRect(hwnd,NULL,TRUE);          			
           			break;
		   }
			break;
			
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_PAINT://设计编辑框  
           hDC=BeginPaint(hwnd,&Ps);  
           TextOut(hDC,320,20,"工号",4);  
           TextOut(hDC,430,20,"姓名",4);  
           TextOut(hDC,560,20,"性别",4);  
           TextOut(hDC,595,20,"人员类别",8);  
           TextOut(hDC,705,20,"手机",4);  
           TextOut(hDC,815,20,"本月奖金",8);  
           TextOut(hDC,905,20,"本月总薪水",10);
           
		   TextOut(hDC,1000,20,"全员统计",8);
		   TextOut(hDC,1055,45,"人数",4);
		   TextOut(hDC,1120,45,"总工资",6);
		   TextOut(hDC,1200,45,"人均工资",8);
		   TextOut(hDC,1000,70,"工人",4);
		   TextOut(hDC,1000,95,"技术",4);
		   TextOut(hDC,1000,120,"管理",4);
		   TextOut(hDC,1000,145,"全员",4);
		   traveller = emps.GetFirstAll();
		   for(i=0;i<4;i++){
		   	c1[i] = 0;
		   	c2[i] = 0;
		   	c3[i] = 0;
		   	c4[i] = 0;
		   	c5[i] = 0; 
		   	c6[i] = 0;
		   }
		   while(traveller){
			int n = typeid(*(traveller->GetData())).name()[0]-'0';
			c1[0] = Worker::GetCount();
			c1[1] = Tech::GetCount();
			c1[2] = Manager::GetCount();
			if(n==6){
				c2[0]+=traveller->GetData()->GetWage().GetBase()+traveller->GetData()->GetWage().GetBonus();
			}else
			if(n==4){
				c2[1]+=traveller->GetData()->GetWage().GetBase()+traveller->GetData()->GetWage().GetBonus();				
			}else{
				c2[2]+=traveller->GetData()->GetWage().GetBase()+traveller->GetData()->GetWage().GetBonus();				
			} 
		   	traveller = traveller->GetNextAll();
		   }
		   c1[3] = c1[0] + c1[1] + c1[2];
		   c2[3] = c2[0] + c2[1] + c2[2];
		   c3[0] = c2[0] / c1[0];
		   c3[1] = c2[1] / c1[1];
		   c3[2] = c2[2] / c1[2];
		   c3[3] = c2[3] / c1[3];
		   for(i=0;i<4;i++){
		   sprintf(right[i],"%d",c1[i]);
		   TextOut(hDC,1055,70+25*i,right[i],strlen(right[i]));    
		   }
		   for(i=0;i<4;i++){
		   sprintf(right[4+i],"%.2f",c2[i]);
		   TextOut(hDC,1120,70+25*i,right[4+i],strlen(right[4+i]));		   	
		   }
		   for(i=0;i<4;i++){
		   sprintf(right[8+i],"%.2f",c3[i]);
		   TextOut(hDC,1200,70+25*i,right[8+i],strlen(right[8+i]));		   	
		   }
		   
 		   TextOut(hDC,1000,200,"当前结果统计",12);
		   TextOut(hDC,1055,225,"人数",4);
		   TextOut(hDC,1120,225,"总工资",6);
		   TextOut(hDC,1200,225,"人均工资",8);
		   TextOut(hDC,1000,250,"工人",4);
		   TextOut(hDC,1000,275,"技术",4);
		   TextOut(hDC,1000,300,"管理",4);
		   TextOut(hDC,1000,325,"全员",4);		   
		   traveller = emps.GetFirstShow();

		   while(traveller){
			int n = typeid(*(traveller->GetData())).name()[0]-'0';
			if(n==6){
				c4[0]++;
				c5[0]+=traveller->GetData()->GetWage().GetBase()+traveller->GetData()->GetWage().GetBonus();
			}else
			if(n==4){
				c4[1]++;
				c5[1]+=traveller->GetData()->GetWage().GetBase()+traveller->GetData()->GetWage().GetBonus();				
			}else{
				c4[2]++;
				c5[2]+=traveller->GetData()->GetWage().GetBase()+traveller->GetData()->GetWage().GetBonus();				
			} 
		   	traveller = traveller->GetNextShow();
		   }
		   c4[3] = c4[0] + c4[1] + c4[2];
		   c5[3] = c5[0] + c5[1] + c5[2];
		   
		   c6[0] = c5[0] / c4[0];
		   c6[1] = c5[1] / c4[1];
		   c6[2] = c5[2] / c4[2];
		   c6[3] = c5[3] / c4[3];
		   for(i=0;i<4;i++){
		   sprintf(left[i],"%d",c4[i]);
		   TextOut(hDC,1055,250+25*i,left[i],strlen(left[i]));    
		   }
		   for(i=0;i<4;i++){
		   sprintf(left[4+i],"%.2f",c5[i]);
		   TextOut(hDC,1120,250+25*i,left[4+i],strlen(left[4+i]));		   	
		   }
		   for(i=0;i<4;i++){
		   sprintf(left[8+i],"%.2f",c6[i]);
		   TextOut(hDC,1200,250+25*i,left[8+i],strlen(left[8+i]));		   	
		   }
		   
		   for(i=0;i<15;i++){
		   	if(chosen[i]){
     		      TextOut(hDC,220,45+25*i,"已选",4);  		   		
			   }else{
			   }
		   }
		   
           EndPaint(hwnd,&Ps); 
           return 0;  
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow) {
	Worker* wks[10];
	Tech* tcs[10];
	Manager* mns[10];
	int i;
	for(i=0;i<10;i++){
		wks[i] = new Worker(i,"a",1,13800000000);
		tcs[i] = new Tech(10+i,"b",0,13900000000);
		mns[i] = new Manager(20+i,"c",0,18900000000);
		emps.Insert(wks[i]);
		emps.Insert(tcs[i]);
		emps.Insert(mns[i]);
	}
	memset(chosen,0,15);
	displayer = emps.GetFirstShow();
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = "MAINMENU";
	wc.lpszClassName = "WindowClass";
	wc.hIconSm       = LoadIcon(hInstance,"A"); /* A is name used by project icons */

	if(!RegisterClassEx(&wc)) {
		MessageBox(0,"Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK|MB_SYSTEMMODAL);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Company Wage Manager",WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1300,600,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL) {
		MessageBox(0, "Window Creation Failed!", "Error!",MB_ICONEXCLAMATION|MB_OK|MB_SYSTEMMODAL);
		return 0;
	}

	ShowWindow(hwnd,1);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
