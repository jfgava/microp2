MAKE=make
SRC_DIR=src

all:
	$(MAKE) -C $(SRC_DIR)

clean:
	$(MAKE) clean -C $(SRC_DIR)

distclean:
	$(MAKE) distclean -C $(SRC_DIR)
