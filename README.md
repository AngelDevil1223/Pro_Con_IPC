PROJECT 1 PART 1 Deliverables

Your project deliverables are to be posted to CUNY Blackboard using the Project 1 link.

1) Only one source code file as follows with filename format showing your last name and student id: MYLASTNAME_MYSTUDENTID.c  

 a) At top of the file: your full name in comments, for example:  // First_Name Last_Name 

 b) In-line code comments no fewer than every 5 lines of code which describe your algorithmic steps in plain English language.
    That is, I do not want to see literal syntax transcriptions of the code.  

2) An MS Word or PDF document with a write-up of your software design and an explanation of the issues encountered and resolved. (1 page)
   The write-up is essay style with no bullet points nor itemized lists nor source code nor pseudo-code listings.
   Filename format is MYLASTNAME_MYSTUDENTID.doc (or pdf.

3) Text file of standard output to screen according to output format in (I) of Project 1 Part 1 Description and Requirements (below) 
   for your program execution with command line arguments CHUNK_SIZE = 48 and BUFFER_SIZE = 10 and the source input file that I have     
   included in CUNY Blackboard for the project.


Project 1 Part 1 Description and Requirements  
--------------------------------------------

(A) In your first project you will create a program that, when executed, has both a parent process and a child process. You use the fork()system call to create a child process as given in the lectures for Chapter 3 Process Creation and Termination. Please refer to your lecture notes.

(B) The parent process code branch utilizes the Producer Algorithm for Shared Memory while the child process code branch makes use of
the Consumer Algorithm for Shared Memory as given in the lectures for Chapter 3 Producer and Consumer Model for Shared Memory. 
Please refer to your lecture notes. The BUFFER_SIZE is to be accepted by your program as a command line argument.

(C) The Parent Producer and Child Consumer Algorithms are to be implemented using the POSIX Shared Memory API as given in the lectures 
for Chapter 3 POSIX Shared Memory API. 

(D) The Parent Process produces the file contents into shared memory as read from a source file (SRC_FILE) in chunks using read() (as was discussed in the Chapter 3 lectures) while the Child Process writes the shared memory items to a target file (TGT_FILE) using write() (as was discussed in the Chapter 3 lectures). The chunk size (CHUNK_SIZE) is the same for both read() and write(). It will be accepted as input by your program as a command line argument. 

(E) You are responsible for designing and implementing a strategy for the termination of the Parent and Child processes when the entire source file has been copied to the the target file.

(F) You may use the C standard library malloc() function for dynamic memory allocation. Otherwise, points will be deducted for not using
the system calls elsewhere.

(G) I will compile and link your source code to generate an executable file using the following command. Therefore, you have to verify that your program can be compiled and linked using the GNU C compiler as was discussed in lectures:
 
   gcc MYLASTNAME_MYSTUDENTID.c  -o MYLASTNAME_MYSTUDENTID.exe

(H) I will run your program as follows with different values of command line arguments in the following order:

   ./MYLASTNAME_MYSTUDENTID.exe SRC_FILE TGT_FILE CHUNK_SIZE BUFFER_SIZE 

(I) Your program will generate output to the screen from the child (consumer) process for each item consumed from shared memory in the  
    following format using write():

   "CHILD: OUT = %d\n"
   "CHILD: ITEM = %s%\n"

    Your program will also generate output to the screen from the parent (producer) process for each item produced to shared memory in 
    the following format using write():
   
   "PARENT: IN = %d\n"
   "PARENT: ITEM = %s%\n"

Project 1 Part 2 Description and Requirements  
--------------------------------------------
(A) Create a pipe between the parent (producer) and child (consumer) processes using the POSIX pipe() system call.
(B) As per the instructions given in lecture, the parent process will have an integer counter variable named shMemPrdCharCount that will be incremented by the number of characters produced into shared memory each time the producer puts a chunk of source file content into a shared memory bufffer element.
(C) As per the instructions given in lecture, the child process will have an integer counter variable named shMemCsrCharCount that will be incremented by the number of characters consumed from shared memory each time the consumer reads a chunk of content from a shared memory buffer element.

(D) After the last chunk of source file input has been produced to shared memory, the producer will write the value of shMemPrdCharCount (from item (B)) to the write end of the pipe and the producer will generate output to the screen of the value of shMemPrdCharCount in the following format:"PARENT: The parent value of shMemPrdCharCount  = %d\n"
(E) After the last chunk of content has been consumed from shared memory by the consumer, the consumer will read the value of shMemPrdCharCount (that was written to the pipe by the producer in (D) ) from the pipe and the consumer will generate output to the 
    screen from the child process with the value read from the pipe and the value of shMemCsrCharCount in the following format:
   "CHILD: The parent value of shMemPrdCharCount  = %d\n"
   "CHILD: The child value of shMemCsrCharCount  = %d\n"
(F) As with Part 1 of the project you must use the write() system call for all output to the screen (standard out) from the parent and child processes.I mentioned in Part 1 of the the project instructions (item (3)) that you must place all your screen output to one text file for your project submission deliverables. Yopu will use this same output text file for screen output from Part 2. No graphics files of screen output will be acccepted.