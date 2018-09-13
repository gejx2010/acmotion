//main.cpp  
//coding=utf-8
#pragma once  
#include<iostream>  
#include<fstream>  
#include<cstring>
#include<cstdio>
#include<utility>
#include<vector>
#include<ctime>
#include"TrieTreeNode.h"
#include"TrieTree.h"  
using namespace std;  
  
class TestStruct {
  //char a : 8;
  int c : 1;
  int d : 31;
};
  
int main()  
{  
    TrieTree<wchar_t>* t = new TrieTree<wchar_t>();  
    wchar_t* c1 = L"好ac";  
    wchar_t* c2 = L"好大";  
    wchar_t* c3 = L"好大一";  
    wchar_t* c4 = L"一棵树";  
    wchar_t* c5 = L"a";
    wchar_t* c6 = L"好大一棵树好大一";
    wchar_t* c7 = L"树好大一";
  
    wchar_t* s = L"好大一棵树好大一abc好ac人a";//要匹配的串  
    t->Insert(c1);  
    t->Insert(c2);  
    t->Insert(c3);  
    t->Insert(c4);  
    t->Insert(c5);
    t->Insert(c6);
    t->Insert(c7);
  
    //test(t);  
    t->SetFailedPointer();  
    t->PrintALL();  
    cout << endl << "匹配结果为:" << endl;  
    
    vector<PosInWord>  ql;
    ql = t->search(s);  
    cout << "共匹配" << ql.size() << "处模式串" << endl;  
    for (int i = 0; i < ql.size(); i++) {
        cout << ql[i].start << "," << ql[i].end << endl;
    }
    printf("before write.\n");
    t->write_struct("cur_byte.txt");
    printf("after write.\n");
    TrieTree<wchar_t>* nt = new TrieTree<wchar_t>();
    clock_t start_time = clock();
    //nt->load_struct("cur_byte.txt", nt);
    nt->load_struct("../tree_tupu/book_tree");
    printf("load dict need time: %f\n", ((float)clock() - start_time) / CLOCKS_PER_SEC);
    //nt->PrintALL();  
    //cout << "max_value:" << nt->max_value << endl;
    //cout << "max_wordlen:" << nt->max_wordlen << endl;
    //cout << "max_vid:" << nt->max_vid << endl;
    //cout << "max_vsz:" << nt->max_vsz << endl;
    //cout << "max_fpid:" << nt->max_fpid << endl;
    //cout << endl << "匹配结果为:" << endl;  
    
    vector<PosInWord>  nql;
    wchar_t* ss = L"龟鳖高效养殖技术图解与实例";
    nql = nt->search(ss);  
    cout << "共匹配" << nql.size() << "处模式串" << endl;  
    for (int i = 0; i < nql.size(); i++) {
        cout << nql[i].start << "," << nql[i].end << endl;
    }
    cout << "all points: " << nt->cntPoints << endl;
    cout << "size of wchar_t:" << sizeof(wchar_t) << endl;
    cout << "sizeof sin tree:" << sizeof(SimTrieTreeNode) << endl;
    cout << "sizeof TestStruct:" << sizeof(TestStruct) << endl;
    return 0;
} 

