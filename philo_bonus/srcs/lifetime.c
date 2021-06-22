/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifetime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 13:53:18 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 17:32:38 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	sem_wait(philo->lunch);
	if (get_time() - philo->last_meal >= philo->pars->tt_die)
	{
		sem_post(philo->lunch);
		if (*philo->stop == false)
		{
			*philo->stop = true;
			sem_wait(philo->print);
			printf("%lums %zu died\n", get_ts(philo->ts), philo->name);
		}
		return (true);
	}
	sem_post(philo->lunch);
	return (false);
}

bool	fall_asleep(t_philo *philo)
{
	if (*philo->stop == true)
		return (false);
	sem_wait(philo->print);
	printf("%lums %zu is sleeping\n", get_ts(philo->ts), philo->name);
	sem_post(philo->print);
	ft_sleep(philo->pars->tt_sleep);
	if (*philo->stop == true)
		return (false);
	sem_wait(philo->print);
	printf("%lums %zu is thinking\n", get_ts(philo->ts), philo->name);
	sem_post(philo->print);
	return (true);
}

bool	eat(t_philo *philo)
{
	sem_wait(philo->lunch);
	philo->last_meal = get_time();
	sem_wait(philo->print);
	printf("%lums %zu is eating\n", get_ts(philo->ts), philo->name);
	sem_post(philo->print);
	sem_post(philo->lunch);
	ft_sleep(philo->pars->tt_eat);
	philo->nb_meals += 1;
	sem_post(philo->forks);
	sem_post(philo->forks);
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
		if (!take_forks(philo))
			break ;
		if (!eat(philo))
			break ;
		if (!fall_asleep(philo))
			break ;
	}
	return (NULL);
}
