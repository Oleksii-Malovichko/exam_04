NAME = argo

SRCS = argo.c learn2.c

CC = cc

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(SRCS) -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: clean all