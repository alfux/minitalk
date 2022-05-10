SRCC	= client.c

SRCS	= server.c

SPATHC	= srcc/

SPATHS	= srcs/

LPATH	= libft/

LIBFT	= $(LPATH)libft.a

OPATHC	= objc/

OPATHS	= objs/

HPATH	= headers/

OBJC	= ${SRCC:%.c=$(OPATHC)%.o}

OBJS	= ${SRCS:%.c=$(OPATHS)%.o}

CLIENT	= client

SERVER	= server

OPTION	= -Wall -Wextra -Werror -I$(LPATH) -I$(HPATH)

GREEN	= \033[32m

WHITE	= \033[0m

RED		= \033[31m

GREY	= \033[90m

SILDIR	= --no-print-directory

all				:	$(CLIENT) $(SERVER)

$(CLIENT)		:	$(LIBFT) $(OBJC)
					@gcc $(OPTION) $(OBJC) $(LIBFT) -o $@
					@echo "$(GREEN)Client compiled.$(WHITE)"

$(SERVER)		:	$(LIBFT) $(OBJS)
					@gcc $(OPTION) $(OBJS) $(LIBFT) -o $@
					@echo "$(GREEN)Server compiled.$(WHITE)"

$(OPATHC)%.o	:	$(SPATHC)%.c $(OPATHC)
					@gcc -c $(OPTION) $< -o $@
					@echo "$(GREY)$@ created.$(WHITE)"

$(OPATHS)%.o	:	$(SPATHS)%.c $(OPATHS)
					@gcc -c $(OPTION) $< -o $@
					@echo "$(GREY)$@ created.$(WHITE)"

$(OPATHC)		:
					@mkdir $(OPATHC)
					@echo "$(GREY)$(OPATHC) directory created.$(WHITE)"

$(OPATHS)		:
					@mkdir $(OPATHS)
					@echo "$(GREY)$(OPATHS) directory created.$(WHITE)"

$(LIBFT)		:
					@(cd $(LPATH) && $(MAKE) bonus $(SILDIR))
					@(echo "$(GREEN)Libft compiled$(WHITE)")

clean			:
					@(rm -rf $(OPATHC) $(OPATHS))
					@(cd $(LPATH) && $(MAKE) $(SILDIR) $@)
					@(echo "$(RED)Object files removed.$(WHITE)")

fclean			:	clean
					@(rm -rf $(CLIENT) $(SERVER))
					@(cd $(LPATH) && $(MAKE) $(SILDIR) $@)
					@(echo "$(RED)Libft, $(CLIENT) and $(SERVER) removed.$(WHITE)")

re				: fclean all

.PHONY			: all clean fclean re
