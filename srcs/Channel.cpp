/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:05:56 by albernar          #+#    #+#             */
/*   Updated: 2025/04/08 16:54:32 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(std::string name) : name(name) {
	this->maxClients = 2147483647;
	this->inviteOnly = false;
	this->password = "";
	this->topic = "";
	this->topicSet = false;
}

void	Channel::addClient(Client *client) {
	this->clients[client->getFd()] = client;
}

void	Channel::removeOperator(Client *client) {
	std::vector<Client *>::iterator it = std::find(this->clientsOperator.begin(), this->clientsOperator.end(), client);
	if (it != this->clientsOperator.end())
		this->clientsOperator.erase(it);
}

void	Channel::setTopicSet(bool topicSet) {
	this->topicSet = topicSet;
}

bool	Channel::getTopicSet(void) const {
	return this->topicSet;
}

void	Channel::setTopicTime(int topicTime) {
	this->topicTime = topicTime;
}

int	Channel::getTopicTime(void) const {
	return this->topicTime;
}

void	Channel::setTopicAuthor(Client *client) {
	this->topicAuthor = client;
}

Client	*Channel::getTopicAuthor(void) const {
	return this->topicAuthor;
}

void	Channel::removeClient(Client *client) {
	if (this->isOperator(client))
		removeOperator(client);
	this->clients.erase(client->getFd());
}

bool	Channel::isClientInChannel(Client *client) const {
	return (this->clients.find(client->getFd()) != this->clients.end());
}

bool	Channel::isInviteOnly(void) const {
	return this->inviteOnly;
}

bool	Channel::isInvited(Client *client) const {
    std::vector<Client *>::const_iterator it = std::find(this->invitedClients.begin(), this->invitedClients.end(), client);
    if (it != this->invitedClients.end())
       return true;
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

int	Channel::getMaxClients(void) const {
	return this->maxClients;
}

void	Channel::setTopic(std::string topic) {
	this->topic = topic;
}

void	Channel::setMaxClients(int number) {
	this->maxClients = number;
}

void	Channel::setInviteOnly(bool value) {
	this->inviteOnly = value;
}

void	Channel::setPassword(std::string password) {
	this->password = password;
}

bool	Channel::isOperator(Client *client) const {
	for (size_t i = 0; i < this->clientsOperator.size(); i++) {
		if (this->clientsOperator[i] == client)
			return true;
	}
	return false;
}

std::string Channel::getName(void) const {
	return this->name;
}

void	Channel::setClientOperator(Client *client) {
	this->clientsOperator.push_back(client);
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

std::vector<Client *> Channel::getInvitedClients(void) const {
	return this->invitedClients;
}

void Channel::addInvitedClient(Client *client) {
	this->invitedClients.push_back(client);
}

void Channel::removeInvitation(Client *client) {
	if (this->isInvited(client)) {
		std::vector<Client *>::iterator it = std::find(this->invitedClients.begin(), this->invitedClients.end(), client);
		if (it != this->invitedClients.end())
			this->invitedClients.erase(it);
	}
}

void	Channel::broadcastMessage(Client *client, std::string message) const {
	for (std::map<int, Client *>::const_iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (it->second != client)
			it->second->sendMessage(message);
	}
}
