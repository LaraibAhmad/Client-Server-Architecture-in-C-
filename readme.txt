Follow the following steps to run the code:-
(1) compile server.cpp file on Ubuntu or any Linux terminal using following command
	g++ -o server server.cpp (type in terminal)
(2) run the server.cpp using following command
	./server (type in terminal)
(3) open another new terminal
(4) In new terminal type the following command to compiler the client.cpp program
	g++ -o client1 client.cpp (type in new terminal)
(5) run the client.cpp program
	./client1 (type in new terminal)
(6) now enter the message in new terminal, server will be replying
	your message as mentioned in question.
NOTE:- you can create multiple clients just by opening multiple terminals, 
	compiling and running the my_new_client.cpp
       e.g : open another 3rd terminal. This is your 2nd client
	compile: g++ -o client2 client.cpp (type in 3rd terminal)
	run: ./client2
NOTE:- run server program before running clients program
