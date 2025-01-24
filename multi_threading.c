#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_data
{
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*simulation_mutex;
	int				simulation_status;
}	t_data;

typedef struct s_tdata
{
	t_data	*data;
	int		t_num;
}	t_tdata;

int	check_simulation_status(t_data *data)
{
	int	res;
	pthread_mutex_lock(data->simulation_mutex);
	res = data->simulation_status;
	pthread_mutex_unlock(data->simulation_mutex);
	return (res);
}

void	*thread_routine(t_tdata *tdata)
{
	t_data	*data = tdata->data;
	unsigned long long	counter = 0;
	int	t_num = tdata->t_num;
	while (1)
	{
		if (!check_simulation_status(data))
			break;
		pthread_mutex_lock(data->print_mutex);
		printf("T_NUM: %d\nCounter: %llu\n", t_num, counter);
		counter++;
		pthread_mutex_unlock(data->print_mutex);
		usleep(100000);
	}
	return (0);
}

void	set_simulation_status(t_data *data, int value)
{
	pthread_mutex_lock(data->simulation_mutex);
	data->simulation_status = value;
	pthread_mutex_unlock(data->simulation_mutex);
}

void	destroy_mutexes(t_data *data)
{
	if (data->print_mutex)
	{
		pthread_mutex_destroy(data->print_mutex);
		free(data->print_mutex);
	}
	if (data->simulation_mutex)
	{
		pthread_mutex_destroy(data->simulation_mutex);
		free(data->simulation_mutex);
	}
}

int	alloc_mutexes(t_data *data)
{
	data->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->print_mutex)
		return (0);
	data->simulation_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->simulation_mutex)
	{
		destroy_mutexes(data);
		return (0);
	}
	return (1);
}

int	init_mutexes(t_data *data)
{
	if (!alloc_mutexes(data))
		return (0);
	if (pthread_mutex_init(data->print_mutex, 0))
	{
		destroy_mutexes(data);
		return (0);
	}
	if (pthread_mutex_init(data->simulation_mutex, 0))
	{
		destroy_mutexes(data);
		return (0);
	}
	return (1);
}

void	tdata_init(t_data *data,
	t_tdata *tdata_x, t_tdata *tdata_y)
{
	tdata_x->data = data;
	tdata_y->data = data;
	tdata_x->t_num = 1;
	tdata_y->t_num = 2;
}

int main()
{
	t_data	data;
	t_tdata	tdata_x;
	t_tdata	tdata_y;
	bzero(&data, sizeof(t_data));
	tdata_init(&data, &tdata_x, &tdata_y);
	if (!init_mutexes(&data))
	{
		perror("init_mutexes");
		return (1);
	}
	pthread_t thread_x;
	data.simulation_status = 1;
	if (pthread_create(&thread_x, 0, (void *)thread_routine, &tdata_x))
	{
		perror("pthread_create");
		destroy_mutexes(&data);
		return (1);
	}
	pthread_t thread_y;
	if (pthread_create(&thread_y, 0, (void *)thread_routine, &tdata_y))
	{
		pthread_mutex_lock(data.print_mutex);
		perror("pthread_create");
		pthread_mutex_unlock(data.print_mutex);
		set_simulation_status(&data, 0);
		pthread_join(thread_x, 0);
		destroy_mutexes(&data);
		return (1);
	}
	char buffer[1];
	read(STDIN_FILENO, buffer, 1);
	set_simulation_status(&data, 0);
	pthread_join(thread_x, 0);
	pthread_join(thread_y, 0);
	destroy_mutexes(&data);
	return (0);
}