/**********************************************************************
 * Copyright 2012 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/
#include "actionflightmode.h"

ActionFlightMode::ActionFlightMode(PlatformUtil *platformUtil)
    : _platformUtil(platformUtil)
{

}

void
ActionFlightMode::activate(const Rule &rule) {
    int flightMode = rule.getFlightMode();

    if (flightMode == 0 || flightMode == 1) {
        qDebug("Setting flight mode %d", flightMode);
        _platformUtil->setFlightMode(flightMode);
    } else {
        qDebug("Not setting flight mode %d", flightMode);
    }

}
