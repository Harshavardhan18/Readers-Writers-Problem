# Readers-Writers-Problem
Readers-Writers using c and GNOME toolkit

Problem Statement:
There is a shared resource which should be accessed by multiple processes. There are two types of processes in this context. They are reader and writer. Any number of readers can read from the shared resource simultaneously, but only one writer can write to the shared resource. When a writer is writing data to the resource, no other process can access the resource. A writer cannot write to the resource if there are non zero number of readers accessing the resource. 

Solution:
From the above problem statement, it is evident that readers have higher priority than writer. If a writer wants to write to the resource, it must wait until there are no readers currently accessing that resource.
Here, we use one mutex m and a semaphore w. An integer variable read_count is used to maintain the number of readers currently accessing the resource. The variable read_count is initialized to 0. A value of 1 is given initially to m and w.
Instead of having the process to acquire lock on the shared resource, we use the mutex m to make the process to acquire and release lock whenever it is updating the read_count variable.

The algorithm for the writer process:
while(TRUE) {
   wait(w);
   /*perform the 
write operation */
   signal(w);
}

The algorithm for the reader process:
while(TRUE) {
   wait(m);   //acquire lock
   read_count++;
   if(read_count == 1)
          wait(w);
   signal(m);  //release lock
   /* perform the 
     reading operation */
   wait(m);   // acquire lock
   read_count--;
   if(read_count == 0)
          signal(w);
   signal(m);  // release lock
} 

To Compile: gcc -pthread `pkg-config --cflags gtk+-3.0` -o Readers_Writers Readers_Writers.c `pkg-config --libs gtk+-3.0`

To Execute: ./Readers_Writers
