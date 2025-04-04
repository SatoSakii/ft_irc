/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 03:29:47 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 04:40:22 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::topicCommand(Client *client, IRCCommand ircCommand) {
	std::string		channelName;
	Channel*		channel;

	if (ircCommand.params.size() < 1) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "TOPIC", client->getNickname()));
		return ;
	}
	channelName = ircCommand.params[0];
	channel = this->server->getChannel(channelName);
	if (!channel) {
		client->sendMessage(ERR_NOSUCHCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
		return ;
	}
	if (!channel->isClientInChannel(client)) {
		client->sendMessage(ERR_NOTONCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
		return ;
	}
	if (ircCommand.params.size() == 1) {
		if (channel->getTopic().empty())
			client->sendMessage(RPL_NOTOPIC(this->server->getServerIp(), client->getNickname(), channelName));
		else
			client->sendMessage(RPL_TOPIC(this->server->getServerIp(), client->getNickname(), channelName, channel->getTopic()));
		return ;
	}
	if (!channel->isOperator(client)) {
		client->sendMessage(ERR_CHANOPRIVSNEEDED(this->server->getServerIp(), channelName, client->getNickname()));
		return ;
	}
	if (ircCommand.params[1].empty()) {
		channel->setTopic("");
		client->sendMessage(RPL_NOTOPIC(this->server->getServerIp(), client->getNickname(), channelName));
		channel->broadcastMessage(client, RPL_NOTOPIC(this->server->getServerIp(), client->getNickname(), channelName));
	} else {
		channel->setTopic(ircCommand.params[1]);
		client->sendMessage(RPL_TOPIC(this->server->getServerIp(), client->getNickname(), channelName, ircCommand.params[1]));
		channel->broadcastMessage(client, RPL_TOPIC(this->server->getServerIp(), client->getNickname(), channelName, ircCommand.params[1]));
	}
}