
ROOT := ../../..

CXXFLAGS := -I$(ROOT)/src/engine/include -I../src
LINKFLAGS := -F$(ROOT)/build/lib -framework GUIDOEngine

all: main.cpp
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) main.cpp -o guidofontmetrics
	