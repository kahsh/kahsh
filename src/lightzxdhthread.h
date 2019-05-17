// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2019 The Dilithium Core developers
// Copyright (c) 2019 The Kahsh Core developers

// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//

#ifndef KAHSH_LIGHTZKSHTHREAD_H
#define KAHSH_LIGHTZKSHTHREAD_H

#include <atomic>
#include "genwit.h"
#include "accumulators.h"
#include "concurrentqueue.h"
#include "chainparams.h"
#include <boost/function.hpp>
#include <boost/thread.hpp>

extern CChain chainActive;
// Max amount of computation for a single request
const int COMP_MAX_AMOUNT = 60 * 24 * 60;


/****** Thread ********/

class CLightWorker{

private:

    concurrentqueue<CGenWit> requestsQueue;
    std::atomic<bool> isWorkerRunning;
    boost::thread threadIns;

public:

    CLightWorker() {
        isWorkerRunning = false;
    }

    enum ERROR_CODES {
        NOT_ENOUGH_MINTS = 0,
        NON_DETERMINED = 1
    };

    bool addWitWork(CGenWit wit) {
        if (!isWorkerRunning) {
            LogPrintf("%s not running trying to add wit work \n", "kahsh-light-thread");
            return false;
        }
        requestsQueue.push(wit);
        return true;
    }

    void StartLightZxdhThread(boost::thread_group& threadGroup) {
        LogPrintf("%s thread start\n", "kahsh-light-thread");
        threadIns = boost::thread(boost::bind(&CLightWorker::ThreadLightZKSHSimplified, this));
    }

    void StopLightZxdhThread() {
        threadIns.interrupt();
        LogPrintf("%s thread interrupted\n", "kahsh-light-thread");
    }

private:

    void ThreadLightZKSHSimplified();

    void rejectWork(CGenWit& wit, int blockHeight, uint32_t errorNumber);

};

#endif //KAHSH_LIGHTZKSHTHREAD_H
