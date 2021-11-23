.PHONY: all, clean





CODE_OBJECTS = hashmap.o vector.o pair.o
TEST_OBJECTS = test_suite.o


ALL_LIBRARIES :=  libhashmap.a  libhashmap_tests.a


CC = gcc
AR = ar -rcs $@ $^


CCFLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99


all: $(ALL_LIBRARIES)


libhashmap.a:$(CODE_OBJECTS)
	$(AR)

libhashmap_tests.a:$(TEST_OBJECTS)
	$(AR)




test_suite.o: test_suite.c test_suite.h hash_funcs.h test_pairs.h hashmap.h pair.h
	$(CC) $(CCFLAGS) -c $<

hashmap.o: hashmap.c hashmap.h vector.h
	$(CC) $(CCFLAGS) -c $< -o $@

vector.o: vector.c vector.h
	$(CC) $(CCFLAGS) -c $<	-o $@

pair.o: pair.c pair.h
	$(CC) $(CCFLAGS) -c $< -o $@


clean:
	rm *.o *.a