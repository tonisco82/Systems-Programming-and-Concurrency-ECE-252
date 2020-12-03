# ECE 252 Assignment 5: The Time War

### Intended to be completed after lecture: 22

It is the last day of the Time War. The Daleks have launched their final assault on the Time Lords' home planet of Gallifrey. Ten million Dalek ships surround the planet and are bombarding it from orbit and sending landing parties. The War Doctor has stolen an ancient weapon called The Moment. It will wipe out the Daleks -- but also Gallifrey, and everyone on it.

But there's another way. Through hundreds and hundreds of years' worth of calculations The Doctor can use stasis technology to freeze the whole planet of Gallifrey in a parallel pocket universe. The Daleks will be obliterated in the crossfire and it would look like they wiped each other out. This takes hundreds and hundreds of years of calculations and the power of every one of the thirteen incarnations' TARDISes.

This won't be easy -- the timing has to be just right and the Doctors have to coordinate their actions and timing and work together to save Gallifrey. If their timing is wrong or if things are out of sync or important steps are skipped, the Doctors will fail and Gallifrey falls. If you can find and correct the problems in the program, then Gallifrey stands!

### Primary Objective
The primary objective of this exercise is to identify and solve memory management and concurrency problems in the provided code.

## Description of Behaviour
The goal is to analyze and correct the program so that there are no longer any errors (that you can fix in your program) reported by Helgrind, and also no memory leaks/issues (that you can fix in your program) reported by Valgrind.

When you compile and run the starter code, the program may crash or hang when executed. Those are just two of the symptoms of problems present. There are a total of nine (9) different problems in the code to be resolved. Some will be found by the memcheck tool of Valgrind; others will be found by the Helgrind tool. If you read the error messages carefully, you will be able to identify the causes of the problems and fix them.

There will be some things reported by both tools that are beyond your ability to fix (because they are in libraries) or otherwise misreported. You'll need to examine the messages carefully and decide if it is something you can fix or not. If not, just leave it be. Generally speaking, errors reported by memcheck need to be fixed; those reported by Helgrind need to be examined more carefully.

If something is wrong, you may see "Gallifrey falls" as an output message at the end of your program. If everything goes well you will see the message "Gallifrey Stands!" at the end of the program, but just seeing that that is not enough. You also need a clean bill of health from Valgrind and Helgrind.


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
* See the docs for Valgrind~\cite{valgrind:tools}.
* Read the valgrind error messages carefully: they usually tell you which function (i.e., which doctor) the problem occurs in. This helps you narrow it down 


### Submitting Your Code. 
When you're done, use the command `git commit -a` to add all files to your commit and enter a commit message. Then `git push` to upload your changes to your repository. You can commit and push as many times as you like; we'll look at whatever was last pushed.
