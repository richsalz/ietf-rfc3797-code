# RFC 3797 Implementation

This is an implementation of IETF RFC 3797 in Python3

It includes the reference input files and output from that RFC.

    make test

Will run the program with the program and compare the output against
the expected diff.  (The program adds the names, the RFC lists the
names separately; I joined them in the `ref.results` file.)

To run the program you need to have a `seeds` file. In this file, lines
beginning with a`#` are ignored. All other lines are taken to be
a space-separated set of numbers (order does not matter) that are
used as the entropy seeds.

You also need a `names` file. This is a list of names, one per line,
from which to chose. There are no comment lines, all lines are
significant.

To run the program, you can use `-h` for help. Normal use is

    ./pick.py -s seeds -n names -cCOUNT

where COUNT is the number of entries you want. A nice feature of
the algorithm -- indeed, the whole point of it -- is that it is
repeateable, so running the program with the same input files and
different `COUNT` values will get the same output for the same
positions.
