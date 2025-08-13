CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

TARGET = bot

SRC = main.cpp discordlogic.cpp envlogic.cpp randomlogic.cpp sqllogic.cpp

INCLUDES = -Imysql/include
LIBPATHS = -Lmysql/lib64

LIBS = -ldpp -lmysqlcppconn

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPATHS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
