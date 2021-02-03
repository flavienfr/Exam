#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>// fd_set, fd_set
#include <stdlib.h>//Linux
#include <stdio.h>//Linux

int g_id = 0; 

typedef struct	s_c{
	int					fd;
	int					id;
	char				*recvBuffer;
	struct s_c			*next;
}				t_c;

typedef struct	s_info{
	int		sock;
	int		fd_max;
	fd_set	select;
	fd_set	save;
	t_c		*clients;
}				t_info;

// Function utils
int extract_message(char **buf, char **msg){
	char	*newbuf;
	int		i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add){
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	if (buf)// Ajout free non null
		free(buf);
	strcat(newbuf, add);
	return (newbuf);
}
//*********************

void exit_error(char *str, t_info *info){
	t_c *client;
	t_c *next;

	write(2, str, strlen(str));
	if (info){
		client = info->clients;
		FD_CLR(info->sock, &info->save);
		close(info->sock);
		while (client){
			next = client->next;
			FD_CLR(client->fd, &info->save);
			close(client->fd);
			if (client->recvBuffer)
				free(client->recvBuffer);
			free(client);
			client = next;
		}
	}
	exit(1);
}

// List functions
int add_client_to_list(t_info *info, int fd){
	t_c *new;
	t_c *client = info->clients;

	if ( !(new = malloc(sizeof(t_c))) )
		exit_error("Fatal error add_client\n", info);

	new->fd = fd;
	new->id = g_id++;
	new->recvBuffer = NULL;
	new->next = NULL;

	if (!client){
		info->clients = new;
		return (new->id);
	}
	while (client->next)
		client = client->next;
	client->next = new;
	
	return (new->id);
}

void remove_client_to_list(t_info *info, int fd){
	t_c *bef = info->clients;
	t_c *cli = bef->next;

	FD_CLR(fd, &info->save);
	close(fd);

	if (bef->fd == fd) {
		info->clients = cli;
		if (bef->recvBuffer)
			free(bef->recvBuffer);
		free(bef);
		return ;
	}
	while (cli)
	{
		if (cli->fd == fd)
		{
			bef->next = cli->next;
			if (cli->recvBuffer)
				free(cli->recvBuffer);
			free(cli);
			return ;
		}
		bef = cli;
		cli = cli->next;
	}
}
//*********************

void init_server(t_info *info, int port){
	struct sockaddr_in servaddr;

	// socket create and verification 
	info->sock = socket(AF_INET, SOCK_STREAM, 0); 
	if (info->sock == -1)
		exit_error("Fatal error socket\n", info);

	//init info
    info->fd_max = info->sock;
    FD_ZERO(&info->save);
    FD_ZERO(&info->select);
    FD_SET(info->sock, &info->save);
    info->clients = NULL;

	// assign IP, PORT 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(info->sock, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		exit_error("Fatal error bind\n", info);

	if (listen(info->sock, 10) != 0)
		exit_error(" listen\n", info);
}

void send_to_clients(t_info *info, char *msg, int fromFd){
	t_c *cli = info->clients;
	int len = strlen(msg);

	while (cli){
		if(cli->fd != fromFd && cli->fd != info->sock){
			int sent = 0;
			int ret = 0;
			while (sent < len){
				ret = send(cli->fd, msg + sent, len - sent, MSG_DONTWAIT);
				sent += ret;
				if (ret <= 0)
					break ;
			}
		}
		cli = cli->next;
	}
}

void new_client(t_info *info){
	struct sockaddr_in cli;
	int connfd;
	unsigned int len;
	char msg[50];
	int id;

	len = sizeof(cli);
	connfd = accept(info->sock, (struct sockaddr *)&cli, &len);
	if (connfd < 0)
		exit_error("Fatal error accept\n", info);

	id = add_client_to_list(info, connfd);
	sprintf(msg, "server: client %d just arrived\n", id);
	send_to_clients(info, msg, connfd);

	FD_SET(connfd, &info->save);
	if (connfd > info->fd_max)
		info->fd_max = connfd;
}

void recv_msg(t_c *cli, t_info *info){
	char buff[15000 + 1];
	char msg[50];
	char *newMsg;
	char *to_send;
	int ret;

	ret = recv(cli->fd, buff, 15000, MSG_DONTWAIT);
	buff[ret] = '\0';
	cli->recvBuffer = str_join(cli->recvBuffer, buff);
	if (!cli->recvBuffer)
		exit_error("Fatal error\n", info);

	if (ret == 0){
		sprintf(msg, "server: client %d just left\n", cli->id);
		remove_client_to_list(info, cli->fd);
        send_to_clients(info, msg, -1);
	}
	else if (ret > 0){
		while ( (ret = extract_message(&cli->recvBuffer, &newMsg)) == 1){
			if ( !(to_send = (char *)malloc(sizeof(char) * (strlen(newMsg) + 50))) )
				exit_error("Fatal error\n", info);
			bzero(to_send, strlen(newMsg) + 50);
			sprintf(to_send, "client %d: %s", cli->id, newMsg);
			send_to_clients(info, to_send, cli->fd);
			free(to_send);
			if (newMsg)
				free(newMsg);
		}
		if (ret == -1)
			exit_error("Fatal error extract\n", info);
	}
}

int main(int ac, char **av) {
	t_info info;
	
	if (ac != 2)
		exit_error("Wrong number of arguments\n", NULL);

	init_server(&info, atoi(av[1]));

	while (1){
		info.select = info.save;
		if ( (select(info.fd_max + 1, &info.select, 0, 0, 0)) == -1 )
			exit_error("Fatal error select\n", &info);

		if (FD_ISSET(info.sock, &info.select))
			new_client(&info);

		t_c *cli = info.clients;
		t_c *next;
		while (cli){
			next = cli->next;
			if (FD_ISSET(cli->fd, &info.select))
				recv_msg(cli, &info);
			cli = next;
		}
	}
	return (0);
}
