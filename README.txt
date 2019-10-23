This is a project completed by Will Curran, a student at Texas A&M University, for CSCE 313. It is not intended to be a resource for other students of this class.

Objective: Facilitate data transfer between two processes.


Given info to students:

You are given a source directory with the following files:
• A makefile that compiles and builds the source files when you type make command
in the terminal.

• FIFORequestChannel class (FIFORequestChannel.cpp/.h) that implements a pipe-
based communication channel. You can use this to communicate with another process.

This class has a read and a write function to receive and send data to/from the server,
respectively. The usage of the function is demonstrated in the given client.cpp. No
change in this class is necessary for PA2.
• A dataserver.cpp that contains the server logic. When compiled with the makefile,
an executable called dataserver is made. You need to run this executable to start the
server. Although nothing will change in this server for this PA, you will have to refer to
its code to understand the server protocol and then implement the client functionality
based on that.
• The client program in client.cpp that, for the time being, is capable of connecting
to the server using the FIFORequestChannel class. The client also sends a sample
message to the server and receives a response. Once compiled, an executable file
client is generated, which you would run to start the client program. This is the file
where you will make most of the changes needed for this assignment.

• A common.h and a common.cpp file that contain different useful classes and functions
potentially shared between the server and the client. For instance, if you decide to
create classes for different types of messages (e.g., data message, file message), you
should put them in these files.

The following are your tasks:

• Requesting Data Points: (worth 15 pts) Request all data points for person 1 by (both
ecg1 and ecg2), collect the responses, and put them in a file called x1.csv. Compare the file against the original 
BIMDC/1.csv using a file compare tool (e.g., fc) and demonstrate that they are exactly same. Also, measure the 
time do the entire thing by using gettimeofday function.
• Requesting Files: (worth 35 pts) Request an entire file by first sending a message to
get its length, and then a series of messages to get the actual content of the file. Put
the content of the flie in a single output file called y1.csv. Compare the file against
the original and demonstrate that they are exactly same. Also, measure the time to
obtain the file.
Make sure to treat the file as binary, because we will use this same program to transfer
any type of file. Putting the data in a STL string will not do, because C++ strings
are NULL terminated. To demonstrate that your file transfer is capable of handling
binary files as well, make a large empty file under the BIMDC/ directory using the 
truncate command (see man pages on how to use truncate), transfer that file, and
then compare to make sure they are identical using the diff command.
• Requesting a New Channel: (worth 15 pts) Ask the server to create a new channel
for you by sending a special NEWCHANNEL MSG request and join that channel. After
the channel is created, demonstrate that you can use that to speak to the server.
Sending a few data point requests and receiving their responses is adequate for that
demonstration.
• Run the Server as a child process (worth 15 pts) Run the server process as a child of
the client process using fork() and exec() such that you do need two terminals: 1 for
the client and another for the server. The outcome is that you open a single terminal,
run the client which first runs the server and then connects to it. Also, to make sure
that the server does not keep running after the client dies, sent a special QUIT MSG to
the server and call wait() function to wait for its finish.
• Closing Channels worth 5 pts You must also ensure that there are NO open connections
at the end and NO temporary files remaining in the directory either. The server would
clean up this resources as long as you send QUIT MSG at the end. This part is worth 5
points. Note that the given client.cpp already does this for the control channel.
