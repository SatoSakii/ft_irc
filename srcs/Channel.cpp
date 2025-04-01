/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:05:56 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 18:24:38 by albernar         ###   ########.fr       */
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

void	Channel::broadcastMessage(Client *client, std::string message) const {
	for (std::map<int, Client *>::const_iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (it->second != client)
			it->second->sendMessage(message);
	}
}