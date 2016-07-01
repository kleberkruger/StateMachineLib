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

#ifndef STATEMACHINELIB_STATEMACHINE_H
#define STATEMACHINELIB_STATEMACHINE_H


#include <map>
#include <fstream>
#include <iostream>

using namespace std;

/* -----------------------------------------------------------------------------
 * Class Declarations
 * ---------------------------------------------------------------------------*/
class TypeIDFactory;
class State;
class RecoveryPoint;
class StateMachine;
class Application;

/* -----------------------------------------------------------------------------
 * Type Definitions
 * ---------------------------------------------------------------------------*/
typedef unsigned long TypeID;

static const char *RECOVERY_FILE = ".state.bin";

/* -----------------------------------------------------------------------------
 * Class Definitions
 * ---------------------------------------------------------------------------*/
class TypeIDFactory {
private:

    static TypeID counter;

    TypeIDFactory() { }

    ~TypeIDFactory() { }

public:

    template<typename T>
    static TypeID getID() {
        static TypeID id = counter++;
        return id;
    }
};

class State {
protected:

    State() { }

    State(State &orig) { }

    virtual ~State() { }

public:

    virtual double getTimeLimit() { return 0.0; }

    virtual void run(StateMachine &sm) = 0;
};

class RecoveryPoint {
protected:

    RecoveryPoint() { }

    RecoveryPoint(RecoveryPoint &orig) { }

    virtual ~RecoveryPoint() { }

public:

    virtual void run(StateMachine &sm) = 0;
};

class StateMachine {
public:

    StateMachine() { }

    StateMachine(StateMachine &orig) :
            recoveryPoints(orig.recoveryPoints), states(orig.states), current(orig.current) { }

    virtual ~StateMachine() { }

    template<class R, class S>
    typename std::enable_if<std::is_base_of<RecoveryPoint, R>::value &&
                            std::is_base_of<State, S>::value, void>::type addRecoveryPoint() {

        TypeID id = TypeIDFactory::getID<S>();
        recoveryPoints[id] = new R;
    };

    template<class S>
    typename std::enable_if<std::is_base_of<State, S>::value, void>::type setToState() {
        StateMachine::current = TypeIDFactory::getID<S>();
        serializeToFlash(current);
        if (states[current] == NULL) {
            addState<S>();
        }
    };

    template<class S>
    typename std::enable_if<std::is_base_of<State, S>::value, void>::type start() {
        setToState<S>();
        run();
    };

    bool recoveryFrom(TypeID id) {
        RecoveryPoint *recoveryPoint = recoveryPoints[id];
        if (recoveryPoint == NULL) {
            return false;
        }
        recoveryPoints[id]->run(*this);
        return true;
    };

private:

    std::map<TypeID, RecoveryPoint *> recoveryPoints;
    std::map<TypeID, State *> states;
    TypeID current;

    template<class S>
    typename std::enable_if<std::is_base_of<State, S>::value, void>::type addState() {
        TypeID id = TypeIDFactory::getID<S>();
        states[id] = new S;
    };

    void run() {
        static bool RUNNING = true;
        while (RUNNING) {
            states[current]->run(*this);
        }
    }

    void serializeToFlash(TypeID id) {
        ofstream outstream(RECOVERY_FILE, ios::binary);
        outstream << id;
        outstream.close();
    }
};

class Application {
protected:

    Application() { }

    Application(Application &orig) { }

    virtual ~Application() { }

public:

    template<class App = Application>
    static typename std::enable_if<std::is_base_of<Application, App>::value, App *>::type getInstance() {
        return (App *) instance;
    };

    template<class App>
    static typename std::enable_if<std::is_base_of<Application, App>::value, void>::type initialize() {
        if (instance != NULL) {
            throw "Application has already been initialized";
        }

        static StateMachine sm;

        instance = new App;
        instance->setup();
        instance->createRecoveryPoints(sm);

        static bool RESET_BY_FAULT = true;
        bool started = false;
        if (RESET_BY_FAULT && instance->recovery(sm)) {
            started = true;
        }
        if (!started) {
            instance->start(sm);
        }
    }

//    void serializeToFlash(TypeID id) {
//        std::cout << "Current state: " << id << std::endl;
//        ofstream outstream(RECOVERY_FILE, ios::binary);
//        outstream << id;
//        outstream.close();
//    }

    TypeID unserializeFromFlash() {
        TypeID id;
        ifstream instream(RECOVERY_FILE, ios::binary);
        instream >> id;
        instream.close();
        return id;
    }

protected:

    virtual void setup() { }

    virtual void createRecoveryPoints(StateMachine &sm) { }

    virtual void start(StateMachine &sm) = 0;

private:

    static Application *instance;

    bool recovery(StateMachine &sm) {
        TypeID id = unserializeFromFlash();
        return sm.recoveryFrom(id);
    }
};

#endif //STATEMACHINELIB_STATEMACHINE_H
