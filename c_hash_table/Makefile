# Use the gcc compiler
CC = gcc

# Set compilation flags
#   -ansi       check syntax against the American National Standard for C
#   -g          include debugging information
#   -Wall       report all warnings
#   -std=gnu99  use the GNU extensions of the C99 standard
CFLAGS = -ansi -g -Wall -std=gnu99

#----------------------------------------------------------------------------
# build rules:
#
# Each rule takes the following form  (Note there MUST be a tab,
# as opposed to several spaces, preceeding each command.
#
# target_name:  dependency_list
#	command(s)


all: map

map: hash_table.o word_map.o
	$(CC) $(LDFLAGS) -o map hash_table.o word_map.o

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c hash_table.c

word_map.o: word_map.c hash_table.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c word_map.c

#----------------------------------------------------------------------------
# cleanup rules: To invoke this command, type "make clean".
# Use this target to clean up your directory, deleting (without warning) 
#   the built program, object files, old emacs source versions, and core dumps.

clean:
	rm -f list *.o *~ core*