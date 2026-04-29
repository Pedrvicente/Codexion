NAME = codexion
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread
SRCDIR = coders

SRCS = $(SRCDIR)/main.c $(SRCDIR)/parsing.c $(SRCDIR)/monitor.c \
       $(SRCDIR)/queue.c $(SRCDIR)/coder.c $(SRCDIR)/errors.c \
       $(SRCDIR)/init.c $(SRCDIR)/simulation.c $(SRCDIR)/timer.c

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
