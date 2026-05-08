CXX = g++
CXXFLAGS = -std=c++14 $(shell pkg-config --cflags opencv4)
LDFLAGS  = $(shell pkg-config --libs opencv4)

NAME1 = 2412747_kadai5-1
NAME2 = 2412747_kadai5-2
NAME3 = 2412747_kadai5-3
NAME4 = 2412747_kadai5-4
NAME5 = 2412747_kadai5-5
NAME6 = 2412747_kadai5-6

SRC1 = 2412747_kadai5-1.cpp
SRC2 = 2412747_kadai5-2.cpp
SRC3 = 2412747_kadai5-3.cpp
SRC4 = 2412747_kadai5-4.cpp
SRC5 = 2412747_kadai5-5.cpp
SRC6 = 2412747_kadai5-6.cpp
OBJ1 = $(SRC1:%.cpp=%.o)
OBJ2 = $(SRC2:%.cpp=%.o)
OBJ3 = $(SRC3:%.cpp=%.o)
OBJ4 = $(SRC4:%.cpp=%.o)
OBJ5 = $(SRC5:%.cpp=%.o)
OBJ6 = $(SRC6:%.cpp=%.o)

all: $(NAME1) $(NAME2) $(NAME3) $(NAME4) $(NAME5) $(NAME6)

$(NAME1): $(OBJ1)
	$(CXX) $(CXXFLAGS) $(OBJ1) -o $(NAME1) $(LDFLAGS)
$(NAME2): $(OBJ2)
	$(CXX) $(CXXFLAGS) $(OBJ2) -o $(NAME2) $(LDFLAGS)
$(NAME3): $(OBJ3)
	$(CXX) $(CXXFLAGS) $(OBJ3) -o $(NAME3) $(LDFLAGS)
$(NAME4): $(OBJ4)
	$(CXX) $(CXXFLAGS) $(OBJ4) -o $(NAME4) $(LDFLAGS)
$(NAME5): $(OBJ5)
	$(CXX) $(CXXFLAGS) $(OBJ5) -o $(NAME5) $(LDFLAGS)
$(NAME6): $(OBJ6)
	$(CXX) $(CXXFLAGS) $(OBJ6) -o $(NAME6) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

p1: $(NAME1)

p2: $(NAME2)

p3: $(NAME3)

p4: $(NAME4)

p5: $(NAME5)

p6: $(NAME6)

clean: 
	rm -f $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6)

fclean: clean
	rm -f $(NAME1) $(NAME2) $(NAME3) $(NAME4) $(NAME5) $(NAME6)

re: fclean all

.PHONY: all clean fclean re
