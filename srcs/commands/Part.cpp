/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 02:48:46 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 05:22:11 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::partCommand(Client *client, IRCCommand ircCommand) {
	std::vector<std::string>		channels;
	std::string						channelName;
	Channel							*channel;
	std::string						partMessage;

	if (ircCommand.params.size() < 1) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "PART", client->getNickname()));
		return ;
	}
	channels = IRCUtils::splitString(ircCommand.params[0], ",");
	for (size_t i = 0; i < channels.size(); ++i) {
		channelName = channels[i];
		channel = this->server->getChannel(channelName);
		if (!channel) {
			client->sendMessage(ERR_NOSUCHCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
			continue ;
		}
		if (!channel->isClientInChannel(client)) {
			client->sendMessage(ERR_NOTONCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
			continue ;
		}
		partMessage = (ircCommand.params.size() > 1) ? ircCommand.params[1] : "Leaving";
		client->sendMessage(RPL_PART(client->getNickname(), client->getUsername(), client->getHostname(), channelName + " :" + partMessage));
		channel->broadcastMessage(client, RPL_PART(client->getNickname(), client->getUsername(), client->getHostname(), channelName + " :" + partMessage));
		channel->removeClient(client);
		channel->removeInvitation(client);
		if (channel->getClients().empty())
			this->server->removeChannel(channelName);
	}
}
