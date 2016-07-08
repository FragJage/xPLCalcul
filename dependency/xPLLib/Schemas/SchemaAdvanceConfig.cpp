/*** LICENCE ***************************************************************************************/
/*
  xPPLib - Simple class to manage xPL & xAP protocol

  This file is part of xPPLib.

    xPPLib is free software : you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xPPLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xPPLib.  If not, see <http://www.gnu.org/licenses/>.
*/
/***************************************************************************************************/
#include "SchemaAdvanceConfig.h"

namespace xPL
{

using namespace std;

/****************************************************************************************************/
/*** Schema advanceconfig.list                                                                       */
SchemaAdvanceConfigList::SchemaAdvanceConfigList() : SchemaObject(ISchema::stat, "advanceconfig", "list")
{
}

SchemaAdvanceConfigList::~SchemaAdvanceConfigList()
{
}

/****************************************************************************************************/
/*** Schema advanceconfig.list -> Request                                                            */
SchemaAdvanceConfigListRequest::SchemaAdvanceConfigListRequest() : SchemaObject(ISchema::cmnd, "advanceconfig", "list")
{
    AddValue("command", "request");
}

SchemaAdvanceConfigListRequest::~SchemaAdvanceConfigListRequest()
{
}

void SchemaAdvanceConfigListRequest::Check()
{
}

/****************************************************************************************************/
/*** Schema advanceconfig.current                                                                          */
SchemaAdvanceConfigCurrent::SchemaAdvanceConfigCurrent() : SchemaObject(ISchema::stat, "advanceconfig", "current")
{
}

SchemaAdvanceConfigCurrent::~SchemaAdvanceConfigCurrent()
{
}

/****************************************************************************************************/
/*** Schema advanceconfig.current -> Request/Delete                                                  */
SchemaAdvanceConfigCurrentCmnd::SchemaAdvanceConfigCurrentCmnd() : SchemaObject(ISchema::cmnd, "advanceconfig", "current")
{
    AddValue("command", "request");
}

SchemaAdvanceConfigCurrentCmnd::~SchemaAdvanceConfigCurrentCmnd()
{
}

void SchemaAdvanceConfigCurrentCmnd::Check()
{
}

}
