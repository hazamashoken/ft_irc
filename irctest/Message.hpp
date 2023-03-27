/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:24:28 by abossel           #+#    #+#             */
/*   Updated: 2023/03/27 11:20:29 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>

class Message
{
    public:
        bool readMessage(std::string message);
        void printMessage();

    private:
        bool isFront(std::string message, std::string front);
        std::string popFront(std::string message, size_t size = 1);
        std::string trimLeft(std::string message, std::string trim);
        std::string toLowerString(std::string message);
        std::string readHostname(std::string message);
        bool readPrefix(std::string message);

        // prefix data
        std::string _servername;
        std::string _nick;
        std::string _user;
        std::string _host;
};

#endif
