all:read write

read:reader.cc test.pb.cc
	g++ -o $@ $^ -std=c++11 -lprotobuf

write:writer.cc test.pb.cc
	g++ -o $@ $^ -std=c++11 -lprotobuf

.PHONY:clean
clean:
	rm -f write read