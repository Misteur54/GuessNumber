/*
** EPITECH PROJECT, 2018
** vivoka
** File description:
** Created by elian.nicaise,
*/

#ifndef VIVOKA_SERVERMANAGER_HPP
#define VIVOKA_SERVERMANAGER_HPP

#include <bits/unique_ptr.h>
#include "IManager.hpp"

namespace vivoka::manager {

    constexpr unsigned short defaultPort = 4242;

    class ServerManager : public IManager {
        public:
        ServerManager() = delete;
        explicit  ServerManager(const unsigned short port);
        ~ServerManager() final = default;

        void run() final;

        private:
        std::unique_ptr<char> _server;
    };
}

#endif //VIVOKA_SERVERMANAGER_HPP
