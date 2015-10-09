echo “Cpt S 422 Client/Server build”
g++ -pthread -std=c++11 -o server EchoServer.cpp
g++ -pthread -std=c++11 -o client Client.cpp TCPComm.cpp
