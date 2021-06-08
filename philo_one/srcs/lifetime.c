/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifetime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 13:53:18 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 16:04:23 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

unsigned long	get_ts(struct timeval ts)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000)
			- (ts.tv_sec * 1000 + ts.tv_usec / 1000));
}

bool	is_dead(t_philo *philo)
{
	if (*philo->stop == true)
		return (true);
	pthread_mutex_lock(&philo->lunch);
	if (get_time() - philo->last_meal >= philo->pars->tt_die)
	{
		pthread_mutex_unlock(&philo->lunch);
		if (*philo->stop == false)
		{
			*philo->stop = true;
			pthread_mutex_lock(philo->print);
			printf("%lums %zu died\n", get_ts(philo->ts), philo->name);
			pthread_mutex_unlock(philo->print);
		}
		return (true);
	}
	pthread_mutex_unlock(&philo->lunch);
	return (false);
}

bool	fall_asleep(t_philo *philo)
{
	if (*philo->stop == true)
		return (false);
	pthread_mutex_lock(philo->print);
	printf("%lums %zu is sleeping\n", get_ts(philo->ts), philo->name);
	pthread_mutex_unlock(philo->print);
	ft_sleep(philo->pars->tt_sleep);
	if (*philo->stop == true)
		return (false);
	pthread_mutex_lock(philo->print);
	printf("%lums %zu is thinking\n", get_ts(philo->ts), philo->name);
	pthread_mutex_unlock(philo->print);
	return (true);
}

bool	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->lunch);
	philo->last_meal = get_time();
	pthread_mutex_lock(philo->print);
	printf("%lums %zu is eating\n", get_ts(philo->ts), philo->name);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(&philo->lunch);
	ft_sleep(philo->pars->tt_eat);
	philo->nb_meals += 1;
	pthread_mutex_unlock(philo->fork[LEFT]);
	pthread_mutex_unlock(philo->fork[RIGHT]);
	if (*philo->stop == true)
		return (false);
	return (true);
}

void	*live(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	if (philo->name % 2)
		ft_sleep(2);
	while (*philo->stop == false)
	{
		if (!take_forks(philo, philo->fork[LEFT], philo->fork[RIGHT]))
			break ;
		if (!eat(philo))
			break ;
		if (!fall_asleep(philo))
			break ;
	}
	pthread_mutex_destroy(&philo->lunch);
	return (NULL);
}
