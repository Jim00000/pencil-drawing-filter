# pencil-drawing-filter

This project implements an pencil drawing filter based on some methods purposed from some papers

## Build

At first, make sure your computer has libraries listed bleow

- CMake 
- OpenCV
- OpenCV-contrib

and use CMake to build this project

```
$ cd pencil-drawing-filter
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make
``` 

## Usage

In this moment, the user interface has not been done yet

## Demo

![demo_image_1](resources/demo/sample.jpg)
![demo_image_1_output](resources/demo/sample_output.jpg)
![demo_image_2](resources/demo/sample2.jpg)
![demo_image_2_output](resources/demo/sample2_output.jpg)

## References

- [AUTOMATIC GENERATION OF PENCIL SKETCH FOR 2D IMAGES](https://pdfs.semanticscholar.org/6317/a749fe29467cdb36b0b3cef492940ee6beb9.pdf)
- [Enanced pencil filter](http://ieeexplore.ieee.org/document/1323994/)
- [Imaging Vector Fields Using Line Integral Convolution](https://dl.acm.org/citation.cfm?id=166151)
- [Texture Synthesis with Line Integral Convolution](http://www.dcs.ed.ac.uk/teaching/cs4/www/visualisation/SIGGRAPH/lic.pdf)

## License

GNU GENERAL PUBLIC LICENSE Version 3