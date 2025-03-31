/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:37:28 by albernar          #+#    #+#             */
/*   Updated: 2025/03/31 21:23:56 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <map>
# include <netinet/in.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h> 
# include <sys/epoll.h>
# include <poll.h>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include <cstring>
# include <sstream>
# include <cerrno>

struct IRCCommand {
	std::string					command;
	std::vector<std::string>	params;
};

class Client
{
private:
	int			fd;
	bool		auth;
	std::string	nickname;
	std::string	username;
	std::string	bufferMessage;
	std::string	hostname;

public:
	int			getFd(void) const { return fd; }

	bool		isAuth(void) const { return auth; }
	void		setAuth(bool auth) { this->auth = auth; }

	std::string	getHostname(void) const { return hostname; }

	void		appendToBuffer(std::string message) { bufferMessage += message; }
	std::string	getBuffer(void) const { return bufferMessage; }
	void		clearBuffer(void) { bufferMessage.clear(); }

	std::string	getNickname(void) const { return nickname; }
	void		setNickname(std::string nickname) { this->nickname = nickname; }

	std::string	getUsername(void) const { return username; }
	void		setUsername(std::string username) { this->username = username; }

	Client(int fd, std::string hostname) : fd(fd), auth(false), hostname(hostname) {}
};

class Server;

class Commands {
	public:
		std::string	nickCommand(Client *client, Server *instance, IRCCommand ircCommand);
};

class Server
{
public:
	Server(long port, std::string &password);
	~Server(void);
	void	serverInit(void);

	std::map<int, Client*>	getClients(void) const { return clients; };
	std::string				getServerIp(void) const { return serverIp; }
private:
	static Commands			*commands;
	static Server			*instance;
	bool					isRunning;
	int						serverSocket;
	int						epollInstance;
	long					serverPort;
	std::string				serverIp;
	std::string				serverPassword;
	std::map<int, Client*>	clients;

	static Server	*getInstance(void) { return instance; }
	static void		signalHandler(int signum);

	void			runServer(void);
	void			stopServer(void);

	void			handleClientMessage(Client *client);
	void    		processClientMessage(Client *client, const std::string &message);
	void			sendMessage(int fd, std::string message);

	void			disconnectClient(Client *client);
	void			acceptNewClient(void);
};


#endif