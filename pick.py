#! /usr/bin/env python3
'''
Use -h for help
'''

import argparse
import hashlib

# Argument parsing
parser = argparse.ArgumentParser(description='Do RFC3797 randomization')
parser.add_argument('-s', '--seedfile',
        help='File containing the seeds',
        metavar='seeds', type=open, required=True)
parser.add_argument('-n', '--names',
        help='File containing the list of names',
        metavar='names', type=open, required=True)
parser.add_argument('-c', '--count', 
        help='Number of entries to print',
        default=12, metavar='N', type=int)
args = parser.parse_args()

# Read the names, print the count
names = [ line.strip() for line in args.names ]
numnames = len(names)
# print('Read', numnames, 'names')
if numnames > 0xFFFF:
    raise SystemError("Too many names (under 0xFFFF)")

# From RFC 3797; "notpicked" might be a better name
selected = [i + 1 for i in range(0, numnames) ]

# Read the seeds.
seeds = []
for line in args.seedfile:
    if line[0:1] == '#': continue
    values = [ int(x) for x in line.split() ]
    values.sort()
    seeds.append(values)

# Generate the key string and convert to bytes
keystring = ''
for s in seeds:
    keystring += '.'.join([ str(n) for n in s ])
    keystring += './'
# print('Key string is ', keystring)
keybytes = keystring.encode()

# Do the work
prefix = 0
div = numnames
print(' index        hex value of MD5        div  selected')
for i in range(0, args.count):
    bracket = prefix.to_bytes(2, 'big')
    result = hashlib.md5(bracket + keybytes + bracket).hexdigest().upper()
    k = int(result, 16) % div
    for j in range(0, numnames):
        if selected[j] != 0:
            k -= 1
            if k < 0: break
    print(' %4d  %s  %2d  -> %2d <- %s' % \
            (i + 1, result, div, selected[j], names[j]))
    selected[j] = 0
    div -= 1
    prefix += 1

