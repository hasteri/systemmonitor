#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours;
    int minutes;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    minutes = seconds / 60;
    seconds = seconds % 60;

    string secondString;
    string minuteString;
    string hoursString;

    if (hours < 10){hoursString = "0" + to_string(hours);}
    else {hoursString = to_string(hours);}
    if (minutes < 10){minuteString = "0" + to_string(minutes);}
    else {minuteString = to_string(minutes);}
    if (seconds < 10){secondString = "0" + to_string(seconds);}
    else {secondString = to_string(seconds);}

    string formattedTime = hoursString + ":" + minuteString + ":" + secondString;
    return formattedTime; 
}