
test:
	./pick.py -s ref.seeds -n ref.names -c16 >pick-output
	-diff pick-output ref.results >diff-output
	cmp diff-output ref.expected-diff
	rm pick-output diff-output
