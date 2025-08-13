NAME = ft_popen

SRCS = ft_popen2.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

CC = cc

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(SRCS) -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: clean all