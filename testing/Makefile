ROOT = ..
TARGETS += graph_test
TARGETS += rt_test
PREREQS = $(ROOT)/deps/gtest

LDFLAGS = -L$(ROOT)/graph
LIBS = graph pthread

include $(ROOT)/common.mk

test:: graph_test
	./graph_test

runtime_test:: rt_test
	./rt_test
