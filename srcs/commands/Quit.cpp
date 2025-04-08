/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:36:39 by albernar          #+#    #+#             */
/*   Updated: 2025/04/08 17:43:38 by albernar         ###   ########.fr       */
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
			if (it->second->getClients().empty() || (it->second->getClients().size() == 1
				&& IRCUtils::equalsIgnoreCase(it->second->getName(), "#bot")
				&& it->second->getClients().begin()->second == this->server->getClientByName("_Tralalelo")))
				this->server->removeChannel(it->first);
		}
	}
	client->sendMessage(RPL_QUIT(client->getNickname(), client->getUsername(), client->getHostname(), quitMessage));
	std::cout << "[-] " << client->getNickname() << std::endl;
	this->server->disconnectClient(client);
	client = NULL;
}
