EllErre - An LR parser generator
================================

EllErre is an LR parser generator. It implements the following algorithms:

+ First Set
+ Follow Set
+ LR(0) parser
+ SLR(1) parser
+ LR(1) parser

It is current in development.

### Installation

     sudo apt-get install git cmake build-essential flex bison;
     git clone git://github.com/schnorr/ellerre.git ; mkdir -p ellerre/b ; cd ellerre/b ; cmake .. ; make

### Execution

     ./ellerre ../examples/g1.ee
