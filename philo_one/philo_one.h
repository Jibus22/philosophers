#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <sys/errno.h>

/*
** philo state
*/

# define TAKE_FORK 1
# define EATING 2
# define SLEEPING 3
# define DIED 4
# define THINKING 5

# define STAT_BUF_LEN 30

# define ALL_ALIVE 1
# define DEAD 0
# define NOT_YET 1

/*
** forks state
*/

# define AVAILABLE 1
# define USED 2


/*
** philo data structures
*/

typedef struct		s_env
{
	int				philo_nb;
	int				ttd;
	int				tte;
	int				tts;
	int				living;
	char			*forks;
	pthread_mutex_t	mtx_output;
}					t_env;

typedef struct		s_philo
{
	t_env			*env;
	pthread_mutex_t	*mtx_rfork;
	pthread_mutex_t	*mtx_lfork;
	char			*right_fork;
	char			*left_fork;
	int				id;
	char			str_id[6];
	int				state;
	char			status_buf[30];
	struct timeval	time_start;
	long			last_lunch;
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
** run
*/

void				*routine(void *arg);
int					sleep_but_listen(t_philo *philo, int duration);
int					am_i_dead(t_philo *philo);

void				print_new_status(t_philo *philo, char *id,
				int msg_code, char *buf);
long				get_timestamp(struct timeval tv_start);

/*
** utils
*/

int					main_exit(t_philo *philo, t_env *env,
				pthread_t *thread, pthread_mutex_t *mutex);

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
