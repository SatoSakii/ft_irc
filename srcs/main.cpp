/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:45:53 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 17:48:28 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdlib>

int main(int argc, char **argv) {
	long		port;
	char		*endPtr;
	std::string	password;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);	
	}
	port = std::strtol(argv[1], &endPtr, 10);
	if (*endPtr != '\0' || port <= 0 || port > 65535)
	{
		std::cerr << "Invalid port number: " << argv[1] << std::endl;
		return (1);
	}
	password = argv[2];
	try {
		Server	server(port, password);
		server.serverInit();
	} catch(const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
