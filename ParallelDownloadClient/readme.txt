# Multithreaded P2P Server
## CS449 - Project #5
@author: Anthony (Tony) Poerio

## Notes 
This program will run a multithreaded download from the serv.c program and output the result in a file anmed "transferred_data.txt".

I've left a number of print statements in the final program so that you can see that multithreading is indeed functional. 

I print whenever a connection is made and note which server is making the connection + what data it has downloaded. 

I'm also printing a few other things, just as confirmations.

## Usage
To run the program, you can Make the executables simply by running "make", using the attached Makefile. 

First you'll need to setup servers using this command: 
./serve.exe -p 1000 testFile.txt &

Then, you can run the client using this command:
./client.exe 127.0.0.1 1000

Where 1000 represents some arbitrary port number that you can access for this purpose.  This program should support any number of ports, but I've only tested up to 3. 