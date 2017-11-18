####### Compiler and options

CC       = g++
DEL_FILE = rm -f
FFLAGS   = -fopenmp

####### Files

FILE_NAME = openMPmm
SOURCES = $(FILE_NAME).cpp
TARGET   = a.out
TAR_FILE = $(FILE_NAME)Tar.tar

####### Build rules

all: $(TARGET)

$(TARGET):  $(SOURCES)
	$(CC) -o $(TARGET) $^ $(FFLAGS) 

clean:
	$(DEL_FILE) $(TARGET) $(TAR_FILE)

tar: $(TAR_FILE)

$(TAR_FILE): $(TARGET)
	@echo "Making tar file ..."
	@tar -cf $(TAR_FILE) $(SOURCES) Makefile $^

testBed: $(TAR_FILE)
	@testBed cs345/openmp $^

timeTest: $(TARGET)
	@time $^ 1
	@echo
	@time $^ 2
	@echo
	@time $^ 3
	@echo
	@time $^ 4

submit: $(TAR_FILE)
	@/mnt/local/submit/submit.pl $^

