
RED='\e[1;31m'
GREEN='\e[0;32m'
CYAN='\e[0;36m'
RESET='\e[0m'

# Variable determinig if the program should be run with valgrind.
# 1 -> NO
# 0 -> YES
TEST_WITH_VALGRIND=0

# Checking if the correct number of arguments was passed to test.sh script.
if (($# != 2))
then
    echo -e "${RED} ERROR \n ${RESET}"
    exit 1
fi

prog=$1
dir=$2

# Correcting file [prog] name so it is ready to be executed
if [[ "${prog}" != /* ]];
then
  prog="./${prog}"
else
  prog="${prog}"
fi

# Checking if given file [prog] exists.
if [ ! -f "${prog}" ];
then
    echo -e "${CYAN} No such file!!! ${RESET}"
    exit 1
fi

# Checking if given directory [dir] exists.
if [ ! -d "${dir}" ];
then
  echo -e "${CYAN} No such directory!!! ${RESET}"
  exit 1
fi

# Creating temporary directory for program outputs.
mkdir tmp

for input in "${dir}"/*.in;
do
    # Creating paths to correct output files
    correct_output="${input%.in}".out
    correct_errors="${input%.in}".err

    # Creating paths program output files
    file_name=$(basename "${input}")
    program_out=tmp/"${file_name%.in}"_tmp.out
    program_err=tmp/"${file_name%.in}"_tmp.err

    # Running the program
    "${prog}" < "${input}" > "${program_out}" 2> "${program_err}"

    # Checking if the program should be run with valgrind
    if [[ "${TEST_WITH_VALGRIND}" == 0 ]]
    then
        valgrind --error-exitcode=123 --leak-check=full \
        --show-leak-kinds=all --errors-for-leak-kinds=all \
            "${prog}" < "${input}" > /dev/null 2> /dev/null

        if [[ $? == 123 ]]
        then
            echo -e "${input}: ${CYAN}!!!${RED}MEMORY ERROR${CYAN}!!!${RESET}"
        else
            echo -e "${input}: ${GREEN}MEMORY TEST PASSED${RESET}"
        fi
    fi

    # Checking if the program outputs were correct
    if diff -Z "${correct_output}" "${program_out}" > /dev/null \
        && diff -Z "${correct_errors}" "${program_err}" > /dev/null
    then
        echo -e "${input}: ${GREEN}PASSED!${RESET}"
    else
        echo -e "${input}: ${RED}ERROR!!!!!!!!${RESET}"
    fi

    rm "${program_out}"
    rm "${program_err}"
done

rm -r tmp

