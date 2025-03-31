/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:16:53 by albernar          #+#    #+#             */
/*   Updated: 2025/03/31 21:18:08 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	isValidNickname(const std::string &nickname) {
	if (nickname.empty() || nickname.length() > 9)
		return (false);
	for (size_t i = 0; i < nickname.length(); ++i) {
		if (!isalnum(nickname[i]) && nickname[i] != '-' && nickname[i] != '_')
			return (false);
	}
	return (true);
}

std::string	Commands::nickCommand(Client *client, Server *instance, IRCCommand ircCommand) {
	std::string				nickname;
	std::map<int, Client *>	clients;
	std::string				returnValue;

	if (ircCommand.params.size() < 1)
		return ":" + instance->getServerIp() + " 431 " + client->getNickname() + " :No nickname given\r\n";
	nickname = ircCommand.params[0];
	if (nickname.empty())
		return ":" + instance->getServerIp() + " 431 " + client->getNickname() + " :No nickname given\r\n";
	if (!isValidNickname(nickname))
		return ":" + instance->getServerIp() + " 432 " + client->getNickname() + " " + nickname + " :Erroneus nickname" + "\r\n";
	clients = instance->getClients();
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getNickname() == nickname)
			return ":" + instance->getServerIp() + " 433 " + client->getNickname() + " " + nickname + " :Nickname is already in use" + "\r\n";
	}
	returnValue = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " NICK :" + nickname + "\r\n";
	client->setNickname(nickname);
	return returnValue;
}
