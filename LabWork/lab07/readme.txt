Anthony Poerio
(adp59@pitt.edu)
Lab 07

README EXPLANATION
------------------

- I updated the freader.c file so that there are mutex locks around ALL
code segments that touch **GLOBAL VARIABLES**.
- This strategy worked, and in particular, I all of my tests starting passing
as soon as I placed a lock __around the ENTIRE while loop in readFile()__,
instead of __within__ the loop itself.
- This makes makes me believe that the **BufCnt** variable was being accessed,
AND INCREMENTED by multiple threads, and that there could be multiple threads
iterating thread the while loop.
- This leads to a situation where the while ends as soon as ANY **one**
of the files has exhausted all of its input. And because of this,
the while loop exits prematurely.
- The chief issue is that the while loop ends as soon as there are no more
values left to read with fscanf(). And so: if multiple threads are executing
within the while loop at the same time, then as soon as the first file 
runs out of data, the loop will exit, even though the other thread may
still have data to read and print.
