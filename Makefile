MAIN = bfcompile
SRC = ./src
TESTS = ./tests

all: $(MAIN)

$(MAIN):
	$(MAKE) -C $(SRC)
	mv $(SRC)/bfcompile $(MAIN)

.PHONY: clean

clean:
	$(RM) $(MAIN)
	$(MAKE) -C $(SRC) clean
	$(MAKE) -C $(TESTS) clean
