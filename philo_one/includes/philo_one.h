/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 12:06:31 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 16:00:44 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdbool.h>

# define LEFT 0
# define RIGHT 1

typedef struct s_pars
{
	unsigned int	nb_philo;
	unsigned int	tt_die;
	unsigned int	tt_eat;
	unsigned int	tt_sleep;
	long			nb_meals;
}	t_pars;

typedef struct s_philo
{
	size_t			name;
	bool			*stop;
	long			nb_meals;
	t_pars			*pars;
	pthread_mutex_t	*fork[2];
	pthread_mutex_t	*print;
	pthread_mutex_t	lunch;
	size_t			last_meal;
	struct timeval	ts;
	pthread_t		thread;
}	t_philo;

/*
**	main.c
*/
int				main(int argc, char **argv);

/*
** time.c
*/
size_t			get_time(void);
void			ft_sleep(size_t ms);

/*
**	philo_one.c
*/
bool			check_death(t_philo *philos, unsigned int nb_philos);
bool			check_meals(t_philo *philos, unsigned int nb_philos,
					long nb_meals);
void			set_philos(t_philo *philos, t_pars *pars,
					pthread_mutex_t *forks, pthread_mutex_t *print);
bool			watch_threads(t_pars *pars, t_philo *philos,
					unsigned int nb_philos, pthread_mutex_t *forks);
bool			launch_threads(t_pars *pars, pthread_mutex_t *forks,
					pthread_mutex_t *print);

/*
**	check.c
*/
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strlen(char *str);
bool			only_numbers(char *str);
unsigned int	pars_nbr(char *nbr);
bool			check_args(int argc, char **argv, t_pars *pars);

/*
**	forks.c
*/
void			unlock_forks(pthread_mutex_t *forks, unsigned int nb_philos);
bool			take_forks(t_philo *philo, pthread_mutex_t *forkl,
					pthread_mutex_t *forkr);
pthread_mutex_t	*init_forks(t_pars *pars);
void			destroy_forks(pthread_mutex_t *forks, t_pars *pars);
void			get_forks(t_philo *philo, size_t index, pthread_mutex_t *forks,
					unsigned int nb_philo);

/*
**	lifetime.c
*/
unsigned long	get_ts(struct timeval ts);
bool			is_dead(t_philo *philo);
bool			fall_asleep(t_philo *philo);
bool			eat(t_philo *philo);
void			*live(void *arg);

#endif
