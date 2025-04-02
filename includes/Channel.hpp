/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:20:33 by albernar          #+#    #+#             */
/*   Updated: 2025/04/02 16:49:05 by albernar         ###   ########.fr       */
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

	void	addClient(Client *client);
	void	removeClient(int fd);
	bool	isClientInChannel(Client *client) const;

	void	broadcastMessage(Client *client, std::string message) const;
};

#endif