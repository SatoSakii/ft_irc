/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:20:33 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 01:32:21 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <map>

# include "Client.hpp"

class Channel
{
private:
	std::string				name;
	std::map<int, Client*>	clients;

public:
	Channel(std::string name);

	void		addClient(Client *client);
	void		removeClient(int fd);
	bool		isClientInChannel(Client *client) const;
	bool		isInviteOnly(void) const;
	bool		isInvited(Client *client) const;
	bool		isPasswordProtected(void) const;
	bool		isFull(void) const;
	std::string	getPassword(void) const;
	std::string	getTopic(void) const;
	std::string getUserList(void) const;
	std::map<int, Client *> getClients(void) const;
	

	void	broadcastMessage(Client *client, std::string message) const;
};

#endif