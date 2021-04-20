/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */
#include "ho-client-server-helper.h"
#include "ns3/ho-server.h"
#include "ns3/ho-client.h"
#include "ns3/udp-trace-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

hoServerHelper::hoServerHelper ()
{
  m_factory.SetTypeId (hoServer::GetTypeId ());
}

hoServerHelper::hoServerHelper (uint16_t port)
{
  m_factory.SetTypeId (hoServer::GetTypeId ());
  SetAttribute ("Port", UintegerValue (port));
}

void
hoServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
hoServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<hoServer> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

Ptr<hoServer>
hoServerHelper::GetServer (void)
{
  return m_server;
}

hoClientHelper::hoClientHelper ()
{
  m_factory.SetTypeId (hoClient::GetTypeId ());
}

hoClientHelper::hoClientHelper (Address address, uint16_t port)
{
  m_factory.SetTypeId (hoClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
}

hoClientHelper::hoClientHelper (Address address)
{
  m_factory.SetTypeId (hoClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
}

void
hoClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
hoClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<hoClient> client = m_factory.Create<hoClient> ();
      node->AddApplication (client);
      apps.Add (client);
      m_clients.push_back(client);
      // (apps.Get(0))->SendData();
      // client->SendData();
    }
  return apps;
}

hoTraceClientHelper::hoTraceClientHelper ()
{
  m_factory.SetTypeId (UdpTraceClient::GetTypeId ());
}

hoTraceClientHelper::hoTraceClientHelper (Address address, uint16_t port, std::string filename)
{
  m_factory.SetTypeId (UdpTraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
  SetAttribute ("TraceFilename", StringValue (filename));
}

hoTraceClientHelper::hoTraceClientHelper (Address address, std::string filename)
{
  m_factory.SetTypeId (UdpTraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("TraceFilename", StringValue (filename));
}

void
hoTraceClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
hoTraceClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<UdpTraceClient> client = m_factory.Create<UdpTraceClient> ();
      node->AddApplication (client);
      apps.Add (client);
      // apps.Get(0);
    }
  return apps;
}

} // namespace ns3
