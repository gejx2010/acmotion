g++ -shared -o acmotion.so -fPIC wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/include -I /usr/include/python2.7/ -L/usr/lib -L/usr/lib/python2.7 -L/usr/local/lib -lboost_python27 -lpython2.7
g++  -g -o acmotion wrapper.cpp TrieTree.h TrieTreeNode.h -I /usr/include -I /usr/include/python2.7/ -L/usr/lib -L/usr/lib/python2.7 -L/usr/local/lib -lboost_python27 -lpython2.7 

