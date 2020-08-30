CXXFLAGS = -Os -std=c++2a -fno-exceptions -fno-rtti

sml: main.cpp
	$(CXX) $(CXXFLAGS) -I. -o sml main.cpp 

.PHONY: clean
clean:
	rm -rf sml