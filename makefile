PRE     = stdafx
CC      = g++
CFLAGS  = -Wall -std=c++14
LDFLAGS = -lwiringPi
LIBS    = 
INCLUDE = -I ./sources
SRC_DIR = ./sources
OBJ_DIR = ./objects
SOURCES = $(shell ls $(SRC_DIR)/*.cpp) 
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
TARGET  = controller
DEPENDS = $(OBJS:.o=.d)
MKDIR   = \
	@if [ ! -d $(OBJ_DIR) ]; \
		then echo "mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR); \
	fi

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/$(PRE).h.gch
	$(MKDIR)
	$(CC) -MMD -MP $(CFLAGS) $(INCLUDE) -include $(PRE).h -o $@ -c $< 

$(SRC_DIR)/$(PRE).h.gch: $(SRC_DIR)/$(PRE).h
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS) $(SRC_DIR)/$(PRE).h.gch

-include $(DEPENDS)

.PHONY: all clean
