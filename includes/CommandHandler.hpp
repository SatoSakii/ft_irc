/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:00:08 by albernar          #+#    #+#             */
/*   Updated: 2025/04/02 18:47:54 by albernar         ###   ########.fr       */
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
	void	nickCommand(Client *client, IRCCommand ircCommand);
	void	userCommand(Client *client, IRCCommand ircCommand);
	void	passCommand(Client *&client, IRCCommand ircCommand);
	void	quitCommand(Client *&client, IRCCommand ircCommand);
};

# define ERR_MESSAGE(message) ("ERROR :" + message + "\r\n")
# define ERR_CLOSELINK(nickname, message) ("ERROR :Closing Link: " + nickname + " " + message + "\r\n")
# define ERR_NONICKNAMEGIVEN(serverIp, nickname) (IRCResponse::error((serverIp), 431, (nickname), "No nickname given"))
# define ERR_ERRONEUSNICKNAME(serverIp, nickname) (IRCResponse::error((serverIp), 432, (nickname), "Erroneous nickname"))
# define ERR_NICKNAMEINUSE(serverIp, nickname) (IRCResponse::error((serverIp), 433, (nickname), "Nickname is already in use"))
# define ERR_NEEDMOREPARAMS(serverIp, command, nickname) (IRCResponse::error((serverIp), 461, (nickname) + " " + (command), "Not enough parameters"))
# define ERR_PASSWDMISMATCH(serverIp) (IRCResponse::error((serverIp), 464, "*", "Password mismatch"))
# define ERR_ALREADYREGISTERED(serverIp, nickname) (IRCResponse::error((serverIp), 462, (nickname), "You may not reregister"))
# define ERR_NOTREGISTERED(serverIp, nickname) (IRCResponse::error((serverIp), 451, (nickname), "You have not registered"))

# define RPL_QUIT(nickname, username, clientHost, reason) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " QUIT :" + (reason) + "\r\n")
# define RPL_CHANGENICK(oldnick, nickname, username, clientHost) (":" + (oldnick) + "!" + (username) + "@" + (clientHost) + " NICK " + (nickname) + "\r\n")
#endif