obj=acmotion.so acmotion
all: $(obj)
.PHONY: all
CC=g++
CINCLUDES=-I /usr/local/include -I /usr/local/include/python3.5m/ -I /usr/include
CLIBS=-L /usr/lib -lboost_python35
CLIBS_2=$(CLIBS) -ldl -lutil -lpython3.5m
FILES=wrapper.cpp TrieTree.h TrieTreeNode.h

acmotion.so: $(FILES)
	$(CC) -shared -o acmotion.so -fPIC $(FILES) $(CINCLUDES) $(CLIBS)

acmotion: $(FILES)
	$(CC) -g -o acmotion -FPIC $(FILES) $(CINCLUDES) $(CLIBS_2)

.PHONY: clean
clean:
	-$(RM) $(obj)
