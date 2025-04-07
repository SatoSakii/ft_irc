/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabartho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:09:42 by sabartho          #+#    #+#             */
/*   Updated: 2025/04/04 21:53:12 by sabartho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include <algorithm>


bool    __acceptValue(char value) {
    if (value != 'i' && value != 't' && value != 'o' && value != 'l' && value != 'k')
        return false;
    return true;
}

void    changePermissions(char value, char status_mode, Channel *channel, std::string keys, int *it, Server *server, Client *clientExecutor) {
    Client                  *client;
    int                     setClientsNumber = 0;
    std::stringstream       out;

    if (status_mode == '+') {
        switch (value) {
            case 'i' :
                if (!channel->isInviteOnly()) {
                    channel->setInviteOnly(true);
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+i"));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+i"));
                }
                break;
            case 't' :
                if (!channel->getTopicSet()) {
                    channel->setTopicSet(true);
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+t"));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+t"));
                }
                break;
            case 'l' :
                (*it)++;
                if (keys.empty()) { 
                    clientExecutor->sendMessage(ERR_NEEDMOREPARAMS(server->getServerIp(), "MODE +l", clientExecutor->getNickname()));
                    return;
                }
                setClientsNumber = (int)std::strtol(keys.c_str(), NULL, 0);
                channel->setMaxClients(setClientsNumber);
                out << setClientsNumber;
                channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+l " + out.str()));
                clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+l " + out.str()));
                break;
            case 'o' :
                (*it)++;
                if (keys.empty()) {
                    clientExecutor->sendMessage(ERR_NEEDMOREPARAMS(server->getServerIp(), "MODE +o", clientExecutor->getNickname()));
                    return;
                }
                client = server->getClientByName(keys);
                if (!client) {
                    clientExecutor->sendMessage(ERR_NOSUCHNICK(server->getServerIp(), channel->getName(), keys));
                    return ;
                }
                if (!channel->isOperator(client)) {
                    channel->setClientOperator(client);
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+o " + keys));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+o " + keys));
                }
                break;
            case 'k' :
                (*it)++;
                if (keys.empty()) {
                    clientExecutor->sendMessage(ERR_NEEDMOREPARAMS(server->getServerIp(), "MODE +k", clientExecutor->getNickname()));
                    return;
                }
                channel->setPassword(keys);
                channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+k " + keys));
                clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "+k " + keys));
                break;
        }
    }
    else if (status_mode == '-') {
        switch (value) {
            case 'i' :
                if (channel->isInviteOnly()) {
                    channel->setInviteOnly(false);
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-i"));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-i"));
                }
                break;
            case 't' :
                if (channel->getTopicSet()) {
                    channel->setTopicSet(false);
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-t"));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-t"));
                }
                break;
            case 'l' :
                if (channel->getMaxClients() != __INT_MAX__) {
                    channel->setMaxClients(__INT_MAX__);
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-l"));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-l"));
                }
                break;

            case 'o' :
                (*it)++;
                client = server->getClientByName(keys);
                if (!client) {
                    clientExecutor->sendMessage(ERR_NOSUCHNICK(server->getServerIp(), channel->getName(), keys));
                    return ;
                }
                if (channel->isOperator(client)) {
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-o " + keys));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-o " + keys));
                    channel->removeOperator(client);
                }
                break;
            case 'k' :
                if (channel->isPasswordProtected()) {
                    channel->setPassword("");
                    channel->broadcastMessage(clientExecutor, RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-k *"));
                    clientExecutor->sendMessage(RPL_MODE(clientExecutor->getNickname(), clientExecutor->getUsername(), clientExecutor->getHostname(), channel->getName(), "-k *"));
                }
                break;
        }
    }
}

void    __parsingValue(std::string str, Channel *channel, Server *server, std::vector<std::string> keys, int *key_index, Client *client) {
    char        status_mode;
    std::string str_key;

    status_mode = str[0];
    for (size_t j = 1; j < str.size(); j++) {
        if ((str[j] == '+' || str[j] == '-') && j + 1 < str.size() && std::isalpha(str[j + 1])) {
            status_mode = str[j];
            continue ;
        }
        if (!__acceptValue(str[j])) {
            client->sendMessage(ERR_UNKNOWNMODE(server->getServerIp(), channel->getName(), client->getNickname(), str[j]));
            return ;
        }
        if (*key_index >= (int)keys.size())
            str_key = "";
        else
            str_key = keys[*key_index];
        changePermissions(str[j], status_mode, channel, str_key, key_index, server, client);
    }
}
void	CommandHandler::modeCommand(Client *client, IRCCommand ircCommand) {
    std::string                 channelName;
    std::vector<std::string>    keys;
    Channel                     *channel;
    int                         stop_flag = ircCommand.params.size();
    int                         key_index = 0;

    if (ircCommand.params.size() < 2) {
		client->sendMessage(ERR_NEEDMOREPARAMS(this->server->getServerIp(), "MODE", client->getNickname()));
		return ;
	}

    channelName = ircCommand.params[0];
    channel = this->server->getChannel(channelName);
    if (!channel) {
        client->sendMessage(ERR_NOSUCHCHANNEL(this->server->getServerIp(), channelName, client->getNickname()));
        return ;
    }
    if (!channel->isOperator(client)) {
        client->sendMessage(ERR_CHANOPRIVSNEEDED(this->server->getServerIp(), channelName, client->getNickname()));
        return ;
    }
    for (size_t i = 1; i < ircCommand.params.size(); i++) {
        std::string str = ircCommand.params[i];
        size_t pos1 = str.find('+');
        size_t pos2 = str.find('-');
        if (pos1 + 1 == str.size())
            return ;
        if (pos2 + 1 == str.size())
            return ;
        if ((pos1 == std::string::npos && pos2 == std::string::npos)
        || (pos2 != std::string::npos && !std::isalpha(str[pos2 + 1]))
        || (pos1 != std::string::npos && !std::isalpha(str[pos1 + 1]))) {
            stop_flag = i;
            for (size_t j = i; j < ircCommand.params.size(); j++) {
                keys.push_back(ircCommand.params[j]);
            }
            break;
        }
    }
    for (int i = 1; i < stop_flag; i++) {
        __parsingValue(ircCommand.params[i], channel, this->server, keys, &key_index, client);
    }
}
