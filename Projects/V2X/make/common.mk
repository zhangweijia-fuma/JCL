#############################################################################
# Common Make File

JCCL_OBJECTS = $(OBJ_DIR)/CList.o $(OBJ_DIR)/Common.o $(OBJ_DIR)/Config.o $(OBJ_DIR)/CString.o \
		$(OBJ_DIR)/Debug.o  $(OBJ_DIR)/File.o $(OBJ_DIR)/FTPClient.o $(OBJ_DIR)/SystemBaseInfo.o \
		$(OBJ_DIR)/ModuleList.o $(OBJ_DIR)/ModuleBase.o $(OBJ_DIR)/Codec.o \
		$(OBJ_DIR)/MsgBase.o $(OBJ_DIR)/MsgServerBase.o ${OBJ_DIR}/MsgClientBase.o $(OBJ_DIR)/MsgSocket.o  \
		$(OBJ_DIR)/Timer.o $(OBJ_DIR)/ASN1PERCoding.o $(OBJ_DIR)/Time.o  $(OBJ_DIR)/FileDataEx.o \
		$(OBJ_DIR)/DownloadFile.o

JCCL_SRC_DIR = $(JCCL_DIR)/src


MODULE_COMMON_OBJECTS = $(OBJ_DIR)/RSUMsgBase.o $(OBJ_DIR)/CSAEMsgBase.o $(OBJ_DIR)/CSAEDataFrame.o \
		$(OBJ_DIR)/CSAEMessageSet.o $(OBJ_DIR)/CSAETest.o $(OBJ_DIR)/BaiduMessage.o $(OBJ_DIR)/RSUMessage.o \

OBJECTS = $(JCCL_OBJECTS) $(MODULE_COMMON_OBJECTS) $(ModuleObject)


$(EXE):$(OBJECTS)
	rm -f $@
	$(CC) -o $(EXE) $(OBJECTS) -lpthread

$(OBJ_DIR)/CList.o: $(JCCL_SRC_DIR)/CList.cpp
	$(CC) -o $@ -c $(CFLAGS) $(JCCL_SRC_DIR)/CList.cpp 

$(OBJ_DIR)/Common.o: ${JCCL_SRC_DIR}/Common.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/Common.cpp

$(OBJ_DIR)/Config.o: ${JCCL_SRC_DIR}/Config.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/Config.cpp

$(OBJ_DIR)/CString.o: ${JCCL_SRC_DIR}/CString.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/CString.cpp

$(OBJ_DIR)/Debug.o: $(JCCL_SRC_DIR)/Debug.cpp
	$(CC) -o $@ -c $(CFLAGS) $(JCCL_SRC_DIR)/Debug.cpp 

$(OBJ_DIR)/File.o: ${JCCL_SRC_DIR}/File.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/File.cpp

$(OBJ_DIR)/FTPClient.o: ${JCCL_SRC_DIR}/FTPClient.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/FTPClient.cpp

$(OBJ_DIR)/SystemBaseInfo.o: ${JCCL_SRC_DIR}/SystemBaseInfo.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/SystemBaseInfo.cpp

$(OBJ_DIR)/ModuleList.o: ${JCCL_SRC_DIR}/ModuleList.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/ModuleList.cpp

$(OBJ_DIR)/ModuleBase.o: ${JCCL_SRC_DIR}/ModuleBase.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/ModuleBase.cpp 

$(OBJ_DIR)/Codec.o: ${JCCL_SRC_DIR}/Codec.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/Codec.cpp

$(OBJ_DIR)/MsgBase.o: ${JCCL_SRC_DIR}/MsgBase.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/MsgBase.cpp

$(OBJ_DIR)/Message.o: ${JCCL_SRC_DIR}/Message.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/Message.cpp

$(OBJ_DIR)/MsgClientBase.o: ${JCCL_SRC_DIR}/MsgClientBase.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/MsgClientBase.cpp 

$(OBJ_DIR)/MsgServerBase.o: ${JCCL_SRC_DIR}/MsgServerBase.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/MsgServerBase.cpp 

$(OBJ_DIR)/MsgSocket.o: ${JCCL_SRC_DIR}/MsgSocket.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/MsgSocket.cpp

$(OBJ_DIR)/ASN1PERCoding.o: ${JCCL_SRC_DIR}/ASN1PERCoding.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/ASN1PERCoding.cpp

$(OBJ_DIR)/Timer.o: ${JCCL_SRC_DIR}/Timer.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/Timer.cpp 

$(OBJ_DIR)/Time.o: ${JCCL_SRC_DIR}/Time.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/Time.cpp 

$(OBJ_DIR)/FileDataEx.o: ${JCCL_SRC_DIR}/FileDataEx.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/FileDataEx.cpp 

$(OBJ_DIR)/DownloadFile.o: ${JCCL_SRC_DIR}/DownloadFile.cpp
	${CC} -o $@ -c ${CFLAGS} ${JCCL_SRC_DIR}/DownloadFile.cpp 


$(OBJ_DIR)/RSUMsgBase.o: $(V2X_Common)/RSUMsgBase.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/RSUMsgBase.cpp 

$(OBJ_DIR)/CSAEMsgBase.o: $(V2X_Common)/CSAEMsgBase.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/CSAEMsgBase.cpp 

$(OBJ_DIR)/CSAEDataFrame.o: $(V2X_Common)/CSAEDataFrame.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/CSAEDataFrame.cpp 

$(OBJ_DIR)/CSAEMessageSet.o: $(V2X_Common)/CSAEMessageSet.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/CSAEMessageSet.cpp 

$(OBJ_DIR)/CSAETest.o: $(V2X_Common)/CSAETest.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/CSAETest.cpp 

$(OBJ_DIR)/BaiduMessage.o: $(V2X_Common)/BaiduMessage.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/BaiduMessage.cpp 

$(OBJ_DIR)/RSUMessage.o: $(V2X_Common)/RSUMessage.cpp
	$(CC) -o $@ -c $(CFLAGS) $(V2X_Common)/RSUMessage.cpp 

