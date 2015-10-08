EllErre - An LR automata generator
================================

EllErre is an LR automata generator. It implements the following algorithms:

+ First Set
+ Follow Set
+ LR(0) automata
+ SLR(1)
+ LR(1)

It is current in development.

### Installation

     sudo apt-get install git cmake build-essential flex bison;
     git clone git://github.com/schnorr/ellerre.git ; mkdir -p ellerre/b ; cd ellerre/b ; cmake .. ; make

### Execution

     ./ellerre < ../examples/g1.ee
