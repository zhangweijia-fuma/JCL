#############################################################################
# RSU Make File

V2X_DIR 	=$(WORKDIR)/Projects/V2X

JCCL_DIR 	=$(WORKDIR)/JCCL

ROOT_DIR 	= $(V2X_DIR)
BIN 		= $(ROOT_DIR)/bin

SRC_DIR 	= $(ROOT_DIR)/src

V2X_Common	= $(SRC_DIR)/Common
RM_SRC 		= $(SRC_DIR)/RSUManager

OBJ_DIR 	= $(ROOT_DIR)/obj/RSUManager

EXE_FILE 	= RSUManager

CFLAGS 		= -g -I$(RM_SRC) -I$(V2X_Common) -I$(JCCL_DIR)/include -D_SRM_RSUManager

# CC = g++
CC = /usr/local/gcc-4.9-2016.02-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-c++

EXE = $(BIN)/$(EXE_FILE)

ModuleObject = $(OBJ_DIR)/main.o $(OBJ_DIR)/RSUManager.o 

include $(ROOT_DIR)/make/common.mk

$(OBJ_DIR)/main.o: $(RM_SRC)/main.cpp
	$(CC) -o $@ -c $(CFLAGS) $(RM_SRC)/main.cpp 

$(OBJ_DIR)/RSUManager.o: $(RM_SRC)/RSUManager.cpp
	$(CC) -o $@ -c $(CFLAGS) $(RM_SRC)/RSUManager.cpp 

clean:
	rm -f $(OBJ_DIR)/*.o 
	rm -f $(EXE)

