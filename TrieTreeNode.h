//TrieTreeNode.h  
#pragma once  
#include<iostream>  
#include<vector>
#include<cstdio>
using namespace std;  

template<class T>  
class TrieTreeNode  
{  
public:  
    TrieTreeNode(wchar_t c = L' ', int id = -1)//用于构造根节点  
    {  
        value = c;
        ChildNodes.clear();  
        word = 0;  
        isEnd = 0;
        wordlen = 0;  
        FailedPointer = NULL;  
        Freq = 0;  
        ID = id;  
        vid = 0;
        vsz = 0;
        fpid = 0;
    }  
public:  
    //int MaxBranchNum;//最大分支数；  
    bool word;//单词字符串的指针  
    bool isEnd;
    wchar_t value;
    int wordlen;  
    int vsz;
    int Freq;//词频统计  
    int ID;//构建TrieTree树时的插入顺序，可用来记录字符串第一次出现的位置  
    int fpid;
    long long vid;
    vector<TrieTreeNode<T>*> ChildNodes;  
    TrieTreeNode<T> *FailedPointer;  
    // use for sort
    bool operator() (TrieTreeNode<T>* a, TrieTreeNode<T>* b) {
        return ((int) a->value) < ((int) b->value);
    }
};  

class SimTrieTreeNode  
{  
public:  
    SimTrieTreeNode(wchar_t c = L' ', int id = -1)//用于构造根节点  
    {  
        set_value(c);
        set_wordlen(0);  // use this to compute the length of match query in O(1), take 2 bytes
        set_isWord(0);
        //set_isEnd(0);
        set_vid(0);
        set_vsz(0);
        set_fpid(0);
    }  
public:  
    //wchar_t value;
    //short int wordlen;  
    //short int vsz;
    //int fpid;
    // 用int替代long long的vid，之前用long long是为了有足够的
    // 空间访问
    //int vid;

    // 简化树结构，用一块14字节的内存存储所有内容
    // 二次简化：仅用10字节的内存存储所有内容
    // 三次修改: 增加wordlen和vsz，主要为了增加搜索效率，占用14个字节
    unsigned char content[14];
    // 对原先的每个变量设置set和get方法
    // value: unsigned short int
    unsigned short get_value() {
        return *( (unsigned short*)(content) ); 
    }
    void set_value(unsigned short v) {
        unsigned short* bef = (unsigned short*)content;
        *bef = v;
    }
    // wordlen: unsigned short int
    unsigned short get_wordlen() {
        return *( (unsigned short*)(content + 2) ); 
    }
    void set_wordlen(unsigned short v) {
        unsigned short* bef = (unsigned short*) (content + 2);
        *bef = v;
    }
    // vsz: unsigned short int
    unsigned short get_vsz() {
        return *( (unsigned short*)(content + 4) ); 
    }
    void set_vsz(unsigned short v) {
        unsigned short* bef = (unsigned short*) (content + 4);
        *bef = v;
    }
    //// isEnd: mark bit in the highest byte of vid
    //bool get_isEnd() {
    //    unsigned int bef = *( (unsigned int *)(content + 6) );
    //    return bef & 0x80000000; 
    //}
    //void set_isEnd(bool v) {
    //    unsigned int* bef = (unsigned int*)(content + 6);
    //    if (v) *bef |= 0x80000000;
    //    else *bef &= 0x7fffffff;
    //}
    // vid: unsigned int
    // the first bit of vid is isEnd mark bit
    unsigned int get_vid() {
        unsigned int bef = *( (unsigned int *)(content + 6) );
        return (unsigned int)(bef & 0x7fffffff); 
    }
    void set_vid(unsigned int v) {
        unsigned int* bef = (unsigned int*)(content + 6);
        *bef = v;
        //if (get_isEnd()) *bef = v | 0x80000000;
        //else *bef = v;
    }
    // isWord: bool
    bool get_isWord() {
        unsigned int bef = *( (unsigned int *)(content + 10) );
        return bef & 0x80000000; 
    }
    void set_isWord(bool v) {
        unsigned int* bef = (unsigned int*)(content + 10);
        if (v) *bef |= 0x80000000;
        else *bef &= 0x7fffffff;
    }
    //fpid: unsigned int
    unsigned int get_fpid() {
        unsigned int bef = *( (unsigned int *)(content + 10) );
        return (unsigned int)(bef & 0x7fffffff); 
    }
    void set_fpid(unsigned int v) {
        unsigned int* bef = (unsigned int*)(content + 10);
        if (get_isWord()) *bef = v | 0x80000000;
        else *bef = v;
    }
}; 
