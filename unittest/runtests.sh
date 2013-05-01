#!/bin/bash

showgen=../showgen

result_suffix="result"
expected_suffix="expected"

failed_tests=""
missing_tests=""

function compare_files {
	if ! diff "$1" "$2" > /dev/null; then
		echo "diff $1 $2"
		diff "$1" "$2"
		return 1
	fi
	return 0
}

function run_test {
	local failed=

	if ! ls "$1.$expected_suffix"* >/dev/null 2>&1; then
		echo ""
		echo "No tests found for '$1'"
		missing_tests+="$1 "
		return
	fi

	for f in "$1.$expected_suffix"*; do

		local g=$(printf "$f" | sed -n "s/.*$expected_suffix.*\.g\([0-9]\+\).*/\1/p")
		local a=$(printf "$f" | sed -n "s/.*$expected_suffix.*\.\(a\).*/\1/p")
		local tx=$(printf "$f" | sed -n "s/.*$expected_suffix.*\.tx\([0-9]\+,[0-9]\+\).*/\1/p")
		local ty=$(printf "$f" | sed -n "s/.*$expected_suffix.*\.ty\([0-9]\+,[0-9]\+\).*/\1/p")
		local wx=$(printf "$f" | sed -n "s/.*$expected_suffix.*\.wx\([0-9]\+,[0-9]\+\).*/\1/p")
		local wy=$(printf "$f" | sed -n "s/.*$expected_suffix.*\.wy\([0-9]\+,[0-9]\+\).*/\1/p")

		local cmd="$showgen "

		[ -n "$a" ] && cmd+="-a "
		[ -n "$g" ] && cmd+="-g $g "
		[ -n "$tx" ] && cmd+="-tx $tx "
		[ -n "$ty" ] && cmd+="-ty $ty "
		[ -n "$wx" ] && cmd+="-wx $wx "
		[ -n "$wy" ] && cmd+="-wy $wy "

		cmd+="$1"

		# echo "$cmd"

		$cmd > "$1.$result_suffix"

		if ! compare_files "$f" "$1.$result_suffix"; then
			echo "FAILED: $cmd"
			failed=1
		else
			printf "."
		fi
	done

	if [ -n "$failed" ]; then
		failed_tests+="$1 "
		# echo "Test '$1' FAILED"
		return
	fi

	# Test successful
	rm "$1.$result_suffix"*
}

function run_all {
	for f in *.aut; do
		run_test "$f"
	done

	echo ""

	if [ -n "$missing_tests" ]; then
		echo "Untested files:"
		echo $missing_tests
		echo ""
	fi

	if [ -n "$failed_tests" ]; then
		echo "Failed tests:"
		echo $failed_tests
	else
		echo "All tests passed!"
	fi
}

function new_test {
	local i=0
	local f="$0"
	while [ -f "$f" ]; do
		printf -v f "test.%03d.aut" $((i++))
	done

	echo "Creating '$f' and '$f.$expected_suffix'"

	#touch "$f" "$f.$expected_suffix"

	printf "Name \"Test Example\";
Xrange 0 10;
Yrange 0 10;
Rules ConwaysLife;
Chars 46, 64;
Colors (255, 255, 255), (0, 0, 0);
Initial {
	State 1;
	Y = 1: 2, 3;
};
" > "$f"

	printf "...........
...........
...........
...........
...........
...........
...........
...........
...........
..@@.......
...........
" > "$f.$expected_suffix"

}

if [ -f "$1" ]; then
	echo "Running test: '$1'"
	run_test "$1"
	echo ""

	if [ -z "$failed_tests" ] && [ -z "$missing_tests" ]; then
		echo "Test '$1' passed!"
	fi

else
	case "$1" in
		new)
			new_test
			;;
		*)
			echo "Running all tests"
			run_all
			;;
	esac
fi
