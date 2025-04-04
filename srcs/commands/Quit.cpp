/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:36:39 by albernar          #+#    #+#             */
/*   Updated: 2025/04/03 23:26:52 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::quitCommand(Client *&client, IRCCommand ircCommand) {
	std::string							quitMessage;
	std::map<std::string, Channel *>	channel;

	quitMessage = "Quit: ";
	if (ircCommand.params.size() > 0)
		quitMessage += ircCommand.params[0];
	channel = this->server->getChannels();
	for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); ++it) {
		if (it->second->isClientInChannel(client)) {
			it->second->broadcastMessage(client, RPL_QUIT(client->getNickname(), client->getUsername(), client->getHostname(), quitMessage));
			it->second->removeClient(client);
		}
	}
	client->sendMessage(RPL_QUIT(client->getNickname(), client->getUsername(), client->getHostname(), quitMessage));
	this->server->disconnectClient(client);
	client = NULL;
}
