/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:03:18 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 18:41:04 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

bool	isValidNickname(const std::string &nickname) {
	if (nickname.empty() || nickname.length() > 9)
		return (false);
	return (true);
}

void	CommandHandler::nickCommand(Client *client, IRCCommand ircCommand) {
	std::string						nickname;
	std::map<int, Client*>			clients;
	std::map<std::string, Channel*>	channel;
	std::string						returnValue;

	if (ircCommand.params.size() < 1) {
		client->sendMessage(IRCResponse::error(this->server->getServerIp(), 431, client->getNickname(), "No nickname given"));
		return ;
	}
	nickname = ircCommand.params[0];
	if (nickname.empty()) {
		client->sendMessage(IRCResponse::error(this->server->getServerIp(), 431, client->getNickname(), "No nickname given"));
		return ;
	}
	if (!isValidNickname(nickname)) {
		client->sendMessage(IRCResponse::error(this->server->getServerIp(), 432, client->getNickname(), "Erroneus nickname"));
		return ;
	}
	clients = this->server->getClients();
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			client->sendMessage(IRCResponse::error(this->server->getServerIp(), 433, client->getNickname(), "Nickname is already in use"));
			return ;
		}
	}
	channel = this->server->getChannels();
	for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); ++it) {
		if (it->second->isClientInChannel(client))
			it->second->broadcastMessage(client, ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " NICK :" + nickname + "\r\n");
	}
	client->sendMessage(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " NICK :" + nickname + "\r\n");
	client->setNickname(nickname);
}
