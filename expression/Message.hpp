/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:24:28 by abossel           #+#    #+#             */
/*   Updated: 2023/04/13 20:09:45 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include "Expression.hpp"

class Message
{
    public:
        Message();
        ~Message();
        bool testHost(std::string s);

    private:
        void expressionInitHostaddr();
        void expressionInit();

        Expression _nickExp;
        std::string _nickStr;

        Expression _userExp;
        std::string _userStr;

        Expression _hostnameExp;
        std::string _hostnameStr;

        Expression _hostaddrExp;
        std::string _hostaddrStr;

        Expression _hostExp;
        std::string _hostStr;
};

#endif
