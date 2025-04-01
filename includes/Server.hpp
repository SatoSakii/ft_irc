/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:08:50 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 18:02:41 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <map>
# include <signal.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <netdb.h>
# include <sys/epoll.h>
# include <cstring>
# include <cerrno>

# include "Client.hpp"
# include "Channel.hpp"
# include "CommandHandler.hpp"
# include "IRCUtils.hpp"

class CommandHandler;
class Server
{
private:
	static Server					*instance;
	bool							isRunning;
	int								serverSocket;
	int								epollInstance;
	long							serverPort;
	std::string						serverPassword;
	std::string						serverIp;

	std::map<int, Client*>			clients;
	std::map<std::string, Channel*>	channels;

	CommandHandler					*commandHandler;

	static Server					*getInstance(void);
	static void						signalHandler(int signal);

	void							runServer(void);
	void							stopServer(void);

	void							acceptNewClient(void);
	void							disconnectClient(Client *client);

	void							handleClientMessage(Client *client);
	void							processClientMessage(Client *client, const std::string &message);
public:
	Server(long serverPort, std::string serverPassword);
	~Server(void);

	void							serverInit(void);

	std::map<int, Client*>			getClients(void) const;
	std::map<std::string, Channel*>	getChannels(void) const;
	std::string						getServerIp(void) const;
};

#endif