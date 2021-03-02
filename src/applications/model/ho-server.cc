/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008,2009 INRIA, UDCAST
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
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/mmwave-helper.h"
#include "packet-loss-counter.h"

#include "seq-ts-header.h"
#include "ho-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("hoServer");

NS_OBJECT_ENSURE_REGISTERED (hoServer);

TypeId
hoServer::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::hoServer")
          .SetParent<Application> ()
          .SetGroupName ("Applications")
          .AddConstructor<hoServer> ()
          .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                         UintegerValue (100), MakeUintegerAccessor (&hoServer::m_port),
                         MakeUintegerChecker<uint16_t> ())
          .AddAttribute (
              "PacketWindowSize",
              "The size of the window used to compute the packet loss. This value should be a "
              "multiple of 8.",
              UintegerValue (32),
              MakeUintegerAccessor (&hoServer::GetPacketWindowSize, &hoServer::SetPacketWindowSize),
              MakeUintegerChecker<uint16_t> (8, 256))
          .AddTraceSource ("Rx", "A packet has been received",
                           MakeTraceSourceAccessor (&hoServer::m_rxTrace),
                           "ns3::Packet::TracedCallback")
          .AddTraceSource ("RxWithAddresses", "A packet has been received",
                           MakeTraceSourceAccessor (&hoServer::m_rxTraceWithAddresses),
                           "ns3::Packet::TwoAddressTracedCallback");
  return tid;
}

hoServer::hoServer () : m_lossCounter (0)
{
  NS_LOG_FUNCTION (this);
  m_received = 0;
}

hoServer::~hoServer ()
{
  NS_LOG_FUNCTION (this);
}

uint16_t
hoServer::GetPacketWindowSize () const
{
  NS_LOG_FUNCTION (this);
  return m_lossCounter.GetBitMapSize ();
}

void
hoServer::SetPacketWindowSize (uint16_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_lossCounter.SetBitMapSize (size);
}

uint32_t
hoServer::GetLost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_lossCounter.GetLost ();
}

uint64_t
hoServer::GetReceived (void) const
{
  NS_LOG_FUNCTION (this);
  return m_received;
}

void
hoServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
hoServer::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      if (m_socket->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&hoServer::HandleRead, this));

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (), m_port);
      if (m_socket6->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
    }

  m_socket6->SetRecvCallback (MakeCallback (&hoServer::HandleRead, this));

  std::cout << "Server :: " << m_socket << " " << m_socket6 << "\n";
}

void
hoServer::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0)
    {
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket>> ());
    }
}

void
hoServer::setDevs (std::vector<Ptr<LteEnbRrc>> enbRrcs1)
{
  // enbDevs = enbDevs1;
  // ueDevs = ueDevs1;
  enbRrcs = enbRrcs1;
  std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << enbRrcs[0] << std::endl;
}

std::vector<std::string>
hoServer::splitData (const std::string &s, char delim)
{
  std::vector<std::string> result;
  std::stringstream ss (s);
  std::string item;

  while (getline (ss, item, delim))
    {
      result.push_back (item);
    }

  return result;
}

void
hoServer::PerformHandover (int t_enb_no, int ue_no, int s_enb_no)
{

  std::cout << "Handover is called...."
            << "\n";
  // uint16_t targetCellId = targetEnbDev->GetObject<mmwave::MmWaveEnbNetDevice> ()->GetCellId ();
  // Ptr<LteEnbRrc> sourceRrc = sourceEnbDev->GetObject<mmwave::MmWaveEnbNetDevice> ()->GetRrc ();
  // uint16_t rnti = ueDev->GetObject<mmwave::MmWaveUeNetDevice> ()->GetRrc ()->GetRnti ();
  std ::cout << ">>>>>>>>>> Handover Params from controller: " << t_enb_no << ", " << ue_no << ", "
             << s_enb_no << "\n";
  // enbRrcs[s_enb_no]->SendHandoverRequest (ue_no, t_enb_no);
}

void
hoServer::HandleRead (Ptr<Socket> socket)
{
  std::cout << "Something got..." << socket << "\n";
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  Address localAddress;

  while ((packet = socket->RecvFrom (from)))
    {
      std::cout << packet->ToString () << "--\n";

      socket->GetSockName (localAddress);
      m_rxTrace (packet);
      m_rxTraceWithAddresses (packet, from, localAddress);
      if (packet->GetSize () > 0)
        {
          SeqTsHeader seqTs;
          packet->RemoveHeader (seqTs);

          uint8_t *buffer = new uint8_t[packet->GetSize ()];
          packet->CopyData (buffer, packet->GetSize ());
          std::string s = std::string (buffer, buffer + packet->GetSize ());
          std::cout << "Received:" << s << "\n";

          std::vector<std::string> v = splitData (s, '|');

          int ho_flag = stoi (v[0]);
          int t_enb_no = stoi (v[1]);
          int ue_no = stoi (v[2]);
          int s_enb_no = stoi (v[3]);

          std::cout << ho_flag << ", " << t_enb_no << ", " << ue_no << ", " << s_enb_no
                    << "+++++++++++++++++++++++++++" << std::endl;

          // std::cout << enbDevs.Get (0)->GetObject<MmWaveEnbNetDevice> ()->GetCellId () << std::endl;

          if (ho_flag == 1)
            PerformHandover (t_enb_no, ue_no, s_enb_no);
          else
            std::cout << "++++++++++++ HANDOVER DENIED ++++++++++++" << std::endl;

          uint32_t currentSequenceNumber = seqTs.GetSeq ();
          if (InetSocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () << " bytes from "
                                             << InetSocketAddress::ConvertFrom (from).GetIpv4 ()
                                             << " Sequence Number: " << currentSequenceNumber
                                             << " Uid: " << packet->GetUid () << " TXtime: "
                                             << seqTs.GetTs () << " RXtime: " << Simulator::Now ()
                                             << " Delay: " << Simulator::Now () - seqTs.GetTs ());
            }
          else if (Inet6SocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () << " bytes from "
                                             << Inet6SocketAddress::ConvertFrom (from).GetIpv6 ()
                                             << " Sequence Number: " << currentSequenceNumber
                                             << " Uid: " << packet->GetUid () << " TXtime: "
                                             << seqTs.GetTs () << " RXtime: " << Simulator::Now ()
                                             << " Delay: " << Simulator::Now () - seqTs.GetTs ());
            }

          m_lossCounter.NotifyReceived (currentSequenceNumber);
          m_received++;
        }
    }

  std::cout << "Addr ::" << from << "\n";
}

} // Namespace ns3
