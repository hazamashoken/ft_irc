/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:33:08 by abossel           #+#    #+#             */
/*   Updated: 2023/04/14 00:24:51 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

#define IRC_SPECIAL std::string("-[]\\`^{}")
#define IRC_NONUSER std::string("\0\r\n @", 5)

Message::Message()
{
    expressionInit();
}

Message::~Message()
{
}

void Message::expressionInitHostaddr()
{
    Expression ip4byte;
    ip4byte.exp(Expression().any("2").any("5").any("012345")).jmp();
    ip4byte.exp(Expression().any("2").any("01234").digit()).jmp();
    ip4byte.exp(Expression().any("01").digit(2, 2)).jmp();
    ip4byte.digit(1, 2);

    Expression ip4addr;
    ip4addr.exp(ip4byte).any(".").exp(ip4byte).any(".").exp(ip4byte).any(".").exp(ip4byte);

    Expression ip6pre;
    ip6pre.xdigit(1, 4).any(":");
    Expression ip6suf;
    ip6suf.any(":").xdigit(1, 4);

    Expression ip6pre0;
    ip6pre0.any("0", 1, 4).any(":");
    Expression ip6suf0;
    ip6suf0.any(":").any("0", 1, 4);
    Expression ip6ffff;
    ip6ffff.any(":").any("fF", 4, 4).any(":");

    // IPv6 addresses
    Expression ip6addr;

    // IPv4 mapped addresses 0:0:0:0:0:ffff:a.b.c.d
    ip6addr.exp(Expression().exp(ip6pre0, 4, 4).any("0", 1, 4).exp(ip6ffff).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().exp(ip6pre0, 1, 4).exp(ip6ffff).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().any(":").exp(ip6suf0, 0, 4).exp(ip6ffff).exp(ip4addr)).jmp();

    // IPv4 translated addresses 0:0:0:0:ffff:0:a.b.c.d
    ip6addr.exp(Expression().exp(ip6pre0, 3, 3).any("0", 1, 4).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().exp(ip6pre0, 1, 3).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().any(":").exp(ip6suf0, 0, 3).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();

    // IPv4 standard addresses 0:0:0:0:0:0:0:0
    ip6addr.exp(Expression().exp(ip6pre, 7, 7).xdigit(1, 4)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 6, 6).exp(ip6suf, 1, 1)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 5, 5).exp(ip6suf, 1, 2)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 4, 4).exp(ip6suf, 1, 3)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 3, 3).exp(ip6suf, 1, 4)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 2, 2).exp(ip6suf, 1, 5)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 1, 1).exp(ip6suf, 1, 6)).jmp();
    ip6addr.exp(Expression().any(":").exp(ip6suf, 1, 7)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 1, 7).any(":")).jmp();

    // special case ::
    ip6addr.all("::");

    _hostaddrExp = Expression(_hostaddrStr);
    _hostaddrExp.exp(ip4addr).jmp().exp(ip6addr);
}

void Message::expressionInit()
{
    // nickname expression
    _nickExp = Expression(_nickStr);
    _nickExp.alpha().add(IRC_SPECIAL).exp(Expression().alnum().add(IRC_SPECIAL).add("-"), 0, 8);

    // username expression
    _userExp = Expression(_userStr);
    _userExp.inv(IRC_NONUSER, 1);

    // hostname expression
	Expression shortnameExp;
	shortnameExp.alnum(1).exp(Expression().any("-", 0).alnum(1), 0);
    _hostnameExp = Expression(_hostnameStr);
	_hostnameExp.exp(shortnameExp).exp(Expression().any(".").exp(shortnameExp, 1), 0);

    // hostaddr expression
    expressionInitHostaddr();

    // host expression
    _hostExp = Expression(_hostStr);
    _hostExp.exp(_hostnameExp).jmpe().exp(_hostaddrExp);
}

bool Message::testHost(std::string s)
{
    bool result;
    result = _hostExp.match(s);
    return (result);
}
