/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com> (This project started from Jan., 2018.)
* This file is part of [BLITZ (Behavioral Learning In The Zebrafish)](https://github.com/Wenlab/BLITZ),
* which is adapted from MindControl (Andrew Leifer et al., 2011) <leifer@fas.harvard.edu>
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: errorHandling.h
* Abstract: this file contains all declarations of functions that
*			used handle devices errors
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Apr. 15, 2019
* Modified on: Apr. 20, 2019
*/

#ifndef ERRORHANDLING_H_DEF
#define ERRORHANDLING_H_DEF



#include <iostream>
#include <string>

void waitUserInput2exit();


/* catch NULL value with any datatype*/
template <typename T>
void tryCatchNull(T value, std::string exceptionString)
{
    if (value == NULL)
    {
        std::cout << exceptionString << std::endl;
        waitUserInput2exit();
    }
};

/* catch false value with any datatype*/
template <typename T>
void tryCatchFalse(T value, std::string exceptionString)
{
    if (value == false)
    {
        std::cout << exceptionString << std::endl;
        waitUserInput2exit();
    }
};

/* catch empty value with any datatype; add this method if needed*/
template <typename T>
void tryCatchEmpty(T value, std::string exceptionString)
{
    if (value.size() == 0)
    {
        std::cout << exceptionString << std::endl;
        waitUserInput2exit();
    }
};

/* Case insensitive comparasion
Adapted from Timmmm, https://stackoverflow.com/a/4119881
*/
//bool iequals(const std::string& a, const std::string& b);




#endif // ERRORHANDLING_H_DEF
