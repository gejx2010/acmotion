//TrieTree.h  
#pragma once
#include"TrieTreeNode.h"  
#include<iostream>  
#include<fstream>
#include<queue>  
#include<cstring>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<utility>
#include<string>
#include<ctime>
#include<map>

#define COMPILE true

using namespace std;
  
class PosInWord {
public:
    int start;
    int end;
    bool operator==(const PosInWord& piw) {
        return start == piw.start && end == piw.end;
    }
    bool operator=(const PosInWord& piw) {
        start = piw.start;
        end = piw.end;
    }
};

template<class T>  
class TrieTree  
{  
    //Insert时为节点代表的单词word分配内存，Delete时只修改Freq而不删除word，Search时以Freq的数值作为判断依据，而不是根据word是否为NULL  
public:  
    TrieTree();  
    ~TrieTree(){ Destroy(root); Destroy(sim_root); };  
    void Insert(wstring str);//插入单词str  
    //void Insert(const T* str, const int num);//插入单词str，带有编号信息  
    int Search(T* str);//查找单词str，返回出现次数  
    bool Delete(T* str);//删除单词str  
    void PrintALL(bool seq_flag = false);//打印trie树中所有节点对应的单词  
    void PrintPre(T* str);//打印以str为前缀的单词  
    void SetFailedPointer();//设置匹配失效时的跳转指针  
    vector<PosInWord> MatchKMP(wstring str);//返回str中出现在该TrieTree中的单词个数  
    int bin_search(vector<int> v, int num);
    int update_pos_vector(vector<int> &bef_v, int start_pos);
    vector<PosInWord> MatchSequenceKMP(wstring str);
    vector<PosInWord> search(wstring str);//返回str中出现在该TrieTree中的单词个数  
    void Print(TrieTreeNode<T>* p, bool seq_flag = false);  
    void Print(SimTrieTreeNode* p);  
    void Destroy(TrieTreeNode<T>* p);//由析构函数调用，释放以p为根节点的树的空间  
    void Destroy(SimTrieTreeNode* p);
    int value_index(TrieTreeNode<T>* p, T c, bool seq_flag = false);
    int value_index(SimTrieTreeNode* p, T c);
    void write(char* file_name);
    void write_file(TrieTreeNode<T>* p, FILE* in_file);
    void write_struct(char* file_name);
    void copy_struct(SimTrieTreeNode* ap, TrieTreeNode<T>* bp);
    void load(char* file_name, TrieTree<T>* ttn);
    TrieTree<T>* load_file(TrieTreeNode<T>* p, FILE* out_file, TrieTree<T>* ttn, TrieTreeNode<T>** q_array, int& q_rank, long long* id_addr);
    void load_struct(char* file_name);
    int cntWords;
    int cntPoints;
    bool isSeqence();
    SimTrieTreeNode* sim_root;
    unsigned int max_value;
    unsigned int max_wordlen;
    unsigned int max_vid;
    unsigned int max_vsz;
    unsigned int max_fpid;
private:  
    TrieTreeNode<T>* root;  
};  
  
template<class T>  
TrieTree<T>::TrieTree()  
{  
    root = new TrieTreeNode<T>('\0', 0);//根节点不储存字符  
    root->FailedPointer = root;//设置失配指针  
    sim_root = NULL;
    cntWords = 0;
    cntPoints = 0;
    max_value = 0;
    max_wordlen = 0;
    max_vid = 0;
    max_vsz = 0;
    max_fpid = 0;
}  

template<class T>  
void TrieTree<T>::Destroy(TrieTreeNode<T>* p)  
{  
    if (!p)  
        return;  
    for (int i = 0; i < p->ChildNodes.size(); i++)  
        Destroy(p->ChildNodes[i]);  
    p->ChildNodes.clear();
    p->FailedPointer = NULL;
    p = NULL;//节点指针置为空  
    //以上的置NULL的两句无太大意义，但是：编程习惯  
}  
  
template<class T>  
void TrieTree<T>::Destroy(SimTrieTreeNode* p)  
{  
    if (!p)  
        return;  
    if (p->get_vid() == 0) { p = NULL; return; }
    int i = 0;
    while (true) {
        SimTrieTreeNode* sttn = sim_root + p->get_vid() + i;
        if (sttn->get_isEnd()) break;
        Destroy(sttn);
        i++;
    }
    p = NULL;
}  

template<class T>  
bool TrieTree<T>::Delete(T* str)  
{  
    TrieTreeNode<T>* p = root;  
    if (!str)  
        return false;  
    for (int i = 0; str[i]; i++)  
    {  
        int index = value_index(p, str[i]);  
        if (0 <= index)  
            p = p->ChildNodes[index];  
        else return false;  
    }  
    p->Freq = 0;  
    p->ID = -1;  
    return true;  
}  
  
template<class T>  
void TrieTree<T>::PrintPre(T* str)  
{  
    TrieTreeNode<T>* p = root;  
    if (!str)  
        return;  
    for (int i = 0; str[i]; i++)  
    {  
        int index = value_index(p, str[i]);  
        if (0 <= index)  
            p = p->ChildNodes[index];  
        else return;  
    }  
    cout << "以" << str << "为前缀的单词有:" << endl;  
    Print(p);  
}  
  
template<class T>  
int TrieTree<T>::Search(T* str)  
{  
    TrieTreeNode<T>* p = root;  
    if (!str)  
        return -1;  
    for (int i = 0; str[i]; i++)  
    {  
        int index = value_index(p, str[i]);  
        if (0 <= index)  
            p = p->ChildNodes[index];  
        else return 0;  
    }  
    return p->Freq;  
}  
  
  
template<class T>
int TrieTree<T>::value_index(TrieTreeNode<T>* p, T c, bool seq_flag) {
   int cs = seq_flag ? p->vsz : p->ChildNodes.size();
   for (int i = 0; i < cs; i++){
       //printf("@value_index, %d, %d\n", int(p->ChildNodes[i]->value), int(c));
       TrieTreeNode<T>* cur_node = seq_flag ? (root + p->vid + i) : p->ChildNodes[i];
       if (int(cur_node->value) == int(c))
           return i;
   }  
   return -1;
}

template<class T>
int TrieTree<T>::value_index(SimTrieTreeNode* p, T c) {
   if (p->get_vid() == 0) return -1;
   int i = 0;
   while (true) {
       SimTrieTreeNode* cur_node = sim_root + p->get_vid() + i;
       if (int(cur_node->get_value()) == int(c))
           return i;
       i++;
       if (cur_node->get_isEnd()) return -1;
   }
}

template<class T>  
void TrieTree<T>::Insert(wstring str)  
{  
    TrieTreeNode<T>* p = root;  
    int i;  
    for (i = 0; str[i]; i++)  
    {  
        int index = value_index(p, str[i]);
        if (index < 0) { 
            cntPoints++;
            p->ChildNodes.push_back(new TrieTreeNode<T>(str[i], cntPoints));  
            p = p->ChildNodes.back();
        } else
            p = p->ChildNodes[index];  
    }  
    if (!p->word)//该词以前没有出现过  
    {  
        p->wordlen = i;//设置单词长度  
        p->word = true;
    }  
    p->Freq++;  
    cntWords++;
}  
  
//template<class T>  
//void TrieTree<T>::Insert(const T* str, const int num)  
//{  
//    TrieTreeNode<T>* p = root;  
//    int i;  
//    for (i = 0; str[i]; i++)  
//    {  
//        int index = value_index(p, str[i]);
//        if (index < 0) { 
//            p->ChildNodes.push_back(new TrieTreeNode<T>(str[i]));  
//            p = p->ChildNodes.back();
//        } else
//            p = p->ChildNodes[index];  
//    }  
//    if (!p->word)//该词以前没有出现过  
//    {  
//        p->word = new wchar_t[wcslen(str) + 1];  
//        wcscpy(p->word, str);  
//        p->wordlen = i;  
//    }  
//    p->Freq++;  
//    cntWords++;
//    if (num < p->ID || p->ID == -1)//取最小的num作为当前节点代表的单词的ID  
//        p->ID = num;  
//}  
  
template<class T>  
void TrieTree<T>::PrintALL(bool seq_flag)  
{  
    if (sim_root == NULL)
      Print(root, seq_flag);  
    else
      Print(sim_root);
}  
  
template<class T>  
void TrieTree<T>::Print(TrieTreeNode<T>* p, bool seq_flag)  
{  
    if (p == NULL)  
        return;  
    int cur_fpid = seq_flag ? p->fpid : p->FailedPointer->ID;
    printf("value:%d word:%d wordlen:%d vid:%d vsz:%d fpid:%d\n", p->value, p->word, p->wordlen, p->vid, p->vsz, cur_fpid);
    int sz = seq_flag ? p->vsz : p->ChildNodes.size();
    for (int i = 0; i < sz; i++)  
    {  
        TrieTreeNode<T>* cur_p = seq_flag ? (root + p->vid + i) : p->ChildNodes[i];
        if (cur_p)  
        {  
            Print(cur_p, seq_flag);  
        }  
    }  
}  
  
template<class T>  
void TrieTree<T>::Print(SimTrieTreeNode* p)  
{  
    if (p == NULL)  
        return;  
    max_value = (max_value < p->get_value()) ? p->get_value() : max_value;
    //max_wordlen = (max_wordlen < p->get_wordlen()) ? p->get_wordlen() : max_wordlen;
    max_vid = (max_vid < p->get_vid()) ? p->get_vid() : max_vid;
    //max_vsz = (max_vsz < p->get_vsz()) ? p->get_vsz() : max_vsz;
    max_fpid = (max_fpid < p->get_fpid()) ? p->get_fpid() : max_fpid;
    //printf("value:%d wordlen:%d vid:%d vsz:%d fpid:%d\n", p->value, p->wordlen, p->vid, p->vsz, p->fpid);
    if (p->get_vid() == 0) return;
    int i = 0;
    while (true) {
        SimTrieTreeNode* cur_p = sim_root + p->get_vid() + i;
        if (cur_p) Print(cur_p);
        if (cur_p->get_isEnd()) break;
        i++;
    } 
}  

template<class T>  
vector<PosInWord> TrieTree<T>::MatchKMP(wstring str)  
{  
    vector<PosInWord> seq_list;
    PosInWord* piw;
    seq_list.clear();
    int count = 0;//str中出现的TrieTree中的单词个数  
    wstring p(str);//str中指针  
    TrieTreeNode<T>* node = root;//TrieTree的节点指针  
    int rank = 0;
    while (p[rank])  
    {  
        int index = value_index(node, p[rank]);
        if (0 <= index)//当前字符匹配成功  
        {  
            TrieTreeNode<T>* temp = node->ChildNodes[index]->FailedPointer;  
            rank++;
            while (temp != root)//在匹配的情况下，仍然沿FailedPointer搜索，可检索出所有模式。  
            {  
                if (temp->Freq > 0)  
                {  
                    count++;  
                    piw = new PosInWord();
                    piw->start = rank - temp->wordlen;
                    piw->end = rank;
                    seq_list.push_back(*piw);
                }  
                temp = temp->FailedPointer;  
            }  
            node = node->ChildNodes[index];  
            if (node->Freq > 0)  
            {  
                count++;  
                piw = new PosInWord();
                piw->start = rank - node->wordlen;
                piw->end = rank;
                seq_list.push_back(*piw);
            }  
        }  
        else//失配，跳转  
        {  
            if (node == root)  
                rank++;
            else  
                node = node->FailedPointer;  
        }  
    }  
    // 去重
    vector<PosInWord>::iterator vit = unique(seq_list.begin(), seq_list.end());
    seq_list.resize(distance(seq_list.begin(), vit));
    return seq_list;  
}  
  
template<class T>
int TrieTree<T>::bin_search(vector<int> v, int num) {
  int sz = v.size();
  int start = 0, end = v.size();
  while (start < end) {
      int med = (start + end) / 2;
      if ((med < sz - 1 && (v[med] <= num && num < v[med + 1])) || (med == sz - 1 && v[med] <= num)) {
          return med + 1;
      } else if (med == 0 && num < v[med]) {
          return 0;
      } else if (v[med] <= num) {
          start = med;
      } else {
          end = med;
      }
  }
  return -1;
}

template<class T>
int TrieTree<T>::update_pos_vector(vector<int> &path_id, int start_pos) {
    if (path_id.size() <= 0) return start_pos;
    SimTrieTreeNode* node = sim_root + path_id[path_id.size() - 1];
    //找到node->get_fpid对应的层数
    if (0 < node->get_fpid()) { // 否则为根节点
        // node->get_fpid节点深度有depth和depth+1两种可能，先假设depth比较深，即+1
        int depth = bin_search(path_id, node->get_fpid()) + 1;
        int ldep = path_id.size() - depth;
        SimTrieTreeNode* sttn;
        // 如果depth直接最大，因为节点深度不可能超过size，所以不用试depth+1
        if (path_id.size() <= depth) {
            depth = path_id.size() - 1, ldep = 1;
        } else if (0 < depth) { // 用depth+1尝试，如果遍历所有节点且最后一个节点有单词标志，则取depth+1，若失败，则取depth
            int cur_rank = ldep;
            sttn = sim_root + path_id[cur_rank];
            SimTrieTreeNode* cur_ttn = sim_root;
            while (true) {
                int ind = value_index(cur_ttn, sttn->get_value());
                if (ind < 0) {
                    depth--, ldep++; 
                    break;
                }
                cur_rank++;
                cur_ttn = sim_root + cur_ttn->get_vid() + ind;
                if (path_id.size() <= cur_rank) {
                    if (cur_ttn != sim_root + node->get_fpid()) {
                        depth--, ldep++;
                    }
                    break;
                }
                sttn = sim_root + path_id[cur_rank];
            }
        }
        start_pos += ldep;
        vector<int> temp_vec;
        int temp_rank = ldep;
        SimTrieTreeNode* t_sttn = sim_root;
        while (temp_rank < path_id.size()) {
            sttn = sim_root + path_id[temp_rank];
            int v_ind = value_index(t_sttn, sttn->get_value());
            temp_vec.push_back(t_sttn->get_vid() + v_ind);
            t_sttn = sim_root + t_sttn->get_vid() + v_ind;
            temp_rank++;
        }
        path_id.clear();
        path_id = temp_vec;
    } else {
        start_pos += path_id.size();
        path_id.clear();
    }
    return start_pos;
}

template<class T>  
vector<PosInWord> TrieTree<T>::MatchSequenceKMP(wstring str)  
{  
    vector<PosInWord> seq_list;
    PosInWord* piw;
    seq_list.clear();
    int count = 0;//str中出现的TrieTree中的单词个数  
    wstring p(str);//str中指针  
    SimTrieTreeNode* node = sim_root;//TrieTree的节点指针  
    int start_pos = 0; //记录初始位置
    int rank = 0;
    vector<int> path_id;
    while (p[rank])  
    {  
        int index = value_index(node, p[rank]);
        if (0 <= index)//当前字符匹配成功  
        {  
            vector<int> temp_pos_vec(path_id);
            temp_pos_vec.push_back(node->get_vid() + index);
            SimTrieTreeNode* temp_bef = sim_root + node->get_vid() + index;
            SimTrieTreeNode* temp = sim_root + temp_bef->get_fpid();  
            int temp_pos = update_pos_vector(temp_pos_vec, start_pos);
            rank++;
            while (temp != sim_root)//在匹配的情况下，仍然沿FailedPointer搜索，可检索出所有模式。  
            {  
                if (temp->get_isWord())  
                {  
                    count++;  
                    piw = new PosInWord();
                    piw->start = temp_pos;
                    piw->end = rank;
                    seq_list.push_back(*piw);
                }  
                temp = sim_root + temp->get_fpid();  
                temp_pos = update_pos_vector(temp_pos_vec, temp_pos);
            }  
            path_id.push_back(node->get_vid() + index);
            node = sim_root + node->get_vid() + index;  
            if (node->get_isWord())  
            {  
                count++;  
                piw = new PosInWord();
                piw->start = start_pos;
                piw->end = rank;
                seq_list.push_back(*piw);
            }  
        }  
        else//失配，跳转  
        {  
            if (node == sim_root) {
                rank++;
                start_pos = rank;
            } else { 
                start_pos = update_pos_vector(path_id, start_pos);
                node = sim_root + node->get_fpid();  
            }
        }  
    }  
    // 去重
    vector<PosInWord>::iterator vit = unique(seq_list.begin(), seq_list.end());
    seq_list.resize(distance(seq_list.begin(), vit));
    return seq_list;  
}  

template<class T>  
vector<PosInWord> TrieTree<T>::search(wstring str)  
{  
    if (sim_root == NULL)
        return MatchKMP(str);
    return MatchSequenceKMP(str); 
}  

template<class T>  
void TrieTree<T>::SetFailedPointer()  
{  
    queue<TrieTreeNode<T>*> q;  
    q.push(root);  
    while (!q.empty())  
    {  
        TrieTreeNode<T>* father = q.front();//父节点  
        q.pop();  
        for (int i = 0; i < father->ChildNodes.size(); i++) //对每一个子节点设置FailedPointer  
        {  
            if (father->ChildNodes[i])  
            {  
                TrieTreeNode<T>* child = father->ChildNodes[i];  
                q.push(child);  
                TrieTreeNode<T>* candidate = father->FailedPointer;//从father->FailedPointer开始游走的指针  
                while (true)  
                {  
                    if (father == root)  
                    {  
                        candidate = root;  
                       break;  
                    }  
                    int can_index = value_index(candidate, child->value);
                    //printf("@setFailed, can_index: %d\n", can_index);
                    if (0 <= can_index)//有与child代表的字母相同的子节点  
                    {  
                        candidate = candidate->ChildNodes[can_index];  
                        break;  
                    }  
                    else  
                    {  
                        if (candidate == root)  
                            break;  
                        candidate = candidate->FailedPointer;//以上两句顺序不能交换，因为在root仍可以做一次匹配  
                    }  
                }  
                child->FailedPointer = candidate;  
            }  
        }  
    }  
} 

template <class T>
void TrieTree<T>::write(char* file_name) {
    FILE* infile = fopen(file_name, "w"); 
    fprintf(infile, "%d %d\n", cntPoints, cntWords);
    queue<TrieTreeNode<T>*> q;
    q.push(root);
    while (!q.empty()) {
        TrieTreeNode<T>* qf = q.front();
        write_file(qf, infile);
        q.pop();
        for (int i = 0; i < qf->ChildNodes.size(); i++) {
            q.push(qf->ChildNodes[i]);
        }
    }
    fclose(infile);
}

template <class T>
void TrieTree<T>::write_file(TrieTreeNode<T>* p, FILE* infile) {
    fprintf(infile, "%d %d %d %d %d %d %d\n", p->ID, p->value, p->wordlen, p->FailedPointer->ID, p->Freq, p->ChildNodes.size(), p->word);
}

template <class T>
void TrieTree<T>::load(char* file_name, TrieTree<T>* ttn) {
    FILE* out_file = fopen(file_name, "r");
    fscanf(out_file, "%d %d\n", &ttn->cntPoints, &ttn->cntWords);
    TrieTreeNode<T>* qf;
    TrieTreeNode<T>** q_array = new TrieTreeNode<T>*[cntPoints];
    for (int i = 0; i < cntPoints; i++)
        q_array[i] = new TrieTreeNode<T>();
    long long* id_addr = new long long[cntPoints + 1];
    //for (int i = 0; i < cntPoints; i++)
        //q_array[i] = new TrieTreeNode<T>;
    int q_rank = 0;
    queue<TrieTreeNode<T>*> q;
    q.push(ttn->root);
    //printf("@load, cntPoints: %d, cntWord: %d\n", ttn->cntPoints, ttn->cntWords);
    while (!feof(out_file) && !q.empty()) {
        qf = q.front();
        //printf("@load, before load file, q_rank: %d\n", q_rank);
        load_file(qf, out_file, ttn, q_array, q_rank, id_addr);
        //printf("@load, after load file, q_rank: %d\n", q_rank);
        q.pop();
        for (int i = 0; i < qf->ChildNodes.size(); i++) {
            q.push(qf->ChildNodes[i]);
        }
    }
    fclose(out_file);
    delete[] id_addr;
    return;
}

template <class T>
TrieTree<T>* TrieTree<T>::load_file(TrieTreeNode<T>* p, FILE* out_file, TrieTree<T>* ttn, TrieTreeNode<T>** q_array, int& q_rank, long long* id_addr) {
    int MAX_WORD_SIZE = 1000;
    int child_size = 0;
    int id = 0;
    int word = 0;
    int wlen = 0;
    int fpid = 0;
    int freq = 0; 
    int value = 0;
    fscanf(out_file, "%d %d %d %d %d %d %d\n", &id, &value, &wlen, &fpid, &freq, &child_size, &word);
    p->word = word;
    p->ID = id;
    p->value = value;
    p->wordlen = wlen;
    p->Freq = freq;
    id_addr[id] = (long long)p;
    TrieTreeNode<T>* tp = (TrieTreeNode<T>*)id_addr[fpid];
    if (tp == NULL) {
        tp = ttn->root;
    }
    p->FailedPointer = tp;
    //printf("id:%d value:%d wordlen:%d fpid:%d freq:%d childsize:%d word:%d\n", p->ID, p->value, p->wordlen, fpid, p->Freq, child_size, p->word);
    p->ChildNodes.insert(p->ChildNodes.begin(), q_array + q_rank, q_array + q_rank + child_size);
    q_rank += child_size;
}

template <class T>
void TrieTree<T>::write_struct(char* file_name) {
    FILE* infile = fopen(file_name, "wb"); 
    SimTrieTreeNode* all_points = new SimTrieTreeNode[cntPoints + 1];
    long long* id_addr = new long long[cntPoints + 1];
    int cur_rank = 0;
    int ap_rank = 1;
    queue<TrieTreeNode<T>*> q;
    q.push(root);
    while (!q.empty()) {
        TrieTreeNode<T>* qf = q.front();
        copy_struct(all_points + cur_rank, qf);
        SimTrieTreeNode* cur_point = all_points + cur_rank;
        if (0 < qf->ChildNodes.size())
            cur_point->set_vid(ap_rank);
        //cur_point->set_vsz(qf->ChildNodes.size());
        id_addr[qf->ID] = (long long)cur_point;
        cur_point->set_fpid( (id_addr[qf->FailedPointer->ID] - (long long)all_points) / sizeof(SimTrieTreeNode) );
        q.pop();
        //printf("vid:%d vsz:%d cur_rank:%d ap_rank:%d fpid: %d\n", cur_point->vid, cur_point->vsz, cur_rank, ap_rank, cur_point->fpid);
        cur_rank++;
        for (int i = 0; i < qf->ChildNodes.size(); i++) {
            if (i == qf->ChildNodes.size() - 1)
                qf->ChildNodes[i]->isEnd = true;
            q.push(qf->ChildNodes[i]);
            ap_rank++;
        }
    }
    delete[] id_addr;
    char* root_chars = (char *)all_points;
    fwrite(root_chars, sizeof(SimTrieTreeNode), cntPoints + 1, infile);
    fclose(infile);
}

template <class T>
void TrieTree<T>::copy_struct(SimTrieTreeNode* ap, TrieTreeNode<T>* bp) {
    ap->set_value( bp->value );
    //ap->set_wordlen( bp->wordlen );
    ap->set_isWord( bp->wordlen );
    ap->set_isEnd( bp->isEnd );
}

template <class T>
void TrieTree<T>::load_struct(char* file_name) {
    ifstream out_file(file_name, std::ifstream::binary);
    out_file.seekg(0, out_file.end);
    long long size = out_file.tellg();
    out_file.seekg(0);
    char* out_struct = new char[size];
    cntPoints = size / sizeof(SimTrieTreeNode);
    out_file.read(out_struct, size);
    sim_root = (SimTrieTreeNode*) out_struct;
    out_file.close();
}

// endef TRIETREE_H
