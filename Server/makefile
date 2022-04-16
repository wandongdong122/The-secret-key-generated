src=$(wildcard *.cpp)
objs=$(patsubst *.cpp,%.o,$(src))
target=serverMain
LIBS=-lpthread -lstdc++ -lssl -lcrypto

$(target):$(objs)
	g++ -std=c+=11 -o $@ $^ ${LIBS}
%.o:%.cpp
	g++ -std=c++11 -c $<

.PHONY:clean
clean:
	rm -f $(target) $(objs)


