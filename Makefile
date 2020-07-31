PRODUCT = neural_network
SRCDIR = src
OBJDIR = obj
INCDIR = include
MODELDIR = saved_models

CXX = g++
FLAGS = -std=c++11 -g -Wall -I$(INCDIR)/

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCFILES)))))
INCFILES = $(wildcard $(INCDIR)/*.h)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCFILES) $(OBJDIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(PRODUCT): $(OBJFILES)
	$(CXX) $(FLAGS) $^ -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm $(OBJFILES)
	rmdir $(OBJDIR)
	rm $(PRODUCT)
	rm $(MODELDIR)/*.txt
