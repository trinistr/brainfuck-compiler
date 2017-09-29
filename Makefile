MAIN = bfcompile
SRC = ./src

all: $(MAIN)

$(MAIN):
	$(MAKE) -C $(SRC)
	mv $(SRC)/bfcompile $(MAIN)

.PHONY: clean

clean:
	$(RM) $(MAIN)
	$(MAKE) -C $(SRC) clean
