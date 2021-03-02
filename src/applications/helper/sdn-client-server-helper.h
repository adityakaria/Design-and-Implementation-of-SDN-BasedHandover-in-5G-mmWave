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
#ifndef sdn_CLIENT_SERVER_HELPER_H
#define sdn_CLIENT_SERVER_HELPER_H

#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/sdn-server.h"
#include "ns3/sdn-client.h"
namespace ns3 {
/**
 * \ingroup sdnclientserver
 * \brief Create a server application which waits for input sdn packets
 *        and uses the information carried into their payload to compute
 *        delay and to determine if some packets are lost.
 */
class sdnServerHelper
{
public:
  /**
   * Create sdnServerHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server application.
   *
   */
  sdnServerHelper ();

  /**
   * Create sdnServerHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server application.
   *
   * \param port The port the server will wait on for incoming packets
   */
  sdnServerHelper (uint16_t port);

  /**
   * Record an attribute to be set in each Application after it is is created.
   *
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   */
  void SetAttribute (std::string name, const AttributeValue &value);

  void SetHoClients(std::vector< Ptr<hoClient> > hoClients);

  /**
   * Create one sdn server application on each of the Nodes in the
   * NodeContainer.
   *
   * \param c The nodes on which to create the Applications.  The nodes
   *          are specified by a NodeContainer.
   * \returns The applications created, one Application per Node in the
   *          NodeContainer.
   */
  ApplicationContainer Install (NodeContainer c);

  /**
   * \brief Return the last created server.
   *
   * This function is mainly used for testing.
   *
   * \returns a Ptr to the last created server application
   */
  Ptr<sdnServer> GetServer (void);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<sdnServer> m_server; //!< The last created server application
};

/**
 * \ingroup sdnclientserver
 * \brief Create a client application which sends sdn packets carrying
 *  a 32bit sequence number and a 64 bit time stamp.
 *
 */
class sdnClientHelper
{

public:
  /**
   * Create sdnClientHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server.
   *
   */
  sdnClientHelper ();

  /**
   *  Create sdnClientHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server. Use this variant with
   * addresses that do not include a port value (e.g., Ipv4Address and
   * Ipv6Address).
   *
   * \param ip The IP address of the remote sdn server
   * \param port The port number of the remote sdn server
   */

  sdnClientHelper (Address ip, uint16_t port);
  /**
   *  Create sdnClientHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server. Use this variant with
   * addresses that do include a port value (e.g., InetSocketAddress and
   * Inet6SocketAddress).
   *
   * \param addr The address of the remote sdn server
   */

  sdnClientHelper (Address addr);

  /**
   * Record an attribute to be set in each Application after it is is created.
   *
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   */
  void SetAttribute (std::string name, const AttributeValue &value);

  /**
     * \param c the nodes
     *
     * Create one sdn client application on each of the input nodes
     *
     * \returns the applications created, one application per input node.
     */
  ApplicationContainer Install (NodeContainer c);

  std::vector< Ptr<sdnClient> > m_clients;


private:
  ObjectFactory m_factory; //!< Object factory.
};
/**
 * \ingroup sdnclientserver
 * Create sdnTraceClient application which sends sdn packets based on a trace
 * file of an MPEG4 stream. Trace files could be downloaded form :
 * http://www2.tkn.tu-berlin.de/research/trace/ltvt.html (the 2 first lines of
 * the file should be removed)
 * A valid trace file is a file with 4 columns:
 * \li -1- the first one represents the frame index
 * \li -2- the second one indicates the type of the frame: I, P or B
 * \li -3- the third one indicates the time on which the frame was generated by the encoder
 * \li -4- the fourth one indicates the frame size in byte
*/
class sdnTraceClientHelper
{
public:
  /**
   * Create sdnTraceClientHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server.
   *
   */
  sdnTraceClientHelper ();

  /**
   * Create sdnTraceClientHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server. Use this variant with
   * addresses that do not include a port value (e.g., Ipv4Address and
   * Ipv6Address).
   *
   * \param ip The IP address of the remote sdn server
   * \param port The port number of the remote sdn server
   * \param filename the file from which packet traces will be loaded
   */
  sdnTraceClientHelper (Address ip, uint16_t port, std::string filename);
  /**
   * Create sdnTraceClientHelper which will make life easier for people trying
   * to set up simulations with sdn-client-server. Use this variant with
   * addresses that do include a port value (e.g., InetSocketAddress and
   * Inet6SocketAddress).
   *
   * \param addr The address of the remote sdn server
   * \param filename the file from which packet traces will be loaded
   */
  sdnTraceClientHelper (Address addr, std::string filename);

  /**
    * Record an attribute to be set in each Application after it is is created.
    *
    * \param name the name of the attribute to set
    * \param value the value of the attribute to set
    */
  void SetAttribute (std::string name, const AttributeValue &value);

  /**
    * \param c the nodes
    *
    * Create one sdn trace client application on each of the input nodes
    *
    * \returns the applications created, one application per input node.
    */
  ApplicationContainer Install (NodeContainer c);

private:
  ObjectFactory m_factory; //!< Object factory.
};

} // namespace ns3

#endif /* sdn_CLIENT_SERVER_H */
