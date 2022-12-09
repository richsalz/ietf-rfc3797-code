
test: test.ref test.2022

test.ref:
	@echo RFC 3797 reference output
	./pick -s ref.seeds -n ref.names -c16 >pick-output
	-diff pick-output ref.results >diff-output
	cmp diff-output ref.expected-diff
	rm pick-output diff-output

test.2022:
	@echo 2022 output
	./pick -s 2022.seeds -n 2022.names -d -c10 >pick-output
	diff pick-output 2022.results
	rm pick-output
