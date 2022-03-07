/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com>
* This file is part of BLITZ (Behavioral Learning In The Zebrafish),
* which is adapted from MindControl (Andrew Leifer et al <leifer@fas.harvard.edu>
* Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
* 	"Optogenetic manipulation of neural activity with high spatial resolution in
*	freely moving Caenorhabditis elegans," Nature Methods, Submitted (2010).
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: errorHandling.cpp
* Abstract: this file contains all function definitions
*			used handle devices errors
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>

* Created on: Apr. 17, 2018
*/

#include "errorHandling.h"

using namespace std;

void waitUserInput2exit()
{
	do
	{
		cout << '\n'
			<< "Please check the error message and press a key to exit...";
	} while (cin.get() != '\n');
	exit(0);
}

//bool iequals(const string& a, const string& b)
//{
//	unsigned int sz = a.size();
//	if (b.size() != sz)
//		return false;
//	for (unsigned int i = 0; i < sz; ++i)
//		if (tolower(a[i]) != tolower(b[i]))
//			return false;
//	return true;
//}