NAME = philo_three

##### SRC & OBJ PATH #####
SRCPATH = .
OBJPATH = $(SRCPATH)/obj

##### INCLUDE #####
PATH_INCLUDE = .
HEADERS = $(PATH_INCLUDE)/*.h
INC = $(addprefix -I , $(PATH_INCLUDE))

##### FLAG #####
PTHREAD_LIB = -lpthread

##### COMPILER #####
CC = clang
##### COMPILATION OPTION #####
SANITIZE = -g -fsanitize=address
##### COMPILATION FLAG #####
FLAGS = -Wall -Wextra -Werror
SAN_COMP = -fno-omit-frame-pointer
##### LINKING FLAG #####
SAN_LINK = 


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
SRCS = $(addprefix $(SRCPATH)/, philo_three.c ft_strtoimax.c ft_isspace.c \
	   ft_isdigit.c tools.c initialization.c exit.c ft_strnew.c ft_pow.c \
	   ft_itoa.c ft_strlcpy.c ft_strlen.c ft_strlcat.c sleep.c utils.c \
	   routine.c main_init.c)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME) : message $(OBJ)
	@echo "\n$(END)$(BLUE)# Making $(NAME) #$(END)$(GREY)"
	$(CC) -o $@ $(SAN_LINK) $(OBJ) $(PTHREAD_LIB)
	@echo "\n$(END)$(GREEN)# $(NAME) is built #$(END)"

message :
	@echo "\n$(END)$(BLUE)# Making $(NAME) objects #$(END)$(GREY)"


$(OBJPATH)/%.o : $(SRCPATH)/%.c $(HEADERS)
	$(CC) $(FLAGS) $(INC) -c $< -o $@


### DEBUG ###

sanitize : FLAGS += $(SANITIZE) $(SAN_COMP)
sanitize :  SAN_LINK += $(SANITIZE)
sanitize : all


### CLEAN ###
.PHONY : sanitize clean fclean re

clean :
	@echo "$(END)$(RED)# removing $(NAME) objects #$(END)$(GREY)"
	@rm -rf $(OBJ)

fclean : clean
	@echo "$(END)$(RED)\n# removing $(NAME) #$(END)$(GREY)"
	@rm -f $(NAME)

re : fclean all
