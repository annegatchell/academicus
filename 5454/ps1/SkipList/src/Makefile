JCC = javac
JFLAGS = -g

MAIN_SRCS = main/*.java
MAIN_CLASSES = $(MAIN_SRCS:.java=.class)

TEST_SRCS = test/*.java
TEST_CLASSES = $(TEST_SRCS:.java=.class)

ALL_SRCS = $(MAIN_SRCS) $(TEST_SRCS)
ALL_CLASSES = $(MAIN_CLASSES) $(TEST_CLASSES)

.PHONY: all clean test main

all: test main
test: $(TEST_CLASSES)
main: $(MAIN_CLASSES)

$(TEST_CLASSES): $(TEST_SRCS)
	$(JCC) $(JFLAGS) $(TEST_SRCS)

$(MAIN_CLASSES): $(MAIN_SRCS)
	$(JCC) $(JFLAGS) $(MAIN_SRCS)

clean:
	$(RM) src/main/*.class src/test/*.class