Assignment 3 - SYSC 4001

Bhavik Tailor 100973983
Tanzim Zaman 100969782

Assignment3.docx - Part 1 answers
part2.c - source code for the ATM and Server
editor.c - source code for the DB Editor
DataBase.txt - the database text file
part2.o - object file for ATM and Server
part2 - runnable file for ATM and Server
editor.o - object file for DB Editor
editor - runnable file for DB Editor

To compile each program
part2.c:

in command line navigate to the folder in which the source code is maintained and type

gcc part2.c -o run -lpthread

editor.c:

open a new terminal and navigate to the file

gcc editor.c -o editor -lpthread

to run the files type

./run

and in another console type 

./editor

