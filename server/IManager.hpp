/*
** EPITECH PROJECT, 2018
** vivoka
** File description:
** Created by elian.nicaise,
*/
#ifndef VIVOKA_IMANAGER_HPP
#define VIVOKA_IMANAGER_HPP

namespace vivoka::manager {

    class IManager {
    public:
        virtual ~IManager() = default;

        virtual void run() = 0;

    };
}

#endif //VIVOKA_IMANAGER_HPP
