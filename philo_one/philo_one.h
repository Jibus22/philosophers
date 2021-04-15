#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/errno.h>

typedef struct		s_env
{
	int				philo_nb;
	int				ttd;
	int				tte;
	int				tts;
}					t_env;

typedef struct		s_philo
{
	t_env			*env;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				id;
	struct timeval	time_start;
	long			timestamp;
}					t_philo;

/*
** init
*/

void				set_struct_null(t_philo **philo, t_env **env,
				pthread_t **thread, pthread_mutex_t **mutex);

int					main_init(t_philo **philo, t_env **env, int ac, char *av[]);

int					thread_mutex_init(t_env *env, pthread_t **thread,
				pthread_mutex_t **mutex);

void				philo_init(t_philo *philo, t_env *env,
				pthread_mutex_t *mutex);

/*
** utils
*/

int					main_exit(t_philo *philo, t_env *env,
				pthread_t *thread, pthread_mutex_t *mutex);

int					ft_strtoimax(const char *str, char **endptr);
int					ft_isspace(int c);
int					ft_isdigit(int c);

/*
** tools
*/

void				print_env(t_env *env);
int					print_thread(t_env *env, pthread_t *thread);

#endif
