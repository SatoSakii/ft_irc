/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:03:18 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 19:05:08 by albernar         ###   ########.fr       */
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
	std::string						oldnick;
	std::map<int, Client*>			clients;
	std::map<std::string, Channel*>	channel;
	std::string						returnValue;

	oldnick = client->getNickname();
	if (client->getNickname().empty())
		oldnick = "*";
	if (ircCommand.params.size() < 1) {
		client->sendMessage(ERR_NONICKNAMEGIVEN(this->server->getServerIp(), oldnick));
		return ;
	}
	nickname = ircCommand.params[0];
	if (nickname.empty()) {
		client->sendMessage(ERR_NONICKNAMEGIVEN(this->server->getServerIp(), oldnick));
		return ;
	}
	if (!isValidNickname(nickname)) {
		client->sendMessage(ERR_ERRONEUSNICKNAME(this->server->getServerIp(), oldnick));
		return ;
	}
	clients = this->server->getClients();
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			client->sendMessage(ERR_NICKNAMEINUSE(this->server->getServerIp(), oldnick));
			return ;
		}
	}
	channel = this->server->getChannels();
	for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); ++it) {
		if (it->second->isClientInChannel(client))
			it->second->broadcastMessage(client, RPL_CHANGENICK(nickname, nickname, client->getUsername(), client->getHostname()));
	}
	client->sendMessage(RPL_CHANGENICK(nickname, nickname, client->getUsername(), client->getHostname()));
	client->setNickname(nickname);
}
