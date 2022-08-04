# RFC 3797 Implementation

This is an implementation of IETF RFC 3797.

It includes the reference input files and output from that RFC.

 make test

Will run the program with the program and compare the output against
the expected diff.  (The program adds the names, the RFC lists the
names separately; I joined them in the `ref.results` file.)
