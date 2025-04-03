/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:13:18 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 01:05:16 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server	*Server::instance = NULL;

void	Server::signalHandler(int signum) {
	if (signum == SIGINT || signum == SIGQUIT) {
		std::cout << "Stopping server..." << std::endl;
		if (instance)
			instance->stopServer();
	}
}

Server::Server(long serverPort, std::string serverPassword) {
	this->serverPort = serverPort;
	this->serverPassword = serverPassword;
	this->isRunning = true;
	this->serverSocket = -1;
	this->epollInstance = -1;
	this->instance = this;
	this->commandHandler = new CommandHandler(instance);
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);	
}

Server::~Server(void) {
	this->stopServer();
	delete this->commandHandler;
}

void	Server::serverInit(void) {
	struct hostent		*host;
	struct sockaddr_in	serverAddr;
    int					optval;
	char				hostname[256];

	optval = 1;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        throw std::runtime_error("Error creating socket");
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("Error setting socket options");
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Error setting socket to non-blocking mode");
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Error binding socket");
    if (listen(serverSocket, SOMAXCONN) < 0)
		throw std::runtime_error("Error listening on socket");
	if (gethostname(hostname, sizeof(hostname)) < 0)
		throw std::runtime_error("Error getting hostname");
	host = gethostbyname(hostname);
	if (!host)
		throw std::runtime_error("Error getting host by name");
	this->serverIp = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
	std::cout << "-----------------" << std::endl;
	std::cout << "IP > " << this->serverIp << std::endl; 
	std::cout << "Port > " << serverPort << std::endl;
	std::cout << "-----------------" << std::endl;
    Server::runServer();
}

void	Server::runServer(void) {
	struct epoll_event	events[1024];
    struct epoll_event	serverEvent;
	int					eventCount;

    this->epollInstance = epoll_create1(0);
    if (this->epollInstance < 0)
		throw std::runtime_error("Error creating epoll instance");
    serverEvent.events = EPOLLIN;
    serverEvent.data.fd = serverSocket;
    if (epoll_ctl(this->epollInstance, EPOLL_CTL_ADD, serverSocket, &serverEvent) < 0)
        throw std::runtime_error("Error adding server socket to epoll");
    while (isRunning) {
        eventCount = epoll_wait(this->epollInstance, events, 1024, -1);
        if (eventCount < 0)
		{
			if (errno == EINTR)
				continue ;
            throw std::runtime_error("Error in epoll_wait");
		}
        for (int i = 0; i < eventCount; i++) {
            if (events[i].data.fd == serverSocket)
                Server::acceptNewClient();
            else
                Server::handleClientMessage(this->clients[events[i].data.fd]);
        }
    }
}

void	Server::stopServer(void) {
	this->isRunning = false;
	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        close(it->first);
        delete it->second;
    }
    this->clients.clear();
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
    if (epollInstance >= 0) {
        close(epollInstance);
        epollInstance = -1;
    }
}

std::map<int, Client*>	Server::getClients(void) const { return this->clients; }

std::map<std::string, Channel *>	Server::getChannels(void) const { return this->channels; }

std::string	Server::getServerIp(void) const { return this->serverIp; }

std::string Server::getPassword(void) const { return this->serverPassword; }

Channel	*Server::getChannel(std::string channelName) const {
	std::map<std::string, Channel *>::const_iterator it = this->channels.find(channelName);
	if (it != this->channels.end())
		return it->second;
	return NULL;
}

Channel	*Server::createChannel(std::string channelName) {
	Channel	*channel = new Channel(channelName);
	this->channels[channelName] = channel;
	return channel;
}

void	Server::disconnectClient(Client *client) {
	int	clientFd;

    if (!client)
        return;
    clientFd = client->getFd();
    if (epoll_ctl(epollInstance, EPOLL_CTL_DEL, clientFd, NULL) < 0)
        std::cerr << "Error removing client from epoll" << std::endl;
    close(clientFd);
    this->clients.erase(clientFd);
    delete client;
}

void	Server::acceptNewClient(void) {
	struct sockaddr_in		clientAddr;
    socklen_t				clientAddrLen;
	int						newClientFd;
	struct epoll_event		clientEvent;
	struct in_addr			addr;
	struct hostent			*host;
	std::string				ip;

	clientAddrLen = sizeof(clientAddr);
    newClientFd = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (newClientFd < 0) {
        std::cerr << "Error accepting client connection" << std::endl;
        return ;
    }
    if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) < 0) {
        close(newClientFd);
        std::cerr << "Error setting client socket to non-blocking mode" << std::endl;
        return ;
    }
    clientEvent.events = EPOLLIN;
    clientEvent.data.fd = newClientFd;
    if (epoll_ctl(epollInstance, EPOLL_CTL_ADD, newClientFd, &clientEvent) < 0) {
        close(newClientFd);
        std::cerr << "Error adding client socket to epoll" << std::endl;
        return ;
    }
	addr.s_addr = inet_addr(this->serverIp.c_str());
	if (addr.s_addr == INADDR_NONE) {
		std::cerr << "Error converting IP address" << std::endl;
		close(newClientFd);
		return ;
	}
	host = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
	if (host && host->h_name)
		ip = host->h_name;
	else
		ip = this->serverIp;
    this->clients[newClientFd] = new Client(newClientFd, ip);
    std::cout << "New client connected" << std::endl;
}

bool	Server::CheckAuthProtocol(Client *&client, IRCCommand ircCommand) {
	if (client->getAuthLevel() == 0)
	{
		if (ircCommand.command == "CAP")
			return false;
		if (ircCommand.command == "PASS") {
			this->commandHandler->passCommand(client, ircCommand);
			return true;
		}
		else {
			client->sendMessage(ERR_NOTREGISTERED(this->serverIp, "*"));
			return false;
		}
	}
	else if (client->getAuthLevel() == 1)
	{
		if (ircCommand.command == "NICK") {
			this->commandHandler->nickCommand(client, ircCommand);
			if (client->getNickname().empty())
				return false;
			return true;
		} else {
			client->sendMessage(ERR_NOTREGISTERED(this->serverIp, "*"));
			return false;
		}
	}
	else if (client->getAuthLevel() == 2)
	{
		if (ircCommand.command == "USER") {
			this->commandHandler->userCommand(client, ircCommand);
			if (client->getUsername().empty())
				return false;
			client->setAuth(true);
			return true;
		} else {
			client->sendMessage(ERR_NOTREGISTERED(this->serverIp, client->getNickname()));
			return false;
		}
	}
	return false;
}

void	Server::processClientMessage(Client *&client, const std::string &message) {
	IRCCommand	ircCommand;

	if (!client)
		return ;
	ircCommand = parseIRCCommand(message);
	std::cout << "Received command: " << ircCommand.command << std::endl;
	for (std::vector<std::string>::iterator it = ircCommand.params.begin(); it != ircCommand.params.end(); ++it)
		std::cout << "Param: " << *it << std::endl;
	if (!client->isAuth())
	{
		if (Server::CheckAuthProtocol(client, ircCommand) && client)
			client->setAuthLevel(client->getAuthLevel() + 1);
		return ;
	}
	if (ircCommand.command == "PASS" || ircCommand.command == "USER")
		client->sendMessage(ERR_ALREADYREGISTERED(this->serverIp, client->getNickname()));
	else if (ircCommand.command == "NICK")
		this->commandHandler->nickCommand(client, ircCommand);
	else if (ircCommand.command == "QUIT")
		this->commandHandler->quitCommand(client, ircCommand);
	else if (ircCommand.command == "JOIN")
		this->commandHandler->joinCommand(client, ircCommand);
}

void	Server::handleClientMessage(Client *client) {
    char		buffer[1024];
    int			bytesRead;
	std::string	tmpBuffer;
	std::string	message;
    size_t		pos;

	while ((bytesRead = recv(client->getFd(), buffer, sizeof(buffer) - 1, 0)) > 0) {
		buffer[bytesRead] = '\0';
		client->appendToBuffer(buffer);	
	}
    if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
        Server::disconnectClient(client);
        return ;
    }
    else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        std::cerr << "Error reading from client" << std::endl;
        Server::disconnectClient(client);
        return ;
    }
	tmpBuffer = client->getBuffer();
    while ((pos = tmpBuffer.find("\r\n")) != std::string::npos) {
        message = tmpBuffer.substr(0, pos);
        tmpBuffer = tmpBuffer.substr(pos + 2);
        if (!message.empty())
        	Server::processClientMessage(client, message);
    }
	if (client)
    	client->clearBuffer();
    if (client && !tmpBuffer.empty())
        client->appendToBuffer(tmpBuffer);
}
