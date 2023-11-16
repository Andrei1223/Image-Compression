NUME:="main"
NUMEFIS:="quadtree"

build:
	touch $(NUMEFIS).out
	gcc -o $(NUMEFIS) -Wall $(NUME).c -lm

clean:
	rm $(NUMEFIS)