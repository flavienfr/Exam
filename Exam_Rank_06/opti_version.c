#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <stdlib.h>//Linux
#include <stdio.h>//Linux

int g_id = 0;

typedef struct s_c{
	int fd;
	int id;
	char *rcvBuf;
	struct s_c *nxt;
}t_c;


typedef struct s_info{
	int sock;
	int fdMax;
	fd_set select;
	fd_set save;
	t_c *clients;
}t_info;

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

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

char *str_join(char *buf, char *add)
{
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
	if (buf)
		free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void exit_error(int code, t_info *info){
	if (info){
		FD_CLR(info->sock, &info->save);
		close(info->sock);
		t_c *nxt;
		t_c *cli = info->clients;
		while (cli){
			nxt = cli->nxt;
			FD_CLR(cli->fd, &info->save);
			close(cli->fd);
			if (cli->rcvBuf)
				free(cli->rcvBuf);
			free(cli);
			cli = nxt;
		}
	}
	if (code == 0)
		write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
	else
		write(2, "Fatal error\n", strlen("Fatal error\n"));
	exit(1);
}

void init(t_info *info, int port){
	int sockfd;
	struct sockaddr_in servaddr; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) 
		exit_error(1, info);
	
	info->sock = sockfd;
	info->fdMax = sockfd;
	FD_ZERO(&info->select);
	FD_ZERO(&info->save);
	FD_SET(info->sock, &info->save);
	info->clients = NULL;

	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		exit_error(1, info);

	if (listen(sockfd, 10) != 0)
		exit_error(1, info);
}

int add_client(t_info *info, int fd){
	t_c *cli = info->clients;
	t_c *new;

	if ( !(new = malloc(sizeof(t_c))) )
		exit_error(1, info);
	new->fd = fd;
	new->id = g_id++;
	new->nxt = NULL;
	new->rcvBuf = NULL;

	if (!cli){
		info->clients = new;
		return (new->id);
	}
	while (cli->nxt)
		cli = cli->nxt;
	cli->nxt = new;

	return (new->id);
}

void send_all(t_info *info, char *msg, int fd){
	t_c *cli = info->clients;
	int len = strlen(msg);

	while(cli){
		if (cli->fd != info->sock && cli->fd != fd){
			int sent = 0;
			int ret = 0;
			while (sent < len){
				ret = send(cli->fd, msg + sent, len - sent, MSG_DONTWAIT);
				if (ret <= 0)
					break;
				sent += ret;
			}
		}
		cli = cli->nxt;
	}
}

void new_client(t_info *info){
	int connfd;
	unsigned int len;
	struct sockaddr_in cli;
	char msg[50];

	len = sizeof(cli);
	connfd = accept(info->sock, (struct sockaddr *)&cli, &len);
	if (connfd < 0)
		exit_error(1, info);
	
	int id = add_client(info, connfd);
	sprintf(msg, "server: client %d just arrived\n", id);
	send_all(info, msg, connfd);

	FD_SET(connfd, &info->save);
	if (connfd > info->fdMax)
		info->fdMax = connfd;
}

void remove_client(t_info *info, int fd){
	t_c *bef = info->clients;
	t_c *cli = bef->nxt;
	
	FD_CLR(fd, &info->save);
	close(fd);

	if (bef->fd == fd){
		info->clients = cli;
		if (bef->rcvBuf)
			free(bef->rcvBuf);
		free(bef);
		return ;
	}

	while (cli){
		if (cli->fd == fd){
			bef->nxt = cli->nxt;
			if (cli->rcvBuf)
				free(cli->rcvBuf);
			free(cli);
			return ;
		}
		bef = cli;
		cli = cli->nxt;
	}
}

void recv_client(t_info *info, t_c *cli){
	char buf[15000 + 1];
	char msg[50];
	char *sendBuf;
	char *newMsg;
	
	int ret = recv(cli->fd, buf, 15000, MSG_DONTWAIT);
	buf[ret] = '\0';
	cli->rcvBuf = str_join(cli->rcvBuf, buf);
	if (!cli->rcvBuf)
		exit_error(1, info);

	if (ret == 0){
		sprintf(msg, "server: client %d just left\n", cli->id);
		remove_client(info, cli->fd);
		send_all(info, msg, -1);
	}
	else if (ret > 0){
		while ( (ret = extract_message(&cli->rcvBuf, &newMsg)) == 1){
			if ( !(sendBuf = (char *)malloc(sizeof(char) * (strlen(newMsg) + 50))) )
				exit_error(1, info);
			bzero(sendBuf, strlen(sendBuf) + 50);// or use calloc
			sprintf(sendBuf, "client %d: %s", cli->id, newMsg);
			send_all(info, sendBuf, cli->fd);
			if (newMsg)
				free(newMsg);
			free(sendBuf);
		}
		if (ret == -1)
			exit_error(1, info);
	}
}

int main(int ac, char **av) {
	t_info info;

	if (ac != 2)
		exit_error(0, NULL);

	init(&info, atoi(av[1]));

	while (1){
		info.select = info.save;
		if ( (select(info.fdMax +1, &info.select, 0, 0, 0)) == -1)
			exit_error(1, &info);
		
		if (FD_ISSET(info.sock, &info.select))
			new_client(&info);

		t_c *cli = info.clients;
		t_c *nxt;
		while (cli){
			nxt = cli->nxt;
			if (FD_ISSET(cli->fd, &info.select))
				recv_client(&info, cli);
			cli = nxt;
		}
	}

	return (0);
}
