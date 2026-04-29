NAME = codexion
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c parsing.c monitor.c queue.c coder.c \
       errors.c init.c simulation.c timer.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
