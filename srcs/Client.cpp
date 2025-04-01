/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:41:36 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 19:51:31 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, std::string hostname) {
	this->fd = fd;
	this->hostname = hostname;
	this->auth = false;
	this->authPhase = 0;
	this->nickname = "";
	this->username = "";
}

int	Client::getFd(void) const { return this->fd; }

bool	Client::isAuth(void) const { return this->auth; }

void	Client::setAuth(bool auth) {
	this->auth = auth;
}

short	Client::getAuthPhase(void) const { return this->authPhase; }

void	Client::setAuthPhase(short authPhase, IRCCommand ircCommand, std::string password) {
	if (authPhase < 0)
		return ;
	this->authPhase = authPhase;
	if (authPhase == 0 && ircCommand.params.size() == 1 && ircCommand.params[0] == password)
		this->authPhase++;
	else if (authPhase == 1 && ircCommand.params.size() == 1)
	{
		this->setUsername(ircCommand.params[0]);
		this->authPhase++;
	}
	else if (authPhase == 2 && ircCommand.params.size() == 1)
	{
		this->setNickname(ircCommand.params[0]);
		this->authPhase = -1;
		this->setAuth(true);
	}
}

std::string	Client::getHostname(void) const { return this->hostname; }

std::string	Client::getNickname(void) const { return this->nickname; }

void	Client::setNickname(std::string nickname) {
	this->nickname = nickname;
}

std::string	Client::getUsername(void) const { return this->username; }

void	Client::setUsername(std::string username) {
	this->username = username;
}

void	Client::appendToBuffer(std::string message) {
	this->bufferMessage += message;
}

std::string	Client::getBuffer(void) const { return this->bufferMessage; }

void	Client::clearBuffer(void) {
	this->bufferMessage.clear();
}

void	Client::sendMessage(std::string message) {
	if (message.empty())
		return ;
	if (send(this->fd, message.c_str(), message.length(), 0) < 0)
		std::cerr << "Error sending message to client" << std::endl;
}
