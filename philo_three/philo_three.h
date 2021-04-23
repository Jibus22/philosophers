/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 14:38:48 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/23 17:13:46 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <signal.h>

/*
** philo state
*/

# define TAKE_FORK 1
# define EATING 2
# define SLEEPING 3
# define DIED 4
# define THINKING 5

# define ALL_ALIVE 21
# define DEAD 42
# define NOT_YET 1

/*
** forks state
*/

# define AVAILABLE 1
# define USED 2

/*
** mutex handling
*/

# define LOCK 1
# define UNLOCK 0

/*
** other
*/

# define FAIL_INIT -1
# define FAIL_DESTROY -2
# define STAT_BUF_LEN 30
# define CONTINUE 4242
# define SUCCESS 1
# define FAIL -1

/*
** philo data structures
*/

typedef struct		s_env
{
	int				philo_nb;
	int				ttd;
	int				tte;
	int				tts;
	int				max_meal;
	int				fork_lock;
	pid_t			*c_pid;
	char			sem_forks_name[30];
	sem_t			*sem_forks;
	char			sem_handle_forks_name[30];
	sem_t			*sem_handle_forks;
	char			sem_output_name[30];
	sem_t			*sem_output;
}					t_env;

typedef struct		s_philo
{
	t_env			*env;
	int				id;
	char			str_id[6];
	int				state;
	struct timeval	time_start;
	long			last_lunch;
	int				meal_nb;
	sem_t			*sem_forks;
}					t_philo;

/*
** init
*/

void				set_struct_null(t_philo **philo, t_env **env);
int					main_init(t_philo **philo, t_env **env, int ac, char *av[]);

int					philo_init(t_philo *philo, t_env *env);

/*
** run
*/

int					routine(t_philo *philo);
int					sleep_but_listen(t_philo *philo, int duration);
int					am_i_dead(t_philo *philo);

int					print_new_status(t_philo *philo, char *id, char *status);
long				get_timestamp(struct timeval tv_start);
void				*dead_snitch(void *arg);

/*
** utils
*/

int					destroy_exit(t_philo *philo, t_env *env);
int					mtx_handler(int code, pthread_mutex_t *mtx, int *living);

size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t				ft_strlcat(char *dst, const char *src, size_t dstsize);
int					ft_strtoimax(const char *str, char **endptr);
int					ft_isspace(int c);
int					ft_isdigit(int c);
char				*ft_itoa(int n);
long				ft_pow(int base, int power);
char				*ft_strnew(int size);
size_t				ft_strlen(const char *s);

/*
** tools
*/

void				print_env(t_env *env);
int					print_thread(t_env *env, pthread_t *thread);

#endif
