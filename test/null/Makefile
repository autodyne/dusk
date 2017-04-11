name = null
lang = -std=c++11 -O3
opts = -ldusk -lnuma -lpthread
main = $(name).main
plot = $(name).plot
bins = $(main) $(plot)

build: $(bins)

clean:
	rm $(bins)

$(main): $(main).cpp
	$(CXX) $(lang) -o $@ $^ $(opts)

$(plot): $(plot).cpp
	$(CXX) $(lang) -o $@ $^ $(opts)
