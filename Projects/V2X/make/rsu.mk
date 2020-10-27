#############################################################################
# RSU Make File

V2X_DIR 	= $(V2X)

JCCL_DIR 	= $(WORKDIR)/JCCL

ROOT_DIR 	= $(V2X_DIR)
BIN 		= $(ROOT_DIR)/bin

SRC_DIR 	= $(ROOT_DIR)/src

V2X_Common	= $(SRC_DIR)/Common
RSU_SRC 	= $(SRC_DIR)/RSU

OBJ_DIR 	= $(ROOT_DIR)/obj/RSU

GHDMA3A 	= $(SRC_DIR)/GHDMA3A

EXE_FILE 	= RSU

CFLAGS 		= -g -fpermissive -I$(RSU_SRC) -I$(V2X_Common) -I$(JCCL_DIR)/include -I$(GHDMA3A) -D_SRM_RSU

# CC = /usr/local/gcc-4.9-2016.02-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-c++
CC = g++
cc = gcc

EXE = $(BIN)/$(EXE_FILE)

ModuleObject = $(OBJ_DIR)/main.o $(OBJ_DIR)/RSUModule.o $(OBJ_DIR)/GHDMA3ADriver.o $(OBJ_DIR)/V2XPrimitive.o \
				$(OBJ_DIR)/GNSS.o $(OBJ_DIR)/CanDriver.o $(OBJ_DIR)/WiFiDriver.o

include $(ROOT_DIR)/make/common.mk

$(OBJ_DIR)/main.o: $(RSU_SRC)/main.cpp
	$(CC) -o $@ -c $(CFLAGS) $(RSU_SRC)/main.cpp 

$(OBJ_DIR)/RSUModule.o: $(RSU_SRC)/RSUModule.cpp
	$(CC) -o $@ -c $(CFLAGS) $(RSU_SRC)/RSUModule.cpp 

$(OBJ_DIR)/GHDMA3ADriver.o: $(RSU_SRC)/GHDMA3ADriver.cpp
	$(CC) -o $@ -c $(CFLAGS) $(RSU_SRC)/GHDMA3ADriver.cpp 

$(OBJ_DIR)/V2XPrimitive.o: $(GHDMA3A)/V2XPrimitive.cpp
	$(CC) -o $@ -c $(CFLAGS) $(GHDMA3A)/V2XPrimitive.cpp

$(OBJ_DIR)/GNSS.o: $(GHDMA3A)/GNSS.cpp
	$(CC) -o $@ -c $(CFLAGS) $(GHDMA3A)/GNSS.cpp


$(OBJ_DIR)/CanDriver.o: $(GHDMA3A)/CanDriver.cpp
	$(CC) -o $@ -c $(CFLAGS) $(GHDMA3A)/CanDriver.cpp

$(OBJ_DIR)/WiFiDriver.o: $(GHDMA3A)/WiFiDriver.cpp
	$(CC) -o $@ -c $(CFLAGS) $(GHDMA3A)/WiFiDriver.cpp

clean:
	rm -f $(OBJ_DIR)/*.o 
	rm -f $(EXE)

