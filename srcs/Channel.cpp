/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:05:56 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 01:27:14 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : name(name) {}

void	Channel::addClient(Client *client) {
	this->clients[client->getFd()] = client;
}

void	Channel::removeClient(int fd) {
	this->clients.erase(fd);
}

bool	Channel::isClientInChannel(Client *client) const {
	return (this->clients.find(client->getFd()) != this->clients.end());
}

bool	Channel::isInviteOnly(void) const {
	// please sacha implement this
	return false;
}

bool	Channel::isInvited(Client *client) const {
	(void)client;
	// please sacha implement this
	return false;
}

bool	Channel::isPasswordProtected(void) const {
	// please sacha implement this
	return false;
}

std::string	Channel::getPassword(void) const {
	// please sacha implement this
	return "";
}

bool	Channel::isFull(void) const {
	// please sacha implement this
	return false;
}

std::string	Channel::getTopic(void) const {
	// please sacha implement this
	return "";
}

std::map<int, Client *> Channel::getClients(void) const { return this->clients; }

std::string Channel::getUserList(void) const {
	std::string				userList;
	std::map<int, Client *>	channelClients;

	channelClients = Channel::getClients();
	for (std::map<int, Client *>::const_iterator it = channelClients.begin(); it != channelClients.end(); ++it) {
		if (!userList.empty())
			userList += " ";
		// if (this->isOperator(it->second))
		// 	userList += "@";
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
