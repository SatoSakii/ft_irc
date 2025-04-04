/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:09:42 by sabartho          #+#    #+#             */
/*   Updated: 2025/04/04 19:11:23 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"


// Command: KICK
// Parameters: <channel> <user> *( "," <user> ) [<comment>]


void	CommandHandler::kickCommand(Client *client, IRCCommand ircCommand)
{
    std::string                 channelName;
    std::string                 reason = "no reason";
    Channel                     *channel;
    std::vector<std::string>    clientsName;

    if (ircCommand.params.size() < 2) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "KICK", client->getNickname()));
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
        return;
    }
    if (!channel->isOperator(client)) {
        client->sendMessage(ERR_CHANOPRIVSNEEDED(this->server->getServerIp(), channelName, client->getNickname()));
        return;
    }
    if (ircCommand.params.size() >= 3)
        reason = ircCommand.params[2];
    clientsName = IRCUtils::splitString(ircCommand.params[1], ",");
    for (size_t i = 0; i < clientsName.size(); ++i) {
        Client *currentClient = this->server->getClientByName(clientsName[i]);
        if (!currentClient || !channel->isClientInChannel(currentClient)) {
            client->sendMessage(ERR_USERNOTINCHANNEL(this->server->getServerIp(), channelName, client->getNickname(), clientsName[i]));
            continue;
        }
        channel->broadcastMessage(currentClient, RPL_KICK(client->getNickname(), client->getUsername(), client->getHostname(), currentClient->getNickname(), channelName, reason));
        currentClient->sendMessage(RPL_KICK(client->getNickname(), client->getUsername(), client->getHostname(), currentClient->getNickname(), channelName, reason));
        channel->removeClient(currentClient);
		if (channel->getClients().empty())
			this->server->removeChannel(channelName);
    }
}