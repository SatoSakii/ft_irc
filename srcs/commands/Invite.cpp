/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:09:42 by sabartho          #+#    #+#             */
/*   Updated: 2025/04/04 05:20:26 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

void	CommandHandler::inviteCommand(Client *&client, IRCCommand ircCommand) {
    Client                  *invitedClient;
    std::string             channelName;
    Channel                 *channel;

    if (ircCommand.params.size() < 1) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "INVITE", client->getNickname()));
		return ;
	}
    channelName = ircCommand.params[1];
    channel = this->server->getChannel(channelName);
    invitedClient = this->server->getClientByName(ircCommand.params[0]);
    if (!invitedClient) {
        client->sendMessage(ERR_NOSUCHNICK(this->server->getServerIp(), ircCommand.params[0], client->getNickname()));
        return ;
	}
    if (!channel) {
        client->sendMessage(ERR_NOSUCHCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
        return ;
    }
    if (channel->isClientInChannel(invitedClient)) {
        client->sendMessage(ERR_USERONCHANNEL(this->server->getServerIp(), channelName, client->getNickname(), invitedClient->getNickname()));
        return ;
    }
    if (channel->isInviteOnly() && !channel->isOperator(client)) {
        client->sendMessage(ERR_CHANOPRIVSNEEDED(this->server->getServerIp(), channelName, client->getNickname()));
        return ;
    }
    client->sendMessage(RPL_INVITING(this->server->getServerIp(), client->getNickname(), invitedClient->getNickname(), channelName));
	invitedClient->sendMessage(RPL_INVITED(client->getNickname(), client->getUsername(), invitedClient->getHostname(), invitedClient->getNickname(), channelName));
    channel->addInvitedClient(invitedClient);
}