/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 00:19:15 by albernar          #+#    #+#             */
/*   Updated: 2025/04/02 02:25:35 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::userCommand(Client *client, IRCCommand ircCommand) {
	if (ircCommand.params.size() < 4) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "USER", client->getNickname()));
		return ;
	}
	if (ircCommand.params[0].empty() || ircCommand.params[3].empty()) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "USER", client->getNickname()));
		return ;
	}
	client->setUsername(ircCommand.params[0]);
}

bool	CommandHandler::passCommand(Client *client, IRCCommand ircCommand) {
	if (ircCommand.params.size() < 1) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "PASS", client->getNickname()));
		client->sendMessage(ERR_CLOSELINK(std::string("*"), "(Bad password)"));
		client->clearBuffer();
		this->server->disconnectClient(client);
		return false;
	}
	if (ircCommand.params[0] != this->server->getPassword()) {
		client->sendMessage(ERR_PASSWDMISMATCH(this->server->getServerIp()));
		client->sendMessage(ERR_CLOSELINK(std::string("*"), "(Bad password)"));
		this->server->disconnectClient(client);
		return false;
	}
	return true;
}
