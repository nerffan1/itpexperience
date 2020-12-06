// UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "stdafx.h"
#include <iostream>
#include "Physics.h"

int main()
{
    std::cout << "Hello World!\n";
    DbgAssert(Physics::UnitTest(), "Physics Test Wrong");



    return 0;
}
