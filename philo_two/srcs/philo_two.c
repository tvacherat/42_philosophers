/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 12:06:36 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 17:21:14 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

bool	join_threads(t_philo *philos, unsigned int nb_philos)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		if (*philos->stop == true)
			sem_post(philos->forks);
		if (pthread_join(philos[i].thread, NULL))
			return (false);
		i++;
	}
	sem_close(philos->forks);
	sem_unlink("forks");
	free(philos);
	return (true);
}

bool	watch_threads(t_pars *pars, t_philo *philos, unsigned int nb_philos)
{
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
	return (join_threads(philos, nb_philos));
}

bool	set_philos(t_philo *philos, t_pars *pars, sem_t *print)
{
	size_t	i;
	sem_t	*forks;
	char	*concat;

	i = 0;
	forks = init_forks(pars);
	if (!forks)
		return (false);
	while (i < pars->nb_philo)
	{
		philos[i].name = i + 1;
		philos[i].nb_meals = 0;
		philos[i].pars = pars;
		philos[i].print = print;
		philos[i].forks = forks;
		concat = ft_itoa(i);
		philos[i].lunch_name = ft_strjoin("lunch_", concat);
		free(concat);
		sem_unlink(philos[i].lunch_name);
		philos[i].lunch = sem_open(philos[i].lunch_name, O_CREAT,
				S_IRWXU, 1);
		i++;
	}
	return (true);
}

bool	launch_threads(t_pars *pars, sem_t *print)
{
	t_philo			*philos;
	size_t			i;
	struct timeval	timestamp;
	bool			stop;

	i = 0;
	stop = false;
	philos = malloc(sizeof(t_philo) * pars->nb_philo);
	if (!philos || !set_philos(philos, pars, print))
		return (false);
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
	return (watch_threads(pars, philos, pars->nb_philo));
}
