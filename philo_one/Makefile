NAME = philo_one

##### SRC & OBJ PATH #####
SRCPATH = .
OBJPATH = $(SRCPATH)/obj

##### INCLUDE #####
PATH_INCLUDE = .
HEADERS = $(PATH_INCLUDE)/*.h
INC = $(addprefix -I , $(PATH_INCLUDE))

##### FLAG #####
CC = clang
FLAGS = -Wall -Wextra -Werror


##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	FLAGS += -D DARWIN
else
	#Linux and others...
	FLAGS += -D LINUX
endif

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, philo_one.c ft_strtoimax.c ft_isspace.c \
	   ft_isdigit.c tools.c initialization.c exit.c ft_strnew.c ft_pow.c \
	   ft_itoa.c ft_strlcpy.c ft_strlen.c ft_strlcat.c sleep.c utils.c \
	   routine.c)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME) : message $(OBJ)
	@echo "\n$(END)$(BLUE)# Making $(NAME) #$(END)$(GREY)"
	$(CC) -o $@ $(OBJ) $(PS_INSTRUCTIONS) $(LIBFT)
	@echo "\n$(END)$(GREEN)# $(NAME) is built #$(END)"

message :
	@echo "\n$(END)$(BLUE)# Making $(NAME) objects #$(END)$(GREY)"


$(OBJPATH)/%.o : $(SRCPATH)/%.c $(HEADERS)
	$(CC) $(FLAGS) $(INC) -c $< -o $@


### CLEAN ###
.PHONY : clean fclean re

clean :
	@echo "$(END)$(RED)# removing $(NAME) objects #$(END)$(GREY)"
	@rm -rf $(OBJ)

fclean : clean
	@echo "$(END)$(RED)\n# removing $(NAME) #$(END)$(GREY)"
	@rm -f $(NAME)

re : fclean all
