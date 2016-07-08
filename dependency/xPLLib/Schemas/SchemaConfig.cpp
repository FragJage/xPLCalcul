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
#include <sstream>
#include "SchemaConfig.h"

namespace xPL
{

using namespace std;

/****************************************************************************************************/
/*** Schema config.end                                                                              */
SchemaConfigEnd::SchemaConfigEnd() : SchemaHbeatEnd()
{
    SetClass("config");
}

SchemaConfigEnd::~SchemaConfigEnd()
{
}

/****************************************************************************************************/
/*** Schema config.basic                                                                            */
SchemaConfigBasic::SchemaConfigBasic() : SchemaHbeatBasic()
{
    SetClass("config");
}
SchemaConfigBasic::SchemaConfigBasic(int interval) : SchemaHbeatBasic(interval)
{
    SetClass("config");
}

SchemaConfigBasic::~SchemaConfigBasic()
{
}

/****************************************************************************************************/
/*** Schema config.app                                                                              */
SchemaConfigApp::SchemaConfigApp() : SchemaHbeatApp()
{
    SetClass("config");
}
SchemaConfigApp::SchemaConfigApp(int interval, int port, const string& remoteIP) : SchemaHbeatApp(interval, port, remoteIP)
{
    SetClass("config");
}

SchemaConfigApp::~SchemaConfigApp()
{
}

/****************************************************************************************************/
/*** Schema config.list                                                                             */
SchemaConfigList::SchemaConfigList() : SchemaObject(ISchema::stat, "config", "list")
{
}

SchemaConfigList::~SchemaConfigList()
{
}

/****************************************************************************************************/
/*** Schema config.current                                                                          */
SchemaConfigCurrent::SchemaConfigCurrent() : SchemaObject(ISchema::stat, "config", "current")
{
}

SchemaConfigCurrent::~SchemaConfigCurrent()
{
}

/****************************************************************************************************/
/*** Schema config.list -> Request                                                                  */
SchemaConfigListRequest::SchemaConfigListRequest() : SchemaObject(ISchema::cmnd, "config", "list")
{
    AddValue("command", "request");
}

SchemaConfigListRequest::~SchemaConfigListRequest()
{
}

void SchemaConfigListRequest::Check()
{
}

/****************************************************************************************************/
/*** Schema config.current -> Request                                                               */
SchemaConfigCurrentRequest::SchemaConfigCurrentRequest() : SchemaObject(ISchema::cmnd, "config", "current")
{
    AddValue("command", "request");
}

SchemaConfigCurrentRequest::~SchemaConfigCurrentRequest()
{
}

void SchemaConfigCurrentRequest::Check()
{
}

}
