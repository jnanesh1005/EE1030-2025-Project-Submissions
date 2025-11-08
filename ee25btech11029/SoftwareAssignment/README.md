\## Steps to run the code

 

\*\*first compile all the files\*\*

 

gcc main.c matrixutils.c image\_io.c svd.c analysis.c svd\_io.c -o svd\_compress -lm

 

\*\*compile convert.c\*\*

 

gcc convert.c -o jpeg\_to\_pgm

 

\*\*then use executable file to create the image\*\*

 

./jpeg\_to\_pgm <image.jpg> <new\_image.pgm>

 

\*\*after this use the python program to run all the C files together\*\*

 

python3 convert.py

 

\*\*after this convert the pgm file to jpeg\*\*

 

\*\*compile pgm\_to\_jpeg.c\*\*

 

gcc pgm\_to\_jpeg.c -o pgm\_to\_jpeg

 

\*\*then execute this\*\*

 

./pgm\_to\_jpeg <image.pgm> <image.jpg>



