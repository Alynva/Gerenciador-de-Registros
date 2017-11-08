# Compilador
CC = g++

# Flags de versao
C_VERSION = -std=c++11

# Arquivo principal
OBJ_FILE = main.cpp

# Arquivos auxiliares
AUX_FILES = 

# Flags do compilador
CC_FLAGS = -g -Wall

# Linkers do compilador
CC_LINKERS = 
ifeq ($(OS),Windows_NT)
	CC_LINKERS += 
endif
CC_LINKERS += 

# Arquivo de saída
ifeq ($(OS),Windows_NT)
	OUT_FILE = ./Ger-Reg.exe
else
	OUT_FILE = ./Ger-Reg.out
endif

all:
	$(CC) $(C_VERSION) $(OBJ_FILE) $(AUX_FILES) $(CC_FLAGS) $(CC_LINKERS) -o $(OUT_FILE)

clean:
	rm $(OUT_FILE)

run:
	$(OUT_FILE)

full: all run