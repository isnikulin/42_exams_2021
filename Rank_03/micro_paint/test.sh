#! /bin/bash

ORIG="our_micro_paint.c"
ORIG_EXEC="original"
CUST="micro.c"
CUST_EXEC="custom"

gcc $ORIG -o $ORIG_EXEC -Wall -Wextra -Werror -lm
gcc $CUST -o $CUST_EXEC -Wall -Wextra -Werror -lm

if [[ -f "$ORIG_EXEC" && -f "$CUST_EXEC" ]]; then
	echo "Compiled successfully. Running tests."
	echo "****************************************"
for i in test/*;
do
	echo
	echo "Running test: $(basename $i)"
	./$ORIG_EXEC $i >expected;  CODE_ORIG=$?
	./$CUST_EXEC $i >actual;    CODE_CUST=$?
	if (( CODE_ORIG != CODE_CUST )); then
			echo "$(tput setaf 1)[KO - RETURN CODES]$(tput sgr0)"
			echo "Expected: "$CODE_ORIG", Actual: "$CODE_CUST""
			KO_FLAG=1;
	fi
	diff -c "expected" "actual"
	if [ $? == 1 ]
	then
		echo "$(tput setaf 1)[KO - DIFF]$(tput sgr0)"
  elif (( KO_FLAG == 1 )); then
    echo "$(tput setaf 2)[OK - DIFF]$(tput sgr0)"
  fi
	if (( KO_FLAG == 0 )); then
		echo "$(tput setaf 2)[OK]$(tput sgr0)"
	fi
done
	echo
	echo "****************************************"
	rm -f "$ORIG_EXEC" "$CUST_EXEC" "expected" "actual"
else
	echo "Compilation failed!"
fi
