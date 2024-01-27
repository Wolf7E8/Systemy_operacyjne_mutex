#include "carThread.h"

int carThread::counter{};

carThread::carThread(): ID(counter+1)
{
    counter++;
}

int carThread::getID()
{
    return this->ID;
}