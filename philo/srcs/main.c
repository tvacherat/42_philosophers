/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 12:07:06 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/03 19:00:02 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_pars			pars;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	int				ret;

	ret = 0;
	if (argc < 5 || argc > 6 || !check_args(argc, argv, &pars))
	{
		write(2, "Wrong arguments\n", 16);
		return (1);
	}
	if (!pars.nb_meals)
	{
		printf("Everyone ate well, even though no one ate\n");
		return (0);
	}
	forks = init_forks(&pars);
	pthread_mutex_init(&print, NULL);
	if (!forks || !launch_threads(&pars, forks, &print))
		ret = 1;
	destroy_forks(forks, &pars);
	pthread_mutex_destroy(&print);
	return (ret);
}
