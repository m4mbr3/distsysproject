//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "FailureManager.h"
#include <cstdlib>

void FailureManager::initialize()
{
    fail = false;
    seed =rand();
}


void FailureManager::handleMessage(cMessage *msg)
{
    // It doesn't do anything in this class
}

bool FailureManager::isFailed()
{
    //this function return if the client is failed or not
    //It changes every minute the state
    //someone nodes will be up at even minutes and down for the odd minues
    //for the others is true the viceversa. Everything is dipendent from the seed compute at initialized time

    int secret;
    struct tm *t;
    now = time(0);
    t = gmtime(&now)
        if (seed % 2 == 0){
            //even
            if (t->tm_min %2 == 0) return false;
            else return true;
        }
        else{
            if (t->tm_min %2 == 0) return true;
            else return false;
        }
}


