NAME	=	libft.a
CC	=	cc
CFLAGS	=	-Wall	-Werror	-Wextra

SRCS	=	ft_atoi.c	ft_bzero.c	ft_calloc.c	ft_isalnum.c	ft_isalpha.c	\
	ft_isascii.c	ft_isdigit.c	ft_isprint.c	ft_itoa.c	ft_memchr.c	\
	ft_memcmp.c	ft_memcpy.c	ft_memmove.c	ft_memset.c	ft_putchar_fd.c	\
	ft_putendl_fd.c	ft_putnbr_fd.c	ft_putstr_fd.c	ft_split.c	ft_strchr.c	\
	ft_strdup.c	ft_striteri.c	ft_strjoin.c	ft_strlcat.c	ft_strlcpy.c	\
	ft_strlen.c	ft_strmapi.c	ft_strncmp.c	ft_strnstr.c	ft_substr.c	\
	ft_strrchr.c	ft_strtrim.c	ft_tolower.c	ft_toupper.c

OBJ	=	$(SRCS:.c=.o)

BONUS_SRCS	=	ft_lstnew.c	ft_lstsize.c	ft_lstadd_back.c	ft_lstlast.c	ft_lstadd_front.c	\
	ft_lstdelone.c	ft_lstclear.c	ft_lstiter.c	ft_lstmap.c

BONUS_OBJ	=	$(BONUS_SRCS:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	ar	rcs	$(NAME)	$(OBJ)
#making	archive	

$(OBJ):	%.o:	%.c
	$(CC)	$(CFLAGS)	-c	$<	-o	$@
#Compiling and saying make object file not executable

bonus:	$(BONUS_OBJ)
	ar	rcs	$(NAME)	$(BONUS_OBJ)
#make rule bonus. create update archive with also bonus if "make bonus"

$(BONUS_OBJ):	%.o:	%.c
	$(CC)	-c	$(CFLAGS)	$<	-o	$@

clean:
	rm	-f	$(OBJ)	$(BONUS_OBJ)
#remove only objet files

fclean:	clean
	rm	-f	$(NAME)
#remove archive

re:	fclean	all
# run fclean

.PHONY:	all	clean	clean_bonus	fclean	re	bonus
#if you have files with these names then still run the command
#"-o" naming it
