/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabartho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:09:42 by sabartho          #+#    #+#             */
/*   Updated: 2025/04/04 02:13:34 by sabartho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include <algorithm>

void	CommandHandler::modeCommand(Client *client, IRCCommand ircCommand) {
    std::string                 channelName;
    std::vector<std::string>    keys;
    Channel                     *channel;
    int                         stop_flag = 0;
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
    for (size_t i = 0; i < ircCommand.params.size(); i++) {
        std::string::iterator it = std::find(ircCommand.params[i].begin(), ircCommand.params[i].end(), '+');
        std::string::iterator it2 = std::find(ircCommand.params[i].begin(), ircCommand.params[i].end(), '-');
        if (it == ircCommand.params[i].end() && it2 == ircCommand.params[i].end()) {
            stop_flag = i;
            for (size_t j = i; j < ircCommand.params.size(); j++)
                keys.push_back(ircCommand.params[j]);
            break;
        }
    }
    for (size_t i = 0; i < stop_flag; i++) {
        __parsingValue(ircCommand.params[i], channel, this->server, keys, &key_index);
    }
}

bool    __acceptValue(char value) {
    if (value != 'i' && value != 't' && value != 'o' && value != 'l' && value != 'k')
        return false;
    return true;
}

void    changePermissions(char value, char status_mode, Channel *channel, std::string keys, int *it, Server *server) {

    if (status_mode == '+') {
        switch (value) {

            case 'i' :
                channel->setInviteOnly(true);
                //message
                break;
            case 't' :
                channel->setTopicSet(true);
                //message
                break;
            case 'l' :
                (*it)++;
                channel->setLimit(/*chiffre*/);
                //message
                break;
            case 'o' :
                (*it)++;
                Client *client = server->getClientByName(keys);
                if (!client) {
                    //marche pas
                    return;
                }
                channel->setClientOperator(client);
                break;
            case 'k' :
                (*it)++;
                channel->setPassword(keys);
                //message
                break;
            
            default:
                break;
        }
    }
    else if (status_mode == '-') {
        switch (value) {

            case 'i' :
                channel->setInviteOnly(false);
                //message
                break;

            case 't' :
                channel->setTopicSet(false);
                //message
                break;

            case 'l' :
                channel->setLimit(__INT_MAX__);
                //message
                break;

            case 'o' :
                (*it)++;
                Client *client = server->getClientByName(keys);
                if (!client) {
                    //marche pas
                    return;
                }
                channel->removeOperator(client);
                break;

            case 'k' :
                channel->setPassword("");
                //message
                break;
            
            default:
                break;
        }
    }

}

void    __parsingValue(std::string str, Channel *channel, Server *server, std::vector<std::string> keys, int *key_index) {
    char        status_mode;
    std::string str_key;

    if (str[0] == '+' || str[0] == '-') {
        status_mode = str[0];
        for (int j = 1; j < str.size(); j++) {
            if (!__acceptValue(str[j])) {
                //blabbla mauvais arg
                return ;
            }
            if (*key_index >= keys.size())
                str_key = "";
            else
                str_key = keys[*key_index];
            changePermissions(str[j], status_mode, channel, str_key, key_index, server);
        }
    }
    else {
        //NOT UNDERSTAND FLAG
    }
}