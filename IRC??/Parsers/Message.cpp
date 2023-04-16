/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:33:08 by abossel           #+#    #+#             */
/*   Updated: 2023/04/16 17:57:21 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

#define IRC_SPECIAL std::string("-[]\\`^{}")
#define IRC_TARGET std::string("-.!@*?\\")
#define IRC_NONUSER std::string("\0\r\n @", 5)
#define IRC_NONPASS std::string("\0\r\n \t", 5)
#define IRC_NONPARAM std::string("\0\r\n :", 5)
#define IRC_NONREAL std::string("\0\r\n:", 4)
#define IRC_NONCHANNEL std::string("\0\a\r\n ,:", 7)

Message::Message()
{
    expressionInit();
    messageInit();
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
    _nicknameExp = Expression(_nicknameStr);
    _nicknameExp.alpha().add(IRC_SPECIAL).exp(Expression().alnum().add(IRC_SPECIAL).add("-"), 0, 8);

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

    // servername expression
    _servernameExp = Expression(_hostnameExp, _servernameStr);

    // prefix expression
    _prefixExp = Expression(_prefixStr);
    _prefixExp.exp(Expression().exp(_nicknameExp).any("!").exp(_userExp).any("@").exp(_hostExp)).jmp();
	_prefixExp.exp(Expression().exp(_nicknameExp).any("@").exp(_hostExp)).jmp();
	_prefixExp.exp(_nicknameExp).jmpe();
    _prefixExp.exp(_servernameExp);

    // password expression
    _passwordExp = Expression(_passwordStr);
    _passwordExp.inv(IRC_NONPASS, 5);

    // bitmode expression
    _bitmodeExp = Expression(_bitmodeStr);
    _bitmodeExp.digit();

    // unused expression
    _unusedExp = Expression(_unusedStr);
    _unusedExp.inv(IRC_NONPARAM, 1);

    // realname expression
    _realnameExp = Expression(_realnameStr);
    _realnameExp.inv(IRC_NONREAL, 1);

    // mode expression
    _modeExp = Expression(_modeStr);
    _modeExp.any("+-").any("iwoOrs");

    // wildname expression
    _targetExp = Expression(_targetStr);
    _targetExp.alnum(1).add(IRC_TARGET);

    // info expression
    _infoExp = Expression(_infoStr);
    _infoExp.inv(IRC_NONREAL, 1);

    // channel expression
    _channelExp = Expression(_channelStr);
    _channelExp.exp(Expression().all("!").upper(5, 5).add("0123456789")).jmp();
    _channelExp.any("#+&").con();
    _channelExp.inv(IRC_NONCHANNEL, 1);
    _channelExp.exp(Expression().all(":").inv(IRC_NONCHANNEL, 1), 0, 1);

    // channel list expression
    _channelListExp = Expression(_channelListStr);
    _channelListExp.all("0").jmp();
    _channelListExp.exp(_channelExp).exp(Expression().all(",").exp(_channelExp), 0);

    // key list expression
    _keyListExp = Expression(_keyListStr);
    _keyListExp.exp(_passwordExp, 1).exp(Expression().all(",").exp(_passwordExp), 0);

    // channel mode expression
    _channelModeExp = Expression(_channelModeStr);
    _channelModeExp.any("eIO").jmp();
    _channelModeExp.any("+-").any("OvaimnqpsrtklbeI", 1);
    _channelModeExp.exp(Expression().all(" ").exp(_targetExp), 0);

    // channel mode list expression
    _channelModeListExp = Expression(_channelModeListStr);
    _channelModeListExp.exp(_channelModeExp);
    _channelModeListExp.exp(Expression().all(" ").exp(_channelModeExp), 0);

    // topic info expression
    _topicExp = Expression(_topicStr);
    _topicExp.all(":").exp(_infoExp, 0, 1);

    // user list expression
    _userListExp = Expression(_userListStr);
    _userListExp.exp(_userExp);
    _userListExp.exp(Expression().all(",").exp(_userExp), 0);
}

void Message::messageInit()
{
    // PASS message
    _passMsgExp = Expression(_messageStr);
    _passMsgExp.all("PASS ").exp(_passwordExp).all("\r\n");

    // NICK message
    _nickMsgExp = Expression(_messageStr);
    _nickMsgExp.all("NICK ").exp(_nicknameExp).all("\r\n");

    // USER message
    _userMsgExp = Expression(_messageStr);
    _userMsgExp.all("USER ").exp(_userExp).all(" ");
    _userMsgExp.exp(Expression().exp(_bitmodeExp).all(" ").exp(_unusedExp)).jmp();
    _userMsgExp.exp(Expression().exp(_hostnameExp).all(" ").exp(_servernameExp)).con();
    _userMsgExp.all(" :").exp(_realnameExp).all("\r\n");

    // OPER message
    _operMsgExp = Expression(_messageStr);
    _operMsgExp.all("OPER ").exp(_userExp);
    _operMsgExp.all(" ").exp(_passwordExp).all("\r\n");

    // MODE message
    _modeMsgExp = Expression(_messageStr);
    _modeMsgExp.all("MODE ").exp(_modeExp).all("\r\n");

    // SERVICE message
    _serviceMsgExp = Expression(_messageStr);
    _serviceMsgExp.all("SERVICE ").exp(_nicknameExp);
    _serviceMsgExp.all(" ").exp(_unusedExp);
    _serviceMsgExp.all(" ").exp(_targetExp);
    _serviceMsgExp.all(" ").exp(_unusedExp);
    _serviceMsgExp.all(" ").exp(_unusedExp);
    _serviceMsgExp.all(" :").exp(_infoExp).all("\r\n");

    // QUIT message
    _quitMsgExp = Expression(_messageStr);
    _quitMsgExp.all("QUIT :").exp(_infoExp).all("\r\n");

    // SQUIT message
    _squitMsgExp = Expression(_messageStr);
    _squitMsgExp.all("SQUIT ").exp(_servernameExp);
    _squitMsgExp.all(" :").exp(_infoExp).all("\r\n");

    // channel JOIN message
    _joinMsgExp = Expression(_messageStr);
    _joinMsgExp.all("JOIN ").exp(_channelListExp);
    _joinMsgExp.exp(Expression().all(" ").exp(_keyListExp), 0, 1).all("\r\n");

    // channel PART message
    _partMsgExp = Expression(_messageStr);
    _partMsgExp.all("PART ").exp(_channelListExp);
    _partMsgExp.exp(Expression().all(" :").exp(_infoExp), 0, 1).all("\r\n");

    // channel MODE message
    _cmodeMsgExp = Expression(_messageStr);
    _cmodeMsgExp.all("MODE ").exp(_channelExp);
    _cmodeMsgExp.all(" ").exp(_channelModeListExp).all("\r\n");

    // channel TOPIC message
    _topicMsgExp = Expression(_messageStr);
    _topicMsgExp.all("TOPIC").exp(Expression().all(" ").exp(_topicExp), 0, 1).all("\r\n");

    // channel NAMES message
    _namesMsgExp = Expression(_messageStr);
    _namesMsgExp.all("NAMES").exp(Expression().all(" ").exp(_channelListExp).
        exp(Expression().all(" ").exp(_targetExp), 0, 1), 0, 1).all("\r\n");

    // channel LIST message
    _listMsgExp = Expression(_messageStr);
    _listMsgExp.all("LIST").exp(Expression().all(" ").exp(_channelListExp).
        exp(Expression().all(" ").exp(_targetExp), 0, 1), 0, 1).all("\r\n");

    // channel INVITE message
    _inviteMsgExp = Expression(_messageStr);
    _inviteMsgExp.all("INVITE ").exp(_nicknameExp);
    _inviteMsgExp.all(" ").exp(_channelExp).all("\r\n");

    // channel KICK message
    _kickMsgExp = Expression(_messageStr);
    _kickMsgExp.all("KICK ").exp(_channelListExp);
    _kickMsgExp.all(" ").exp(_userListExp);
    _kickMsgExp.exp(Expression().all(" :").exp(_infoExp), 0, 1).all("\r\n");
}

void Message::stringClear()
{
    _messageStr.clear();
    _nicknameStr.clear();
    _userStr.clear();
    _hostnameStr.clear();
    _hostaddrStr.clear();
    _hostStr.clear();
    _servernameStr.clear();
    _prefixStr.clear();
    _passwordStr.clear();
    _bitmodeStr.clear();
    _unusedStr.clear();
    _realnameStr.clear();
    _modeStr.clear();
    _targetStr.clear();
    _infoStr.clear();
    _channelStr.clear();
    _channelListStr.clear();
    _keyListStr.clear();
    _channelModeStr.clear();
    _channelModeListStr.clear();
    _topicStr.clear();
    _userListStr.clear();
}

std::string Message::getNickname() const
{
    return (_nicknameStr);
}

std::string Message::getUser() const
{
    return (_userStr);
}

std::string Message::getHostname() const
{
    return (_hostaddrStr);
}

std::string Message::getHostaddr() const
{
    return (_hostaddrStr);
}

std::string Message::getHost() const
{
    return (_hostStr);
}

std::string Message::getServername() const
{
    return (_servernameStr);
}

std::string Message::getPrefix() const
{
    return (_prefixStr);
}

std::string Message::getPassword() const
{
    return(_passwordStr);
}

std::string Message::getBitmode() const
{
    return(_bitmodeStr);
}

std::string Message::getUnused() const
{
    return(_unusedStr);
}

std::string Message::getRealname() const
{
    return(_realnameStr);
}

std::string Message::getMode() const
{
    return(_modeStr);
}

std::string Message::getTarget() const
{
    return(_targetStr);
}

std::string Message::getInfo() const
{
    return(_infoStr);
}

std::string Message::getChannel() const
{
    return(_channelStr);
}

std::string Message::getChannelList() const
{
    return(_channelListStr);
}

std::string Message::getKeyList() const
{
    return(_keyListStr);
}

std::string Message::getChannelModeList() const
{
    return(_channelModeListStr);
}

std::string Message::getTopic() const
{
    return(_topicStr);
}

std::string Message::getUserList() const
{
    return(_userListStr);
}

int Message::getMsgType(std::string message)
{
    stringClear();
    if (_passMsgExp.match(message))
        return (IRC_PASS);
    if (_nickMsgExp.match(message))
        return (IRC_NICK);
    if (_userMsgExp.match(message))
        return (IRC_USER);
    if (_operMsgExp.match(message))
        return (IRC_OPER);
    if (_modeMsgExp.match(message))
        return (IRC_MODE);
    if (_serviceMsgExp.match(message))
        return (IRC_SERVICE);
    if (_quitMsgExp.match(message))
        return (IRC_QUIT);
    if (_squitMsgExp.match(message))
        return (IRC_SQUIT);
    if (_joinMsgExp.match(message))
        return (IRC_JOIN);
    if (_partMsgExp.match(message))
        return (IRC_PART);
    if (_cmodeMsgExp.match(message))
        return (IRC_CMODE);
    if (_topicMsgExp.match(message))
    {
        if (!_topicStr.empty() && _topicStr[0] == ':')
            _topicStr.erase(0, 1);
        return (IRC_TOPIC);
    }
    if (_namesMsgExp.match(message))
        return (IRC_NAMES);
    if (_listMsgExp.match(message))
        return (IRC_LIST);
    if (_inviteMsgExp.match(message))
        return (IRC_INVITE);
    if (_kickMsgExp.match(message))
        return (IRC_KICK);
    return (IRC_INVALID);
}

std::string Message::getMsgStr() const
{
    return (_messageStr);
}
