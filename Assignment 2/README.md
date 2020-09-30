# ECE 252 Assignment 2: Rogue One: A Star Wars Story

### Intended to be completed after lecture: 8

You are Jyn Erso, a traitor to His Majesty the Emperor, a member of the vile rebellion. You are trapped on the planet Scarif, having stolen the plans for the Death Star, and you plan to transmit them to the rebel fleet above. Time is short, because Lord Vader is on his way... and you know how he deals with traitors. You need to upload the plans, in the hopes that the rebellion can analyze them and possibly find a weakness. Upon transmission of the plans you'll receive an answer from the command ship indicating whether or not the transmission was successful. Furthermore, the Death Star has just dropped out of hyperspace -- and Grand Moff Tarkin is keen to open fire on the facility.

### Primary Objective
The primary objective of this exercise is to practice communicating with a remote server using socket communication in C.


### Description of Behaviour 

The goal is to implement the program so that the following behaviour occurs:


* Your program should take in the IP of the rebel command ship (remote server) as the first argument. So if the IP to use is `127.0.0.1` then the program is invoked like `bin/rogue-one 127.0.0.1`. (The actual IP will be kept up-to-date in Piazza, because these things can and do change with time!).
* Establish a TCP/stream connection to the command ship  This is the client-side, so you need to get the address information (`getaddrinfo()`), create a socket (`socket()`), and then `connect()` to the remote server.
* Once connected, transmit the plans for the Death Star! The plans are loaded for you in the `load_plans` function (which loads the file into memory for you -- remember to deallocate the data when done). The plans are large, so you perhaps cannot send it all in one call to `send()`; so check the results and send repeatedly as needed.
* After you make your transmission, try to receive (using `recv()`) a response that could be up to 64 characters long. If your upload was unsuccessful you will get an error of some sort, otherwise you will get back a message that indicates success. You should print this message so you can see what it says. If it says ``Success'', it worked!
* Close the connection.
* Your program should not leak memory; be sure to destroy/deallocate anything initialized/allocated.

### General Guidance 

If you're having trouble, try checking these things, and it may resolve your problem.

* Check the documentation for how functions work if you are unfamiliar with them (google is your friend!)
* Check return values (and possibly `errno`) for network functions if things are going wrong.
* Have you initialized all variables? It is easy to forget; `malloc` does not initialize the value...
* Pay attention to compiler warnings. Warnings can sometimes be ignored, but may be a hint about a semantic error that you will want to resolve. If the compiler is trying to tell you something, listen.
* You need a buffer for receiving the data, and the receive function tells you how many bytes the server sent.
* Don't forget null terminators for strings!
* Is there a missing or extra `*` (dereference) on a pointer somewhere?
* Does every memory allocation have a matching deallocation?
* It may be helpful to put `printf()` statements to follow along what the program is doing and it may help you narrow down where the issue is.

### Submitting Your Code. 
When you're done, use the command `git commit -a` to add all files to your commit and enter a commit message. Then `git push` to upload your changes to your repository. You can commit and push as many times as you like; we'll look at whatever was last pushed.
