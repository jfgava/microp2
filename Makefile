MAKE=make
TARGET_LIB=libp2.a
SRC_DIR=src
LIB_DIR=lib

all: lib
	@$(MAKE) -C $(SRC_DIR)

.PHONY: lib
lib: 
	@$(MAKE) -C $(LIB_DIR)

clean:
	@$(MAKE) clean -C $(SRC_DIR)
	@$(MAKE) clean -C $(LIB_DIR)

distclean:
	@$(MAKE) distclean -C $(SRC_DIR)
	@$(MAKE) distclean -C $(LIB_DIR)
