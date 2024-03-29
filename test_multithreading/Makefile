
SOURCEDIR  = src
INCLUDEDIR = include
TMPDIR     = tmp

CXX        = g++
CXXFLAGS   = -O2 -std=c++11 -I$(INCLUDEDIR) -Wall -Ibenchmark/include
#CXXFLAGS  = -pg -std=c++11 -I$(INCLUDEDIR) -Wall
LIBS       = -pthread -Lbenchmark/build/src -lbenchmark

VPATH      = $(INCLUDEDIR):$(SOURCEDIR):$(TMPDIR)

target     = fastnet
sources    = $(wildcard src/*.cpp)
objects    = $(notdir $(patsubst %.cpp, %.o, $(target).cpp $(sources)))

$(target): $(objects) 
	@echo "Linking..."
	@$(CXX) $(CXXFLAGS) -o $(target) $(patsubst %.o,$(TMPDIR)/%.o,$(objects)) $(LIBS)

-include $(patsubst %.d,$(TMPDIR)/%.d,$(objects:.o=.d))
	
%.o: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -o $(TMPDIR)/$@ -c  $<

$(TMPDIR)/%.d: %.cpp
	@echo "Making dependencies for $<..."
	@$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

.PHONY: all clean 

all: $(target) 

clean:
	@echo "Cleaning up..."
	@/bin/rm -f $(TMPDIR)/*.o $(TMPDIR)/*.d $(target)

