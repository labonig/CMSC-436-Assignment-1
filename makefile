CXX = g++
CXXFLAGS = -Wall

format: format.cpp
	$(CXX) $(CXXFLAGS) format.cpp -o format

run:
	./format
