/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 14:05:57 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/03 18:58:01 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	only_numbers(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < 48 || str[i] > 57)
			return (false);
		i++;
	}
	return (true);
}

unsigned int	pars_nbr(char *str)
{
	size_t			i;
	unsigned int	nbr;

	i = 0;
	nbr = 0;
	while (str[i])
	{
		nbr *= 10;
		nbr += str[i] - 48;
		i++;
	}
	return (nbr);
}

bool	check_args(int argc, char **argv, t_pars *pars)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_strlen(argv[i]) > 11 || !only_numbers(argv[i])
			|| (ft_strlen(argv[i]) == 11
				&& ft_strcmp(argv[i], "42949667295") > 0))
			return (false);
		i++;
	}
	pars->nb_philo = pars_nbr(argv[1]);
	pars->tt_die = pars_nbr(argv[2]);
	pars->tt_eat = pars_nbr(argv[3]);
	pars->tt_sleep = pars_nbr(argv[4]);
	if (argc == 6)
		pars->nb_meals = pars_nbr(argv[5]);
	else
		pars->nb_meals = -1;
	if (!pars->nb_philo)
		return (false);
	return (true);
}
