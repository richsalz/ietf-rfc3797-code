# RFC 3797 Implementation and Tools

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

    ./pick -s seeds -n names -cCOUNT

where COUNT is the number of entries you want. A nice feature of
the algorithm -- indeed, the whole point of it -- is that it is
repeateable, so running the program with the same input files and
different `COUNT` values will get the same output for the same
positions.

## Deliberations

The same program is also used for deliberation meetings.
In this case the `names` file is the 10 voting volunteers.
I used the URL of the meeting webext as the `-t` seed text.

See the `deliberations` Bash script.

## Extra seeding

When a volunteer is unable or disqualified, it had been common practice to
pick the next one on the list, although this is not mentioned in RFC 8713.
Martin Thomson (NomCom Chair 2023-2024) used a hash-chain for this situation,
starting from a private seed and working backwards.  See RFC 1760 for more
details.  The `extra-seed` program posted by Martin does this, and the
`-x` flag to `pick` is used to add in this extra seed string.

## C Reference code

An algorithm for picking volunteers was described in RFC 3797, and
the reference code from that RFC can be found in `./reference-code`
I added the `-x` flag to that code.
