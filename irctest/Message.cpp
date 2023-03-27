/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:33:08 by abossel           #+#    #+#             */
/*   Updated: 2023/03/27 12:04:08 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

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
 * returns true if any character in string front is at the front of message
 */
bool Message::isFront(std::string message, std::string front)
{
    if (message.size() > 0 && front.find(message[0]) != std::string::npos)
        return (true);
    return (false);
}

/*
 * returns a string with the first size chars removed
 */
std::string Message::popFront(std::string message, size_t size)
{
    if (message.size() > 0 && message.size() > size)
        return (message.substr(size));
    return ("");
}

/*
 * returns a string with any chars from trim removed from front of string
 */
std::string Message::trimLeft(std::string message, std::string trim)
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

/*
 * returns a string with all chars converted to lower case
 */
std::string Message::toLowerString(std::string message)
{
    std::string low;
    size_t pos;

    for (pos = 0; pos < message.size(); pos++)
        low.push_back(static_cast<char>(tolower(message[pos])));
    return (low);
}

std::string Message::readHostname(std::string message)
{
    std::string temp;
    std::string host;

    temp = toLowerString(message);
    host += trimLeft(temp, "abcdefghijklmnopqrstuvwxyz");

    return (host);
}

bool Message::readPrefix(std::string message)
{
    std::string tmp;

    if (!isFront(message, ":"))
        return (true);
    tmp = popFront(message);
    tmp = trimLeft(tmp, " ");
}