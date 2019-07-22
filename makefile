NAME=coordinate

INC_DIRS=/usr/local/lib
INC_DIRS+=/usr/bin/local/include

CC=g++
SOURCES=coordinate.cpp

CFLAGS+=-Wall -g -std=gnu++11
CFLAGS+=$(addprefix -I,$(INC_DIRS))

LDLIBS=-L/usr/local/lib/

LDLIBS+=-lrealsense2

OBJ_DIR=obj
OBJ=$(filter %.o,$(SOURCES:%.cpp=$(OBJ_DIR)/%.o))

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ): $(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(NAME)
