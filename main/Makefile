primary = dprop dwrap dtask deque
outline = dcore dnode dnuma dehcs
tactics = dfifo dhief
coupler = dbase dutil
module1 = $(primary) $(outline)
module2 = $(tactics) $(coupler)
object1 = $(addprefix .,$(module1))
object2 = $(addprefix .,$(module2))
objects = $(object1) $(object2)
depends = -lnuma -lpthread
cppopts = -std=c++11 -fpic -O3
options = $(depends) $(cppopts)
product = ../libdusk.so

$(product): $(objects)
	$(CXX) -shared $(options) -o $@ $^

clean:
	rm -f $(objects)

.%: internal/%.cpp
	$(CXX) -c $(options) -o $@ $^
.%: external/%.cpp
	$(CXX) -c $(options) -o $@ $^
