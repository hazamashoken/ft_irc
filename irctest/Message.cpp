/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:33:08 by abossel           #+#    #+#             */
/*   Updated: 2023/03/28 09:34:28 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

#define IRC_LETTER "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define IRC_NUMBER "0123456789"
#define IRC_SPECIAL "-[]\\`^{}"
#define IRC_HOSTEX ".-"
#define IRC_HOST (IRC_LETTER IRC_NUMBER IRC_HOSTEX)
#define IRC_NICK (IRC_LETTER IRC_NUMBER IRC_SPECIAL)

bool Message::readMessage(std::string message)
{
    readPrefix(message);
}

void Message::printMessage()
{
    std::cout << "Prefix:" << std::endl;
    std::cout << "  Servername: " << _servername << std::endl;
    std::cout << "  Nick: " << _nick << std::endl;
    std::cout << "  User: " << _user << std::endl;
    std::cout << "  Host: " << _host << std::endl;
}

/*
 * returns a string with the first size chars removed
 */
std::string Message::popFront(std::string message)
{
    if (message.size() > 0)
        return (message.substr(1));
    return (message);
}

/*
 * returns a string with any chars from trim removed from front of string
 */
std::string Message::trimFront(std::string message, std::string trim)
{
    size_t pos;

    pos = 0;
    while (pos < message.size())
    {
        if (trim.find(message[pos]) == std::string::npos)
            break;
        pos++;
    }
    return (message.substr(pos));
}

bool Message::readPrefix(std::string message)
{
    std::string s;
    size_t pos;
    size_t end;
    size_t next;

    pos = 0;
    s = message;
    if (s.compare(pos, 1, ":") != 0)
        return (true);
    pos++;
    next = s.find_first_not_of(IRC_LETTER IRC_NUMBER IRC_HOSTEX IRC_SPECIAL, pos);
    if 
    // read hostname or nick
    pos = next;
}