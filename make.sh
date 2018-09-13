g++ -shared -o acmotion.so -fPIC wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/local/include -I /usr/local/include/python3.6m/ -lunicode-encoding -lencoding -lboost_regex -lboost_python3 -lboost_date_time
g++  -g -o acmotion wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/local/include -I /usr/local/include/python3.6m/ -lunicode-encoding -lencoding -lboost_regex -lboost_python3 -lboost_date_time -lpython3.6m

