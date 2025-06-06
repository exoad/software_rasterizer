# software_rasterizer

Resources:
1. [UT Austin CS354 ~ Line Rasterization](https://www.cs.utexas.edu/~bajaj/graphics2012/cs354/lectures/lect02.pdf)
2. [UT Austin CS354 ~ Projection Clipping](https://www.cs.utexas.edu/~fussell/courses/cs354-fall2015/lectures/lecture9.pdf)
3. [Paul Bourke Object Files](https://paulbourke.net/dataformats/obj/)
4. [Cornell CS4620 View Explorer](https://www.cs.cornell.edu/courses/cs4620/2019fa/demos/view_explore/view_explore_ortho.html)

## Building

This project uses a very simple [Makefile](./Makefile). Please note that this project does use non standard ISO C features like `statement-expressions` which means a preferable compiler
will be GCC or Clang.

By default it compiles without an address sanitizer (because I am on Windows and the GCC MingW64 installation I have
does not have a native ASAN port and I am too lazy):

```bash
make re
```

Outputs a final executable in the root directory and other compiled objects are located in [`/obj`](./obj/)

> Running `make` also works, but it can be problematic for not cleaning up old compilation stuffs

If you have and can link with an address sanitizer build it with the `asan` target:

```bash
make asan
```
