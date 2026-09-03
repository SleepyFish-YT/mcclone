//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_REPORTEDEXCEPTION_H
#define MCCLONE_REPORTEDEXCEPTION_H

#include <vcruntime_exception.h>

/**
 * @author SleepyFish
 * @brief This class is used to report exceptions
 */
class ReportedException : public std::exception {

public:

    /*

        const CrashReport crashReport;

        ReportedException(CrashReport crashReport) :
            crashReport(crashReport)
        {}

     */

    char const* what() const override {
        // return crashReport.getCrashReport();
        return "DONT FORGET: ReportedException not implemented yet!!!";
    }

};


#endif //MCCLONE_REPORTEDEXCEPTION_H
