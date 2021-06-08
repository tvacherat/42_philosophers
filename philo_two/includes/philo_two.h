/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 11:26:32 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 16:08:19 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdbool.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>

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
	sem_t			*forks;
	sem_t			*print;
	sem_t			*lunch;
	char			*lunch_name;
	size_t			last_meal;
	struct timeval	ts;
	pthread_t		thread;
}	t_philo;

/*
**	main.c
*/
int				main(int argc, char **argv);

/*
** utils.c
*/
bool			check_meals(t_philo *philos, unsigned int nb_philos,
					long nb_meals);
bool			check_death(t_philo *philos, unsigned int nb_philos);
size_t			get_time(void);
void			ft_sleep(size_t ms);

/*
**	philo_two.c
*/
bool			join_threads(t_philo *philos, unsigned int nb_philos);
bool			watch_threads(t_pars *pars, t_philo *philos,
					unsigned int nb_philos);
bool			set_philos(t_philo *philos, t_pars *pars, sem_t *print);
bool			launch_threads(t_pars *pars, sem_t *print);

/*
**	check.c
*/
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strlen(const char *str);
bool			only_numbers(char *str);
unsigned int	pars_nbr(char *nbr);
bool			check_args(int argc, char **argv, t_pars *pars);

/*
**	forks.c
*/
bool			take_forks(t_philo *philo);
sem_t			*init_forks(t_pars *pars);

/*
**	lifetime.c
*/
unsigned long	get_ts(struct timeval ts);
bool			is_dead(t_philo *philo);
bool			fall_asleep(t_philo *philo);
bool			eat(t_philo *philo);
void			*live(void *arg);

/*
**	ft_strjoin.c
*/
char			*ft_strjoin(char const *s1, char const *s2);

/*
**	ft_iota.c
*/
char			*ft_itoa(int n);

#endif
