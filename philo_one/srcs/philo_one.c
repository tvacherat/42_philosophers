/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 15:18:19 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 16:01:52 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

bool	check_meals(t_philo *philos, unsigned int nb_philos, long nb_meals)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		if (philos[i].nb_meals < nb_meals)
			return (false);
		i++;
	}
	return (true);
}

bool	check_death(t_philo *philos, unsigned int nb_philos)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		if (is_dead(&philos[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	watch_threads(t_pars *pars, t_philo *philos, unsigned int nb_philos,
			pthread_mutex_t *forks)
{
	size_t	i;

	i = 0;
	while (*philos->stop == false)
	{
		ft_sleep(8);
		check_death(philos, nb_philos);
		if (*philos->stop == false && pars->nb_meals > 0
			&& check_meals(philos, nb_philos, pars->nb_meals))
		{
			*philos->stop = true;
			ft_sleep(5);
			printf("Everyone ate well\n");
		}
	}
	while (i < nb_philos)
	{
		if (i == 0 && *philos->stop == true)
			unlock_forks(forks, nb_philos);
		if (pthread_join(philos[i].thread, NULL))
			return (false);
		i++;
	}
	free(philos);
	return (true);
}

void	set_philos(t_philo *philos, t_pars *pars, pthread_mutex_t *forks,
			pthread_mutex_t *print)
{
	size_t	i;

	i = 0;
	while (i < pars->nb_philo)
	{
		philos[i].name = i + 1;
		philos[i].nb_meals = 0;
		philos[i].pars = pars;
		philos[i].print = print;
		pthread_mutex_init(&philos[i].lunch, NULL);
		get_forks(&philos[i], i, forks, pars->nb_philo);
		i++;
	}
}

bool	launch_threads(t_pars *pars, pthread_mutex_t *forks,
			pthread_mutex_t *print)
{
	t_philo			*philos;
	size_t			i;
	struct timeval	timestamp;
	bool			stop;

	i = 0;
	stop = false;
	philos = malloc(sizeof(t_philo) * pars->nb_philo);
	if (!philos)
		return (false);
	set_philos(philos, pars, forks, print);
	gettimeofday(&timestamp, NULL);
	while (i < pars->nb_philo)
	{
		philos[i].stop = &stop;
		philos[i].ts = timestamp;
		philos[i].last_meal = get_time();
		if (pthread_create(&philos[i].thread, NULL, &live, &philos[i]))
			return (false);
		i++;
	}
	return (watch_threads(pars, philos, pars->nb_philo, forks));
}
