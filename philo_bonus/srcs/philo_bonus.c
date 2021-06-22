/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 12:45:26 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/22 11:17:43 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_children(t_philo *philos, unsigned int nb_philos)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
	i = 0;
	while (i < nb_philos)
	{
		sem_close(philos[i].lunch);
		sem_unlink(philos[i].lunch_name);
		free(philos[i].lunch_name);
		i++;
	}
}

bool	watch_children(t_philo *philos, unsigned int nb_philos)
{
	size_t	nb_full;
	int		status;

	nb_full = 0;
	while (philos->pars->nb_meals > 0 && nb_full != nb_philos)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) != ATE_ENOUGH)
			break ;
		nb_full++;
	}
	if (philos->pars->nb_meals < 0)
		waitpid(-1, &status, 0);
	kill_children(philos, nb_philos);
	if (philos->pars->nb_meals && nb_full == nb_philos)
		printf("Everyone ate well\n");
	sem_close(philos->forks);
	sem_unlink("forks");
	sem_close(philos->sync);
	sem_unlink("sync");
	sem_post(philos->print);
	free(philos);
	return (true);
}

void	process(t_philo *philo)
{
	pthread_t	thread;
	bool		stop;

	stop = false;
	philo->stop = &stop;
	sem_wait(philo->sync);
	philo->last_meal = get_time();
	if (pthread_create(&thread, NULL, &live, philo)
		|| pthread_detach(thread))
		exit(ERR);
	while (42)
	{
		ft_sleep(4);
		if (is_dead(philo))
			exit(IS_DEAD);
		else if (philo->pars->nb_meals > 0
			&& philo->nb_meals >= philo->pars->nb_meals)
			exit(ATE_ENOUGH);
	}	
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
				S_IRWXU | S_IRWXG, 1);
		i++;
	}
	return (true);
}

bool	launch_children(t_pars *pars, sem_t *print)
{
	t_philo			*philos;
	size_t			i;
	sem_t			*sync;
	struct timeval	ts;

	i = 0;
	sync = sem_open("sync", O_CREAT, S_IRWXU | S_IRWXG, 0);
	philos = malloc(sizeof(t_philo) * pars->nb_philo);
	if (!philos || !set_philos(philos, pars, print))
		return (false);
	gettimeofday(&ts, NULL);
	while (i < pars->nb_philo)
	{
		philos[i].sync = sync;
		philos[i].pid = fork();
		philos[i].ts = ts;
		if (!philos[i].pid)
			process(&philos[i]);
		else if (philos[i].pid < 0)
			return (false);
		i++;
	}
	unlock_sync(sync, pars->nb_philo);
	return (watch_children(philos, pars->nb_philo));
}
