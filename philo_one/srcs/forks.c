/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 13:02:26 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/08 15:00:33 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	unlock_forks(pthread_mutex_t *forks, unsigned int nb_philos)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		pthread_mutex_unlock(&forks[i]);
		i++;
	}
}

bool	take_forks(t_philo *philo, pthread_mutex_t *forkl,
			pthread_mutex_t *forkr)
{
	if (*philo->stop == true)
		return (false);
	pthread_mutex_lock(forkl);
	if (*philo->stop == true)
		return (false);
	pthread_mutex_lock(philo->print);
	printf("%lums %zu has taken a fork\n", get_ts(philo->ts), philo->name);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_lock(forkr);
	if (*philo->stop == true)
		return (false);
	pthread_mutex_lock(philo->print);
	printf("%lums %zu has taken a fork\n", get_ts(philo->ts), philo->name);
	pthread_mutex_unlock(philo->print);
	return (true);
}

pthread_mutex_t	*init_forks(t_pars *pars)
{
	pthread_mutex_t	*forks;
	size_t			i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * pars->nb_philo);
	if (!forks)
		return (0);
	while (i < pars->nb_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

void	destroy_forks(pthread_mutex_t *forks, t_pars *pars)
{
	size_t	i;

	i = 0;
	while (i < pars->nb_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
}

void	get_forks(t_philo *philo, size_t index, pthread_mutex_t *forks,
				unsigned int nb_philo)
{
	if (nb_philo == 1)
	{
		philo->fork[LEFT] = &forks[0];
		philo->fork[RIGHT] = &forks[0];
	}
	else if (index == 0)
	{
		philo->fork[LEFT] = &forks[nb_philo - 1];
		philo->fork[RIGHT] = &forks[0];
	}
	else
	{
		philo->fork[LEFT] = &forks[index - 1];
		philo->fork[RIGHT] = &forks[index];
	}
}
