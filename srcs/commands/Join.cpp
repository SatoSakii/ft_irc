/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:09:42 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 02:13:34 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::joinCommand(Client *client, IRCCommand ircCommand) {
	std::vector<std::string>		channels, keys;
	std::string						channelName, key;
	Channel							*channel;

	if (ircCommand.params.size() < 1) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "JOIN", client->getNickname()));
		return ;
	}
	channels = IRCUtils::splitString(ircCommand.params[0], ",");
	if (ircCommand.params.size() > 1)
		keys = IRCUtils::splitString(ircCommand.params[1], ",");
	for (size_t i = 0; i < channels.size(); ++i) {
		channelName = channels[i];
		key = (i < keys.size()) ? keys[i] : "";
		if (channelName.empty() || channelName[0] != '#') {
			client->sendMessage(ERR_NOSUCHCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
			continue ;
		}
		channel = this->server->getChannel(channelName);
		if (channel && channel->isClientInChannel(client))
			continue ;
		if (!channel)
			channel = this->server->createChannel(channelName, client);
		if (channel->isInviteOnly() && !channel->isInvited(client)) {
			client->sendMessage(ERR_INVITEONLYCHAN(this->server->getServerIp(), channelName, client->getNickname()));
			continue ;
		}
		if (channel->isPasswordProtected() && key != channel->getPassword()) {
			client->sendMessage(ERR_BADCHANNELKEY(this->server->getServerIp(), channelName, client->getNickname()));
			continue ;
		}
		if (channel->isFull()) {
			client->sendMessage(ERR_CHANNELISFULL(this->server->getServerIp(), channelName, client->getNickname()));
			continue ;
		}
		channel->addClient(client);
		channel->removeInvitation(client);
		client->sendMessage(RPL_JOIN(client->getNickname(), channelName));
		channel->broadcastMessage(client, RPL_JOIN(client->getNickname(), channelName));
		if (!channel->getTopic().empty())
			client->sendMessage(RPL_TOPIC(this->server->getServerIp(), client->getNickname(), channelName, channel->getTopic()));
		client->sendMessage(RPL_NAMREPLY(this->server->getServerIp(), client->getNickname(), channelName, channel->getUserList()));
		client->sendMessage(RPL_ENDOFNAMES(this->server->getServerIp(), client->getNickname(), channelName));
	}
}

