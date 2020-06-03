CXX ?= g++

DEBUG ?= 1

ifeq ($(DEBUG),1)
	CXXFLAGS += -g
else
	CXXFLAGS += -O2

endif

server: main.cpp ./utils/utils.* ./chat/ChatConnection.* ./chatserver/ChatServer.* ./lock/locker.h ./threadpool/threadpool.h ./utils/libevent_utils.*
	$(CXX) -o server $^ $(CXXFLAGS) -lpthread -levent

clean:
	rm -r server
