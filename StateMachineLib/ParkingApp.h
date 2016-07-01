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

#ifndef STATEMACHINELIB_PARKINGAPP_H
#define STATEMACHINELIB_PARKINGAPP_H


#include <iostream>
#include "StateMachine.h"

using namespace std;

/* -----------------------------------------------------------------------------
 * Class Declarations
 * ---------------------------------------------------------------------------*/
class WaitingCarArrive;

class WaitingPressButton;

class WaitingCarEnter;

class CloseGateway;

/* -----------------------------------------------------------------------------
 * Class Definitions
 * ---------------------------------------------------------------------------*/
class WaitingCarArrive : public State {
public:
    void run(StateMachine &sm);
};

class WaitingPressButton : public State {
public:
    void run(StateMachine &sm);

private:
    void openGateway();
};

class WaitingCarEnter : public State {
public:
    void run(StateMachine &sm);
};

class CloseGateway : public State {
public:
    void run(StateMachine &sm);

private:
    void closeGateway();
};

class RecoveryToWaitingPressButton : public RecoveryPoint {
public:
    void run(StateMachine &sm);
};

class RecoveryToWaitingCarEnter : public RecoveryPoint {
public:
    void run(StateMachine &sm);

private:
    void openGateway();
};

class RecoveryToCloseGateway : public RecoveryPoint {
public:
    void run(StateMachine &sm);

private:
    void closeGateway();
};

class ParkingApp : public Application {
protected:
    virtual void createRecoveryPoints(StateMachine &sm);

private:
    void start(StateMachine &sm);

public:
    void printMenu(TypeID id);
};


#endif //STATEMACHINELIB_PARKINGAPP_H
