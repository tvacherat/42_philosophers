/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 13:02:26 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/08 15:01:33 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

bool	take_forks(t_philo *philo)
{
	if (*philo->stop == true)
		return (false);
	sem_wait(philo->forks);
	if (*philo->stop == true)
		return (false);
	sem_wait(philo->print);
	printf("%lums %zu has taken a fork\n", get_ts(philo->ts), philo->name);
	sem_post(philo->print);
	sem_wait(philo->forks);
	if (*philo->stop == true)
		return (false);
	sem_wait(philo->print);
	printf("%lums %zu has taken a fork\n", get_ts(philo->ts), philo->name);
	sem_post(philo->print);
	return (true);
}

sem_t	*init_forks(t_pars *pars)
{
	sem_t	*forks;

	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT, S_IRWXU, pars->nb_philo);
	return (forks);
}
