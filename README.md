# External-Sort-using-C-++
External sort is a sorting algorithm that can handle huge amounts of data.It is required when that data being sorted do not fit in the main memory(RAM). Hence, the file containing this large data is kept on the disk and chunks of this file are taken in the main memory, which are then sorted individually and then merged into one single output file.

This is the C++ code for implementing sorting in a large file containing lines of text.
Sorting is based off the number of occurences of 's' or 'S' in each line.
This is the stable sorting algorithm such that if two lines have the same number of occurences of 's' or 'S', then they will
be in the same order in the output file as in the input file.


Run Commands :

g++ sortme.cpp -o sortme.out

sortme.out inputfile outputfile available_memory[optional]


Arguments:

Argument	Argument-type		Description

first		mandatory		path to input file

second		mandatory		path to output file

third		optional		total memory available to the process


Assumptions :

1) Size of one line of text in any file cannot be more than the size of available memory. 
2) Sum of the sizes of single lines of text taken from each partitioned file should not be greater than the available 
   memory size.
3) I have assumed to take the number of occurences of both 's' and 'S' irrespective of the case.



Environment:

I have run the code correctly in the following environment :

g++.exe (GCC) 4.7.0
Windows platform




