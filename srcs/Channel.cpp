/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:05:56 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 03:34:50 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : name(name) {
	this->clientOperator = NULL;
	this->maxClients = 2147483647;
	this->inviteOnly = false;
	this->password = "";
	this->topic = "";
}

void	Channel::addClient(Client *client) {
	this->clients[client->getFd()] = client;
}

void	Channel::removeClient(int fd) {
	if (this->clientOperator && this->clientOperator->getFd() == fd)
		this->clientOperator = NULL;
	this->clients.erase(fd);
}

bool	Channel::isClientInChannel(Client *client) const {
	return (this->clients.find(client->getFd()) != this->clients.end());
}

bool	Channel::isInviteOnly(void) const {
	return this->inviteOnly;
}

bool	Channel::isInvited(Client *client) const {
	(void)client;
	// please sacha implement this
	return false;
}

bool	Channel::isPasswordProtected(void) const {
	return !this->password.empty();
}

std::string	Channel::getPassword(void) const {
	return this->password;
}

bool	Channel::isFull(void) const {
	return ((int)this->clients.size() >= this->maxClients);
}

std::string	Channel::getTopic(void) const {
	return this->topic;
}

void	Channel::setTopic(std::string topic) {
	this->topic = topic;
}

bool	Channel::isOperator(Client *client) const {
	return (this->clientOperator == client);
}

void	Channel::setClientOperator(Client *client) {
	this->clientOperator = client;
}

std::map<int, Client *> Channel::getClients(void) const { return this->clients; }

std::string Channel::getUserList(void) const {
	std::string				userList;
	std::map<int, Client *>	channelClients;

	channelClients = Channel::getClients();
	for (std::map<int, Client *>::const_iterator it = channelClients.begin(); it != channelClients.end(); ++it) {
		if (!userList.empty())
			userList += " ";
		if (this->isOperator(it->second))
			userList += "@";
		userList += it->second->getNickname();
	}
	return userList;
}

void	Channel::broadcastMessage(Client *client, std::string message) const {
	for (std::map<int, Client *>::const_iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (it->second != client)
			it->second->sendMessage(message);
	}
}
