# These are variables
CC= g++ -Wall -Wextra -Wshadow -Werror -lm
PROG=Main

# Actual makefile


# Run the script
.SILENT:
RUN:			${PROG}.exe
				echo ""
				echo ""
				echo ".\${PROG}.exe"
				.\${PROG}.exe
				echo ""


# Make the executable file
${PROG}.exe: 	${PROG}.o
				${CC} ${PROG}.o -o ${PROG}.exe


# Create the object file
${PROG}.o:		${PROG}.cpp
				${CC} -c ${PROG}.cpp


# Notes
# gcc -Wall -Wextra -Wshadow -Werror
#		-Wall: raise all standard warning
#		-Wextra: raise extra warnings
#		-Wshadow: raise a warning when a variable is defined with the same name as one in a higher scope
#		-Werror: treat every single warning as a hard error and stop compilation.