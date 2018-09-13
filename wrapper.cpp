#include "TrieTree.h"
#include "TrieTreeNode.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace boost::python;
using namespace std;

BOOST_PYTHON_MODULE(acmotion) {
    class_< TrieTreeNode<wchar_t> >("ACNode", init<wchar_t, int>())
        .def_readwrite("word", &TrieTreeNode<wchar_t>::word)
        .def_readwrite("wordlen", &TrieTreeNode<wchar_t>::wordlen)
        .def_readwrite("childNodes", &TrieTreeNode<wchar_t>::ChildNodes)
        .def_readwrite("freq", &TrieTreeNode<wchar_t>::Freq)
        .def_readwrite("id", &TrieTreeNode<wchar_t>::ID)
        .def_readwrite("failedPointer", &TrieTreeNode<wchar_t>::FailedPointer);
    class_< TrieTree<wchar_t> >("ACMotion")
        .def("insert", &TrieTree<wchar_t>::Insert)
        .def("setFailedPointer", &TrieTree<wchar_t>::SetFailedPointer)
        .def("matchKMP", &TrieTree<wchar_t>::MatchKMP) 
        .def("write", &TrieTree<wchar_t>::write)
        .def("load", &TrieTree<wchar_t>::load)
        .def("write_struct", &TrieTree<wchar_t>::write_struct)
        .def("load_struct", &TrieTree<wchar_t>::load_struct)
        .def("search", &TrieTree<wchar_t>::search);
    class_<PosInWord>("PosInWord")
        .def_readwrite("start", &PosInWord::start)
        .def_readwrite("end", &PosInWord::end);
    class_< vector<PosInWord> >("PosInWords")
        .def(vector_indexing_suite< vector<PosInWord> >());
}

int main(int argc, char** argv) {
	return 0;
}
