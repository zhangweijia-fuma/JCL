#############################################################################
# RSU Make File

V2X_DIR 	=$(WORKDIR)/Projects/V2X

JCCL_DIR 	=$(WORKDIR)/JCCL

ROOT_DIR 	= $(V2X_DIR)
BIN 		= $(ROOT_DIR)/bin

SRC_DIR 	= $(ROOT_DIR)/src

V2X_Common	= $(SRC_DIR)/Common
ES_SRC 		= $(SRC_DIR)/EdgeServer

OBJ_DIR 	= $(ROOT_DIR)/obj/EdgeServer


EXE_FILE 	= EdgeServer

CFLAGS 		= -g -I$(ES_SRC) -I$(V2X_Common) -I$(JCCL_DIR)/include -D_SRM_EdgeServer

CC = g++

EXE = $(BIN)/$(EXE_FILE)

ModuleObject = $(OBJ_DIR)/main.o $(OBJ_DIR)/EdgeServer.o 

include $(ROOT_DIR)/make/common.mk

$(OBJ_DIR)/main.o: $(ES_SRC)/main.cpp
	$(CC) -o $@ -c $(CFLAGS) $(ES_SRC)/main.cpp 

$(OBJ_DIR)/EdgeServer.o: $(ES_SRC)/EdgeServer.cpp
	$(CC) -o $@ -c $(CFLAGS) $(ES_SRC)/EdgeServer.cpp 

clean:
	rm -f $(OBJ_DIR)/*.o 
	rm -f $(EXE)

