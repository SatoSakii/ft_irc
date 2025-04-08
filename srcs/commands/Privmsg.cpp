/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 20:06:25 by albernar          #+#    #+#             */
/*   Updated: 2025/04/08 17:37:19 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::privmsgCommand(Client *client, IRCCommand ircCommand) {
	std::vector<std::string>		targets;
	std::string						messageText;
	std::string						target;
	Channel							*channel;
	Client							*targetClient;
	std::string						botCommand;

	if (ircCommand.params.size() < 2) {
		client->sendMessage(ERR_NORECIPIENT(this->server->getServerIp(), std::string("PRIVMSG"), client->getNickname()));
        return ;
	}
	targets = IRCUtils::splitString(ircCommand.params[0], ",");
	messageText = ircCommand.params[1];
	botCommand = messageText;
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it) {
		target = *it;
		if (target[0] == '#') {
			channel = this->server->getChannel(target);
			if (!channel) {
				client->sendMessage(ERR_NOSUCHCHANNEL(this->server->getServerIp(), target, client->getNickname()));
				continue ;
			}
			if (!channel->isClientInChannel(client)) {
				client->sendMessage(ERR_CANNOTSENDTOCHAN(this->server->getServerIp(), target, client->getNickname()));
				continue ;
			}
			#ifdef BONUS
			if (IRCUtils::equalsIgnoreCase(channel->getName(), "#bot") && IRCUtils::startsWith(messageText, "!transform")
				&& channel->isClientInChannel(this->server->getClientByName("_Tralalelo")))
				botCommand = BotCommands::transformCommand(messageText);
			#endif
			channel->broadcastMessage(client, RPL_PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), target, botCommand));
		} else {
			targetClient = this->server->getClientByName(target);
			if (!targetClient) {
				client->sendMessage(ERR_NOSUCHNICK(this->server->getServerIp(), target, client->getNickname()));
				continue ;
			}
			if (targetClient != client)
				targetClient->sendMessage(RPL_PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), target, messageText));
			else
				client->sendMessage(RPL_PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), target, messageText));
		}
	}
}
