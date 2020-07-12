mle_cxxflags:=-g -O2 -std=c++17 -Wall -Wno-missing-braces -Wno-unknown-pragmas -Wno-unused-function -Wno-pragmas -Wno-reorder -I. -I../vendor/tvision/include -I../vendor/tvision/include/override $(CXXFLAGS)
mle_tuiobjects:=$(patsubst %.cc,%.o,$(wildcard *.cc))

all: $(mle_tuiobjects)

$(mle_tuiobjects): %.o: %.cc
	$(CXX) -c $(mle_cxxflags) $< -o $@
