# image processing from scratch

## what is this?
This repository contains many interesting image processing algorithms that are written from scratch. Read these codes will allow you to have a comprehensive understanding of the principles of these algorithms. I also have video tutorials for these algorithms [here](https://space.bilibili.com/14672002). Go check out if you know Chinese :-)  

<strong>Implementation</strong>  
All codes were wrote in _python3.7_ or _c++_  
moudles you may need:  
_python:_ 
- numpy for matix calculation  
- matplotlib for reading and showing images  
- opencv2 for some image operations  

_c++:_  
- opencv2  

<strong>Usage</strong>  
you can always run a python script just by  
    `python script.py`  

for c++, you need to compile first  
    `cd build`  
    `cmake ..`  
    `make`  

when it's done, you are ready to run the executable file by  
    `./Main`  
Just make sure you have the images in the right path, and you might wanna modify the code a bit to process another image.  
<strong>Have fun!</strong>  

## Contents
* canny edge detection  
It is an algorithm that extracts edges of an image.  
![](./results/canny edge detection/lena.png)![](./results/canny edge detection/lena.jpg)  
* hough transform
It is an algorithm that can theoratically detects shapes that you can write formulas for it.  
![](./results/hough transform/Line/02.jpeg)![](./results/hough transform/Line/2.jpg)  
![](./results/hough transform/Circle/2.jpeg)![](./results/hough transform/Circle/twitter.jpg)  

