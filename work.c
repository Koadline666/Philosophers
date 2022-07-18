/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 15:58:12 by afenzl            #+#    #+#             */
/*   Updated: 2022/07/18 16:54:01 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_feedback(philo, 'r');
	if (philo->data->amount_phil == 1)
	{
		sleep_ms(philo->limit - get_current_time_ms());
		pthread_mutex_unlock(philo->left_fork);
		check_if_dead(philo);
		philo->data->death = true;
		return (1);
	}
	pthread_mutex_lock(philo->right_fork);
	print_feedback(philo, 'r');
	return (0);
}

void	take_forks_and_eat(t_philo *philo)
{
	if (take_forks(philo) == 1)
		return ;
	print_feedback(philo, 'e');
	philo->times_eaten++;
	sleep_ms(philo->data->time_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	take_nap(t_philo *philo)
{
	print_feedback(philo, 's');
	if (philo->limit < get_current_time_ms() + philo->data->time_sleep)
	{
		printf("heyy\n");
		sleep_ms(philo->data->time_sleep);
		check_if_dead(philo);
		return ;
	}
	sleep_ms(philo->data->time_sleep);
}

void	*work(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	// sleep_ms(philo->number * 1000);
	// if (pthread_mutex_lock(&(philo->data->wait_to_start)) != 0)
	// 	printf("he(%i) couldnt lock wait to start\n", philo->number);
	print_feedback(philo, 't');
	if ((philo->number & 1) == 1)
		sleep_ms(5);
	philo->limit = get_current_time_ms() + philo->data->time_die;
	while (true)
	{
		take_forks_and_eat(philo);
		if (check_if_dead(philo) == 1)
			break ;
		take_nap(philo);
		if (check_if_dead(philo) == 1)
			break ;
		print_feedback(philo, 't');
		philo->limit = get_current_time_ms() + philo->data->time_die;
	}
	return (data);
}