# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/16 19:38:50 by mhurtamo          #+#    #+#              #
#    Updated: 2025/02/16 20:01:25 by mhurtamo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= main.c atoi.c err_handler.c init.c monitor.c philo.c sim_utils.c validator.c handle_forks.c

HEAD	= ./

OBJS	= ${SRCS:.c=.o}

NAME	= philo

RM	= rm -f

CC	= cc

CFLAGS	= -Wall -Wextra -Werror -pthread

all:	${NAME}

c.o: ${CC} ${CFLAGS} -c -I ${HEAD} $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	
clean:
	rm -f ${OBJS}

fclean:	clean
	rm -f ${NAME}

re:	fclean all clean

.PHONY:	all clean fclean re
