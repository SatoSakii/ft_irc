/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:00:08 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 18:26:24 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP
# include "Client.hpp"
# include "IRCUtils.hpp"
# include "Server.hpp"

class Server;

class CommandHandler
{
private:
	Server *server;
public:
	CommandHandler(Server *server);
	void nickCommand(Client *client, IRCCommand ircCommand);
};

#endif