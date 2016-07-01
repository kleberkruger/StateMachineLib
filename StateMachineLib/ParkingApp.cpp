/*
 * Created by Kleber Kruger on 30/06/16.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ParkingApp.h"

/* -----------------------------------------------------------------------------
 * WaitingCarArrive Class
 * ---------------------------------------------------------------------------*/
void WaitingCarArrive::run(StateMachine &sm) {
    char c;
    Application::getInstance<ParkingApp>()->printMenu(TypeIDFactory::getID<WaitingCarArrive>());
    std::cin >> c;

    switch (c) {
        case 'C':
        case 'c':
            sm.setToState<WaitingPressButton>();
    }
}

/* -----------------------------------------------------------------------------
 * WaitingPressButton Class
 * ---------------------------------------------------------------------------*/
void WaitingPressButton::run(StateMachine &sm) {
    char c;
    Application::getInstance<ParkingApp>()->printMenu(TypeIDFactory::getID<WaitingPressButton>());
    std::cin >> c;

    switch (c) {
        case 'T':
        case 't':
            openGateway();
            sm.setToState<WaitingCarEnter>();
            break;

        case 'V':
        case 'v':
            sm.setToState<WaitingCarArrive>();
            break;

        default:
            std::cout << "Opção inválida. Tente novamente..." << std::endl;
            break;
    }
}

void WaitingPressButton::openGateway() {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "\tOpening gateway..." << std::endl;
//    std::cout << "--------------------------------------------------" << std::endl;
}

/* -----------------------------------------------------------------------------
 * WaitingCarEnter Class
 * ---------------------------------------------------------------------------*/
void WaitingCarEnter::run(StateMachine &sm) {
    char c;
    Application::getInstance<ParkingApp>()->printMenu(TypeIDFactory::getID<WaitingCarEnter>());
    std::cin >> c;
    switch (c) {
        case 'E':
        case 'e':
            sm.setToState<CloseGateway>();
            break;

        case 'V':
        case 'v':
            sm.setToState<WaitingCarArrive>();
            break;
    }
}

/* -----------------------------------------------------------------------------
 * CloseGateway Class
 * ---------------------------------------------------------------------------*/
void CloseGateway::run(StateMachine &sm) {
    closeGateway();
    sm.setToState<WaitingCarArrive>();
}

void CloseGateway::closeGateway() {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "\tClosing gateway..." << std::endl;
//    std::cout << "--------------------------------------------------" << std::endl;
}

/* -----------------------------------------------------------------------------
 * RecoveryToWaitingPressButton Class
 * ---------------------------------------------------------------------------*/
void RecoveryToWaitingPressButton::run(StateMachine &sm) {
    sm.start<WaitingPressButton>();
}

/* -----------------------------------------------------------------------------
 * RecoveryToWaitingCarEnter Class
 * ---------------------------------------------------------------------------*/
void RecoveryToWaitingCarEnter::run(StateMachine &sm) {
    bool GATEWAY_OPENED = false;
    if (!GATEWAY_OPENED) {
        openGateway();
    }
    sm.start<WaitingCarEnter>();
}

void RecoveryToWaitingCarEnter::openGateway() {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "\tOpening gateway..." << std::endl;
//    std::cout << "--------------------------------------------------" << std::endl;
}

/* -----------------------------------------------------------------------------
 * RecoveryToCloseGateway Class
 * ---------------------------------------------------------------------------*/
void RecoveryToCloseGateway::run(StateMachine &sm) {
    bool GATEWAY_OPENED = true;
    if (GATEWAY_OPENED) {
        closeGateway();
    }
    sm.start<WaitingCarEnter>();
}

void RecoveryToCloseGateway::closeGateway() {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "\tClosing gateway..." << std::endl;
//    std::cout << "--------------------------------------------------" << std::endl;
}

/* -----------------------------------------------------------------------------
 * ParkingApp Class
 * ---------------------------------------------------------------------------*/
void ParkingApp::createRecoveryPoints(StateMachine &sm) {
    sm.addRecoveryPoint<RecoveryToWaitingPressButton, WaitingPressButton>();
    sm.addRecoveryPoint<RecoveryToWaitingCarEnter, WaitingCarEnter>();
    sm.addRecoveryPoint<RecoveryToCloseGateway, CloseGateway>();
}

void ParkingApp::start(StateMachine &sm) {

//    sm.addRecoveryPoint<RecoveryToWaitingCarEnter, WaitingCarEnter>();
//    sm.addRecoveryPoint<RecoveryToCloseGateway, CloseGateway>();

    sm.start<WaitingCarArrive>();
}

void ParkingApp::printMenu(TypeID id) {
    std::cout << "--------------------------------------------------" << std::endl;
    if (id == TypeIDFactory::getID<WaitingCarArrive>()) {
        std::cout << "\t[C] - Simular um carro chegando" << std::endl;
    } else if (id == TypeIDFactory::getID<WaitingPressButton>()) {
        std::cout << "\t[T] - Imprimir o ticket" << std::endl;
        std::cout << "\t[V] - Voltar" << std::endl;
    } else if (id == TypeIDFactory::getID<WaitingCarEnter>()) {
        std::cout << "\t[E] - Entrar" << std::endl;
        std::cout << "\t[V] - Voltar" << std::endl;
    }
    std::cout << "--------------------------------------------------" << std::endl;
}