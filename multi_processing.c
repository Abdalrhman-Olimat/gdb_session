#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

typedef struct s_data
{
	sem_t	*sem_print;
	sem_t	*sem_simulation;
	sem_t	*sem_simulation_check;
}	t_data;

void	child_routine(int proc_num, t_data *data)
{
	unsigned long long	counter = 0;
	int	value;
	while (1)
	{
		sem_wait(data->sem_simulation);
		if (sem_getvalue(data->sem_simulation_check, &value))
		{
			perror("sem_getvalue");
			sem_wait(data->sem_simulation_check);
			sem_post(data->sem_simulation);
			return ;
		}
		if (!value)
		{
			sem_post(data->sem_simulation);
			return ;
		}
		sem_post(data->sem_simulation);
		sem_wait(data->sem_print);
		printf("proc_num: %d\nCounter: %llu\n", proc_num, counter);
		counter++;
		sem_post(data->sem_print);
		usleep(100000);
	}
}

void	close_sem(t_data *data, int mode)
{
	if (mode >= 0)
	{
		sem_close(data->sem_print);
		sem_close(data->sem_simulation);
		sem_close(data->sem_simulation_check);
	}
	if (mode >= 1)
	{
		sem_unlink("TEST_PRINT");
		sem_unlink("TEST_SIMULATION");
		sem_unlink("TEST_SIMULATION_CHECK");
	}
}

int	init_sem(t_data *data)
{
	data->sem_print = sem_open("TEST_PRINT", O_CREAT | O_EXCL, S_IRWXU, 1);
	if (data->sem_print == SEM_FAILED)
	{
		perror("sem_open");
		return (0);
	}
	data->sem_simulation = sem_open("TEST_SIMULATION", O_CREAT | O_EXCL, S_IRWXU, 1);
	if (data->sem_simulation == SEM_FAILED)
	{
		perror("sem_open");
		sem_close(data->sem_print);
		sem_unlink("TEST_PRINT");
		return (0);
	}
	data->sem_simulation_check = sem_open("TEST_SIMULATION_CHECK", O_CREAT | O_EXCL, S_IRWXU, 1);
	if (data->sem_simulation == SEM_FAILED)
	{
		perror("sem_open");
		sem_close(data->sem_print);
		sem_close(data->sem_simulation);
		sem_unlink("TEST_PRINT");
		sem_unlink("TEST_SIMULATION");
		return (0);
	}
	return (1);
}

int main()
{
	t_data	data;
	
	if (!init_sem(&data))
		return (0);
	int pid_x = fork();
	if (pid_x == -1)
	{
		close_sem(&data, 1);
		return (1);
	}
	if (!pid_x)
	{
		child_routine(1, &data);
		close_sem(&data, 0);
		return (0);
	}
	int pid_y = fork();
	if (pid_y == -1)
	{
		kill(pid_x, SIGKILL);
		close_sem(&data, 1);
		return (1);
	}
	if (!pid_y)
	{
		child_routine(2, &data);
		close_sem(&data, 0);
		return (0);
	}

	char buffer[1];
	read(STDIN_FILENO, buffer, 1);
	sem_wait(data.sem_simulation_check);

	while (wait(0) != -1)
		;
	close_sem(&data, 1);
	return (0);
}