/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:24:12 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 19:45:39 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream> 
# include <sys/socket.h>
# include "IRCUtils.hpp"

class Client
{
private:
	int			fd;
	bool		auth;
	short		authPhase;
	std::string	bufferMessage;
	std::string	hostname;

	std::string	nickname;
	std::string	username;

public:
	int			getFd(void) const;
	bool		isAuth(void) const;
	void		setAuth(bool auth);
	short		getAuthPhase(void) const;
	void		setAuthPhase(short authPhase, IRCCommand ircCommand, std::string password = "");

	std::string	getHostname(void) const;

	void		appendToBuffer(std::string message);
	std::string	getBuffer(void) const;
	void		clearBuffer(void);

	std::string	getNickname(void) const;
	void		setNickname(std::string nickname);
	std::string	getUsername(void) const;
	void		setUsername(std::string username);

	void		sendMessage(std::string message);

	Client(int fd, std::string hostname);
};

#endif