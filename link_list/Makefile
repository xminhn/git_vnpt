CC := gcc
#CC = /home/test/buildroot-2015.05/output/host/usr/bin/mips-buildroot-linux-uclibc-gcc

CFLAGS := -g -Wall -Isrc -std=c99

# Thư mục gốc của dự án
PLUS_DIR := .

# Các thư mục con
BIN_DIR := $(PLUS_DIR)/bin
OBJ_DIR := $(PLUS_DIR)/obj
SRC_DIR := $(PLUS_DIR)/src

# Tên file thực thi cuối cùng
TARGET := test_iface

# Tự động tìm tất cả các file .c trong thư mục src
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# Tự động tạo danh sách file .o tương ứng trong thư mục obj
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Quy tắc mặc định: Tạo các thư mục cần thiết và biên dịch tất cả
all: $(OBJ_DIR) $(BIN_DIR) $(OBJ_FILES) $(BIN_DIR)/$(TARGET)

# Quy tắc liên kết cuối cùng: Tạo file thực thi từ các file .o
$(BIN_DIR)/$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@

# Quy tắc biên dịch file .c thành file .o
# Phụ thuộc vào file .c tương ứng
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Quy tắc tạo thư mục bin và obj nếu chúng chưa tồn tại
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Quy tắc dọn dẹp
clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OBJ_DIR)/*

# Đánh dấu các quy tắc không phải là tên file thực
.PHONY: all clean

