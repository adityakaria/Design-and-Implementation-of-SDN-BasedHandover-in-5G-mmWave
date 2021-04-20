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
#include "sdn-client-server-helper.h"
#include "ns3/sdn-server.h"
#include "ns3/sdn-client.h"
#include "ns3/udp-trace-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

sdnServerHelper::sdnServerHelper ()
{
  m_factory.SetTypeId (sdnServer::GetTypeId ());
}

sdnServerHelper::sdnServerHelper (uint16_t port)
{
  m_factory.SetTypeId (sdnServer::GetTypeId ());
  SetAttribute ("Port", UintegerValue (port));
}

void
sdnServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

void 
sdnServerHelper::SetHoClients(std::vector< Ptr<hoClient> > hoClients){
  //m_server is already declared here, use it
  m_server->m_hoClients=hoClients;
}


ApplicationContainer
sdnServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<sdnServer> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}


Ptr<sdnServer>
sdnServerHelper::GetServer (void)
{
  return m_server;
}

sdnClientHelper::sdnClientHelper ()
{
  m_factory.SetTypeId (sdnClient::GetTypeId ());
}

sdnClientHelper::sdnClientHelper (Address address, uint16_t port)
{
  m_factory.SetTypeId (sdnClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
}

sdnClientHelper::sdnClientHelper (Address address)
{
  m_factory.SetTypeId (sdnClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
}

void
sdnClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
sdnClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<sdnClient> client = m_factory.Create<sdnClient> ();
      node->AddApplication (client);
      apps.Add (client);
      m_clients.push_back(client);
      // (apps.Get(0))->SendData();
      // client->SendData();
    }
  return apps;
}

sdnTraceClientHelper::sdnTraceClientHelper ()
{
  m_factory.SetTypeId (UdpTraceClient::GetTypeId ());
}

sdnTraceClientHelper::sdnTraceClientHelper (Address address, uint16_t port, std::string filename)
{
  m_factory.SetTypeId (UdpTraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
  SetAttribute ("TraceFilename", StringValue (filename));
}

sdnTraceClientHelper::sdnTraceClientHelper (Address address, std::string filename)
{
  m_factory.SetTypeId (UdpTraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("TraceFilename", StringValue (filename));
}

void
sdnTraceClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
sdnTraceClientHelper::Install (NodeContainer c)
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
