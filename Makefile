CC          := ./output/host/bin/arm-buildroot-linux-musleabi-gcc
NAME_PROJECT:= agent_lan
PATH_OUTPUT := ./out
 
INC_DIRS    := ./inc ./dynamic_lib/include
SRC_DIRS    := ./src
LIB_DIRS    := ./output/host/lib ./dynamic_lib/lib
   
LIBS        := -lm -luci -lubox
 
CFLAGS      := $(foreach dir,$(INC_DIRS),-I$(dir))
LDFLAGS     := $(LIBS) \
               $(foreach dir,$(LIB_DIRS),-L$(dir)) \
               $(foreach dir,$(LIB_DIRS),-Wl,-rpath-link=$(dir))
 
SRC_FILES   := $(wildcard $(SRC_DIRS)/*.c)
OBJ_FILES   := $(patsubst $(SRC_DIRS)/%.c,$(PATH_OUTPUT)/%.o,$(SRC_FILES))
 
# Pattern rules
.PHONY: all clean run
 
all: $(PATH_OUTPUT)/$(NAME_PROJECT)
	@echo "[DONE]"
    @echo "Object files: $(OBJ_FILES)"
 
$(PATH_OUTPUT)/$(NAME_PROJECT): $(OBJ_FILES)
	@echo "[LINK] $@"
	$(CC) $^ -o $@ $(LDFLAGS)
 
$(PATH_OUTPUT)/%.o: $(SRC_DIRS)/%.c | $(PATH_OUTPUT)
	@echo "[COMPILING] $< -> $@"
	LD_LIBRARY_PATH=$(firstword $(LIB_DIRS)):$$LD_LIBRARY_PATH $(CC) -c $< -o $@ $(CFLAGS)
 
$(PATH_OUTPUT):
	@mkdir -p $@
 
run: $(PATH_OUTPUT)/$(NAME_PROJECT)
	@echo "[RUNNING] $<"
    @LD_LIBRARY_PATH=$(subst $(space),:,$(LIB_DIRS)) ./$<
 
clean:
	@echo "[CLEAN]"
	rm -rf $(PATH_OUTPUT)/*