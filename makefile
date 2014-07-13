

all:
	cd study-3 && $(MAKE) clean
	cd study-3 && $(MAKE) test

lib:
	cd study-3 && $(MAKE)	

test:
	cd study-3 && $(MAKE) clean
	cd study-3 && $(MAKE) test

clean:
	cd study-3 && $(MAKE) clean

