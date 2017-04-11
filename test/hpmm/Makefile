name = hpmm
lang = -std=c++11 -O3
opts = -ldusk -lnuma -lpthread
main = $(name).main
grid = $(name).grid
tree = $(name).tree
plot = $(name).plot
bins = $(grid) $(tree) $(plot)

build: $(bins)

clean:
	rm $(bins) .*.csv

$(grid): $(grid).cpp
	$(CXX) $(lang) -o $@ $^ $(opts)

$(tree): $(tree).cpp
	$(CXX) $(lang) -o $@ $^ $(opts)

$(plot): $(plot).cpp
	$(CXX) $(lang) -o $@ $^ $(opts)
