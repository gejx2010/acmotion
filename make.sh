#g++ -shared -o acmotion.so -fPIC wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/local/include -I /usr/local/include/python3.5m/ -lunicode-encoding -lencoding -lboost_regex -lboost_python35 -lboost_date_time
g++ -shared -o acmotion.so -fPIC wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/local/include -I /usr/local/include/python3.5m/ -I /usr/include -L /usr/lib -lboost_python35
#g++  -g -o acmotion wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/local/include -I /usr/local/include/python3.5m/ -lunicode-encoding -lencoding -lboost_regex -lboost_python35 -lboost_date_time -lpython3.5m
g++  -g -o acmotion wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/local/include -I /usr/local/include/python3.5m/ -ldl -lutil -lboost_python35 -lpython3.5m
