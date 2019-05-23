VPATH=./
EXECDIR=./bin
OBJDIR=./obj

CC=gcc
CXX=g++
AR=ar
ARFLAGS=rcs
LDFLAGS=
INCLUDES= -I/usr/include/eigen3
OPTS= -O0 -g
CXXFLAGS= -std=c++11 -w
CXXFLAGS+=$(OPTS)
DEPS= Makefile $(wildcard *.hpp)

EXECOBJ=$(patsubst %.cc, %.o, $(wildcard *.cc))
EXECOBJS=$(addprefix $(OBJDIR)/, $(EXECOBJ))
EXEC=$(patsubst %.o, %, $(EXECOBJ))
EXECS=$(addprefix $(EXECDIR)/, $(EXEC))

all: bin obj $(EXECS)

$(EXECDIR)/%: $(OBJDIR)/%.o
	$(CXX) $(INCLUDES) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cc $(DEPS)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

bin:
	mkdir -p $(EXECDIR)

obj:
	mkdir -p $(OBJDIR)

.PHONY: clean

clean:
	rm -rf $(EXECDIR)/* $(OBJDIR)/* $(EXECDIR) $(OBJDIR)

