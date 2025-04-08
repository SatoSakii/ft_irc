/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:00:08 by albernar          #+#    #+#             */
/*   Updated: 2025/04/08 17:11:51 by albernar         ###   ########.fr       */
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
	void	joinCommand(Client *client, IRCCommand ircCommand);
	void	partCommand(Client *client, IRCCommand ircCommand);
	void	topicCommand(Client *client, IRCCommand ircCommand);
	void	privmsgCommand(Client *client, IRCCommand ircCommand);
	void	passCommand(Client *&client, IRCCommand ircCommand);
	void	quitCommand(Client *&client, IRCCommand ircCommand);
	void	inviteCommand(Client *client, IRCCommand ircCommand);
	void	modeCommand(Client *client, IRCCommand ircCommand);
	void	kickCommand(Client *client, IRCCommand ircCommand);
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
# define ERR_NOSUCHCHANNEL(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 403, (nickname) + " " + (channelName), "No such channel"))
# define ERR_USERNOTINCHANNEL(serverIp, channelName, nickname, target) (IRCResponse::error((serverIp), 441, (nickname) + " " + (target) + " " + (channelName), "They aren't on that channel"))
# define ERR_NOSUCHNICK(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 401, (nickname) + " " + (channelName), "No such nick/channel"))
# define ERR_USERONCHANNEL(serverIp, channelName, nickname, target) (IRCResponse::error((serverIp), 443, (nickname) + " " + (target) + " " + (channelName), "is already on channel"))
# define ERR_INVITEONLYCHAN(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 473, (nickname) + " " + (channelName), "Cannot join channel (+i)"))
# define ERR_BADCHANNELKEY(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 475, (nickname) + " " + (channelName), "Cannot join channel (+k)"))
# define ERR_CHANNELISFULL(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 471, (nickname) + " " + (channelName), "Cannot join channel (+l)"))
# define ERR_NOTONCHANNEL(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 442, (nickname) + " " + (channelName), "You're not on that channel"))
# define ERR_CHANOPRIVSNEEDED(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 482, (nickname) + " " + (channelName), "You're not channel operator"))
# define ERR_CANNOTSENDTOCHAN(serverIp, channelName, nickname) (IRCResponse::error((serverIp), 404, (nickname) + " " + (channelName), "Cannot send to channel"))
# define ERR_NORECIPIENT(serverIp, command, nickname) (IRCResponse::error((serverIp), 411, (nickname) + " " + (command), "No recipient given (" + (command) + ")"))
# define ERR_UNKNOWNMODE(serverIp, channelName, nickname, command) (IRCResponse::error((serverIp), 472, (nickname) + " " + (command), "is unknown mode char to me for " + (channelName)))

# define RPL_TOPICWHOTIME(serverIp, channelName , username, clientHost, nickname, authorname, time) (":" + (serverIp) + " 333 " + (nickname) + " " + (channelName) + " " + (authorname) + "!" + (username) + "@" + (clientHost) + " " + (time) + "\r\n")
# define RPL_PRIVMSG(nickname, username, clientHost, target, message) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " PRIVMSG " + (target) + " :" + (message) + "\r\n")
# define RPL_PART(nickname, username, clientHost, channelName) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " PART :" + (channelName) + "\r\n")
# define RPL_JOIN(nickname, username, clientHost, channelName) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " JOIN :" + (channelName) + "\r\n")
# define RPL_NOTOPIC(server, nickname, channel) (":" + (server) + " 331 " + (nickname) + " " + (channel) + " :No topic is set\r\n")
# define RPL_TOPIC(server, nickname, channel, topic) (":" + (server) + " 332 " + (nickname) + " " + (channel) + " :" + (topic) + "\r\n")
# define RPL_NAMREPLY(serverIp, nickname, channelName, userList) (":" + (serverIp) + " 353 " + (nickname) + " = " + (channelName) + " :" + (userList) + "\r\n")
# define RPL_ENDOFNAMES(serverIp, nickname, channelName) (":" + (serverIp) + " 366 " + (nickname) + " " + (channelName) + " :End of /NAMES list\r\n")
# define RPL_QUIT(nickname, username, clientHost, reason) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " QUIT :" + (reason) + "\r\n")
# define RPL_CHANGENICK(oldnick, nickname, username, clientHost) (":" + (oldnick) + "!" + (username) + "@" + (clientHost) + " NICK " + (nickname) + "\r\n")
# define RPL_INVITING(serverIp, nickname, target, channelName) (":" + (serverIp) + " 341 " + (nickname) + " " + (target) + " " + (channelName) + "\r\n")
# define RPL_INVITED(nickname, username, clientHost, target, channelName) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " INVITE " + (target) + " :" + (channelName) + "\r\n")
# define RPL_KICK(nickname, username, clientHost, target, channelName, reason) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " KICK " + (channelName) + " " +(target) + " :" + (reason) + "\r\n")
# define RPL_MODE(nickname, username, clientHost, channelName, param) (":" + (nickname) + "!" + (username) + "@" + (clientHost) + " MODE " + (channelName) + " " + (param) + "\r\n")
# define RPL_CHANNELMODEIS(serverIp, channelName ,nickname, string) (":" + (serverIp) + " 324 " + (nickname) + " " + (channelName) + " " + (string) + "\r\n")

#endif