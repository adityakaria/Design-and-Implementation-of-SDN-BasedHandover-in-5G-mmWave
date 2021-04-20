#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
#include <ns3/buildings-module.h>
#include <ns3/packet.h>
#include <ns3/tag.h>
#include <ns3/queue-size.h>
#include "ns3/netanim-module.h"

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store.h"
#include "ns3/mmwave-helper.h"
#include <ns3/buildings-helper.h>
#include "ns3/log.h"
#include <ns3/buildings-module.h>

#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
#include <ns3/buildings-module.h>
#include <ns3/packet.h>
#include <ns3/tag.h>
#include <ns3/queue-size.h>
#include "ns3/netanim-module.h"


#include "ns3/sdn-client-server-helper.h"

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store.h"
#include "ns3/mmwave-helper.h"
#include <ns3/buildings-helper.h>
#include "ns3/log.h"
#include <ns3/buildings-module.h>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store.h"
#include "ns3/mmwave-helper.h"
#include <ns3/buildings-helper.h>
#include "ns3/global-route-manager.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/log.h"
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
// #include <ns3/ofswitch13-module.h>
#include <ns3/internet-apps-module.h>
#include "ns3/netanim-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/mmwave-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/mmwave-point-to-point-epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
#include <ns3/buildings-module.h>
#include <ns3/packet.h>
#include <ns3/tag.h>
#include <ns3/queue-size.h>
#include "ns3/netanim-module.h"


using namespace ns3;
using namespace mmwave;



NS_LOG_COMPONENT_DEFINE ("5ghandover");

void
NotifyConnectionEstablishedUe (std::string context,
                               uint64_t imsi,
                               uint16_t cellid,
                               uint16_t rnti)
{
  std::cout << context
            << " UE IMSI " << imsi
            << ": connected to CellId " << cellid
            << " with RNTI " << rnti
            << std::endl;
}

void
NotifyHandoverStartUe (std::string context,
                       uint64_t imsi,
                       uint16_t cellid,
                       uint16_t rnti,
                       uint16_t targetCellId)
{
  std::cout << context
            << " UE IMSI " << imsi
            << ": previously connected to CellId " << cellid
            << " with RNTI " << rnti
            << ", doing handover to CellId " << targetCellId
            << std::endl;
}
void PerformHandover(Ptr<NetDevice> targetEnbDev , Ptr<NetDevice> ueDev, Ptr<NetDevice> sourceEnbDev){
  std::cout << "Handover is initiated...." << "\n";
    uint16_t targetCellId = targetEnbDev->GetObject<MmWaveEnbNetDevice> ()->GetCellId ();
    Ptr<LteEnbRrc> sourceRrc = sourceEnbDev->GetObject<MmWaveEnbNetDevice> ()->GetRrc ();
    uint16_t rnti = ueDev->GetObject<MmWaveUeNetDevice> ()->GetRrc ()->GetRnti ();
    // std :: cout << rnti << " " << targetCellId << "\n";
    sourceRrc->SendHandoverRequest (rnti, targetCellId);
}
void
NotifyHandoverEndOkUe (std::string context,
                       uint64_t imsi,
                       uint16_t cellid,
                       uint16_t rnti)
{
  std::cout << context
            << " UE IMSI " << imsi
            << ": successful handover to CellId " << cellid
            << " with RNTI " << rnti
            << std::endl;
}

void
NotifyConnectionEstablishedEnb (std::string context,
                                uint64_t imsi,
                                uint16_t cellid,  
                                uint16_t rnti)
{
  std::cout << context
            << " eNB CellId " << cellid
            << ": successful connection of UE with IMSI " << imsi
            << " RNTI " << rnti
            << std::endl;
}

void
NotifyHandoverStartEnb (std::string context,
                        uint64_t imsi,
                        uint16_t cellid,
                        uint16_t rnti,
                        uint16_t targetCellId)
{
  std::cout << context
            << " eNB CellId " << cellid
            << ": start handover of UE with IMSI " << imsi
            << " RNTI " << rnti
            << " to CellId " << targetCellId
            << std::endl;
}

void
NotifyHandoverEndOkEnb (std::string context,
                        uint64_t imsi,
                        uint16_t cellid,
                        uint16_t rnti)
{
  std::cout << context
            << " eNB CellId " << cellid
            << ": completed handover of UE with IMSI " << imsi
            << " RNTI " << rnti
            << std::endl;
}

void check(Ptr<sdnClient> ch){
  std::cout << "Called....1" << "\n";

  std::string name= "dhruvik123sksksk";
  char data[ name.length() ];
  std::strcpy(data,name.c_str());
  ch->SendData(data , 17);

  std::cout << "Called....2" << "\n";

  return;
}

// void check(Ptr<hoClient> ch){
//   std::cout << "Called....1" << "\n";

//   std::string name= "dhruvik123sksksk";
//   char data[ name.length() ];
//   std::strcpy(data,name.c_str());
//   ch->SendData(data , 17);

//   std::cout << "Called....2" << "\n";

//   return;
// }


void reportParams( NetDeviceContainer ueDevs, uint16_t indx , Ptr<sdnClient> ch){
  // std :: cout << "****************" << ueDevs.Get(indx)->GetObject<MmWaveUeNetDevice> ()->GetPhy()->avgLen << "\n";
  // std::cout << Simulator::Now() << "\n";
  std::string params = ueDevs.Get(indx)->GetObject<MmWaveUeNetDevice> ()->GetPhy()->GetParams();
  params = params + std::to_string(indx);
  char data[ params.length() ];
  std::strcpy(data,params.c_str());
  ch->SendData(data , params.length()+1);
  // std:: cout  << "Reporting.. || UE :: " << 0 << " | CELL :: " << indx << " | SINR :: " << ueDevs.Get(0)->GetObject<MmWaveUeNetDevice> ()->GetPhy()->frqAvg[indx].second / 15 << "\n";
  Simulator::Schedule ( MilliSeconds(200) , &reportParams , ueDevs , indx, ch );
}



int
main (int argc, char *argv[])
{

  LogComponentEnable ("sdnClient", LOG_LEVEL_INFO);
  LogComponentEnable ("sdnServer", LOG_LEVEL_INFO);

  LogComponentEnable ("hoClient", LOG_LEVEL_INFO);
  LogComponentEnable ("hoServer", LOG_LEVEL_INFO);


  // LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  uint16_t numberOfUes = 1;
  uint16_t numberOfEnbs = 2;
  double distance = 500.0; // mfrom 7.0.0.2 Sequence Num
  double yForUe = 1000.0;   // m
  double speed = 500;       // m/s
  double simTime = (double)(numberOfEnbs + 1) * distance / speed; // 1500 m / 20 m/s = 75 secs
  double enbTxPowerDbm = 46.0;

  Config::SetDefault ("ns3::UdpClient::Interval", TimeValue (MilliSeconds (10)));
  Config::SetDefault ("ns3::UdpClient::MaxPackets", UintegerValue (1000000));

  Config::SetDefault ("ns3::sdnClient::Interval", TimeValue (MilliSeconds (10)));
  Config::SetDefault ("ns3::sdnClient::MaxPackets", UintegerValue (1000000));

  Config::SetDefault ("ns3::MmWaveHelper::UseIdealRrc", BooleanValue (false));

  Packet::EnablePrinting();

  CommandLine cmd;
  cmd.AddValue ("simTime", "Total duration of the simulation (in seconds)", simTime);
  cmd.AddValue ("speed", "Speed of the UE (default = 20 m/s)", speed);
  cmd.AddValue ("enbTxPowerDbm", "TX power [dBm] used by HeNBs (default = 46.0)", enbTxPowerDbm);

  cmd.Parse (argc, argv);

 Ptr<MmWaveHelper> mmwaveHelper = CreateObject<MmWaveHelper> ();

  mmwaveHelper->SetAttribute ("PathlossModel", StringValue ("ns3::BuildingsObstaclePropagationLossModel"));
  mmwaveHelper->Initialize ();
  
  Ptr<MmWavePointToPointEpcHelper>  epcHelper = CreateObject<MmWavePointToPointEpcHelper> ();
  mmwaveHelper->SetEpcHelper (epcHelper);
  mmwaveHelper->SetSchedulerType ("ns3::MmWaveFlexTtiMacScheduler");
  // mmwaveHelper->SetLteHandoverAlgorithmType ("ns3::A2A4RsrqHandoverAlgorithm");
  mmwaveHelper->SetLteHandoverAlgorithmType ("ns3::NoOpHandoverAlgorithm");
  


  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  // NodeContainer remoteHostContainer;  
  // remoteHostContainer.Create (1);
  // Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  // internet.Install (remoteHostContainer);

  // // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.0010)));
  // NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  // ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  // Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);

  // // Routing of the Internet Host (towards the LTE network)
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  // Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  // // interface 0 is localhost, 1 is the p2p device
  // remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  /*
   * Network topology:
   *
   *      |     + --------------------------------------------------------->
   *      |     UE
   *      |
   *      |               d                   d                   d
   *    y |     |-------------------x-------------------x-------------------
   *      |     |                 eNodeB              eNodeB
   *      |   d |
   *      |     |
   *      |     |                                             d = distance
   *            o (0, 0, 0)                                   y = yForUe
   */


  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (numberOfEnbs);
  ueNodes.Create (numberOfUes);

  // Install Mobility Model in eNB
  Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < numberOfEnbs; i++)
    {
      Vector enbPosition (distance * (i + 1), distance, 0);
      enbPositionAlloc->Add (enbPosition);
    }


  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator (enbPositionAlloc);
  enbMobility.Install (enbNodes);
  BuildingsHelper::Install (enbNodes);

  // Install Mobility Model in UE
  MobilityHelper ueMobility;
  ueMobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  ueMobility.Install (ueNodes);
  ueNodes.Get (0)->GetObject<MobilityModel> ()->SetPosition (Vector (0, yForUe , 0));
  ueNodes.Get (0)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (speed, 0, 0));
  // ueNodes.Get (1)->GetObject<MobilityModel> ()->SetPosition (Vector (1000, yForUe , 0));
  
  BuildingsHelper::Install (ueNodes);


   Ptr<Node> controllerNode = CreateObject<Node> ();
  MobilityHelper controllermobility;
  Ptr<ListPositionAllocator> controllerPositionAlloc = CreateObject<ListPositionAllocator> ();
  controllerPositionAlloc->Add (Vector (720.0, 250.0, 0.0));
  controllermobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  controllermobility.SetPositionAllocator(controllerPositionAlloc);
  controllermobility.Install (controllerNode);
  BuildingsHelper::Install (controllerNode);

  internet.Install (controllerNode);

  // Ptr<OFSwitch13InternalHelper> of13Helper = CreateObject<OFSwitch13InternalHelper> ();
  // Ptr<OFSwitch13Controller> cntrl = of13Helper->InstallController (controllerNode);
  // // OFSwitch13DeviceContainer devs = of13Helper->InstallSwitch(enbNodes);
  // of13Helper->CreateOpenFlowChannels ();

  NetDeviceContainer internetDevices2 = p2ph.Install (pgw, controllerNode);

  ipv4h.SetBase ("1.1.0.0", "255.255.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices2);
  Ipv4Address cntrlAddress = internetIpIfaces.GetAddress (1);
  std::cout << cntrlAddress << "\n";
  // Routing of the Internet Host (towards the LTE network)
  // Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting1 = ipv4RoutingHelper.GetStaticRouting (controllerNode->GetObject<Ipv4> ());
  // // interface 0 is localhost, 1 is the p2p device
  remoteHostStaticRouting1->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);


  // Install LTE Devices in eNB and UEs
  Config::SetDefault ("ns3::MmWaveEnbPhy::TxPower", DoubleValue (enbTxPowerDbm));
  NetDeviceContainer enbDevs = mmwaveHelper->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueDevs = mmwaveHelper->InstallUeDevice (ueNodes);

  // 

  // std::cout <<"******************" << enbDevs.Get(0)->GetCellId() << "\n";


  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));

  mmwaveHelper->AttachToClosestEnb(ueDevs, enbDevs);

  // Add X2 interface
  mmwaveHelper->AddX2Interface (enbNodes);

   NS_LOG_LOGIC ("setting up applications");




  // Install and start applications on UEs and remote host
  uint16_t dlPort = 10000;
  uint16_t ulPort = 20000;

  // randomize a bit start times to avoid simulation artifacts
  // (e.g., buffer overflows due to packet transmissions happening
  // exactly at the same time)
  Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable> ();
  startTimeSeconds->SetAttribute ("Min", DoubleValue (0));
  startTimeSeconds->SetAttribute ("Max", DoubleValue (0.010));

  ApplicationContainer serverApps;
  sdnServerHelper sdnServerH (ulPort);
  serverApps.Add( sdnServerH.Install(controllerNode) );

  std::vector< Ptr<sdnClient> > sdnClients;
  std::vector< Ptr<hoClient> > hoClients;
  std::vector< Ptr<hoServer> > hoServers;

  for (uint32_t u = 0; u < numberOfUes; ++u)
    {
      Ptr<Node> ue = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);

      for (uint32_t b = 0; b < 1; ++b)
        {
          // ++dlPort;
          // ++ulPort;

          ApplicationContainer clientApps;
          

          // NS_LOG_LOGIC ("installing UDP UL app for UE " << u);
          sdnClientHelper sdnClientH ( cntrlAddress , ulPort);
          clientApps.Add (sdnClientH.Install (ue));

          sdnClients.push_back( sdnClientH.m_clients[0] );


          // NS_LOG_LOGIC ("installing UDP DL app for UE " << u);
          hoClientHelper hoClientH (ueIpIfaces.GetAddress (u), dlPort);
          clientApps.Add (hoClientH.Install (controllerNode));
          hoServerHelper hoServerH(dlPort);
          serverApps.Add( hoServerH.Install(ue) );

          hoClients.push_back( hoClientH.m_clients[0] );
          hoServers.push_back( hoServerH.GetServer() );

          // hoServers.back()->sourceEnbDev = enbDevs.Get(0);

          // if(u==0){
          //   Simulator::Schedule (Seconds(0.1), &check, sdnClientH);
          // }
          // else{
          //   Simulator::Schedule (Seconds(0.3), &check, sdnClientH);

          // }

          // ApplicationContainer temp = clientApps.Get(0);
          // temp.Get(0)->Send();

          // PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory",
          //                                      InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          // serverApps.Add (dlPacketSinkHelper.Install (ue));

          // NS_LOG_LOGIC ("installing UDP UL app for UE " << u);
          // UdpClientHelper ulClientHelper (cntrlAddress, ulPort);
          // clientApps.Add (ulClientHelper.Install (ue));
          // PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
          //                                      InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          // serverApps.Add (ulPacketSinkHelper.Install (controllerNode));

          Ptr<EpcTft> tft = Create<EpcTft> ();
          EpcTft::PacketFilter dlpf;
          dlpf.localPortStart = dlPort;
          dlpf.localPortEnd = dlPort;
          tft->Add (dlpf);
          EpcTft::PacketFilter ulpf;  
          ulpf.remotePortStart = ulPort;
          ulpf.remotePortEnd = ulPort;
          tft->Add (ulpf);
          EpsBearer bearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT);
          // mmwaveHelper->ActivateDataRadioBearer (ueDevs.Get (u), bearer);
          mmwaveHelper->ActivateDedicatedEpsBearer(ueDevs.Get (u), bearer,tft);
          // mmwaveHelper->ActivateDedicatedEpsBearer(5);

          Time startTime = Seconds (startTimeSeconds->GetValue ());
          serverApps.Start (startTime);
          clientApps.Start (startTime);

        } // end for b
    }
    //cntrl
    sdnServerH.SetHoClients(hoClients);

    std::vector< Ptr<NetDevice>  > enbDevsVect;
    for(int i=0;i<numberOfEnbs;i++){
      enbDevsVect.push_back( enbDevs.Get(i) );
    }


    // std:: cout << "------------>" << hoServers[0]->GetPacketWindowSize() << "\n";

    for(int i=0;i<numberOfUes;i++){

    // hoServers[0]->sourceEnbDev = enbDevs.Get(0);
    // hoServers[1]->sourceEnbDev = enbDevs.Get(1);

    // hoServers[0]->enbDevsVect = enbDevsVect;
    hoServers[i]->enbDevsVect = enbDevsVect;
  }


  for(int i=0;i<numberOfUes;i++){
    Simulator::Schedule (Seconds(0.1) , &reportParams , ueDevs , i, sdnClients[i] );
  }



    // Simulator::Schedule (Seconds(0.1), &check,  sdnClients[0] );




    // Simulator::Schedule (Seconds(0.1), &check,  sdnClients[1] );
    // sourceEnbDev->GetObject<MmWaveEnbNetDevice> ()->GetRrc ();
    // hoServer.Get(0)->sourceRrc = enbDevs.Get(0)->GetObject<MmWaveEnbNetDevice> ()->GetRrc ();
    // Simulator::Schedule (Seconds(0.1), &testtt,  hoServer.Get(0) , enbDevs.Get(0)->GetObject<MmWaveEnbNetDevice> ()->GetRrc () );


    // Simulator::Schedule (Seconds(0.1), &check,  hoClients[0] );
    // Simulator::Schedule (Seconds(0.1), &check,  hoClients[1] );

  // Simulator::Schedule (Seconds(1), &PerformHandover, enbDevs.Get(1) , ueDevs.Get(0) , enbDevs.Get(0) );


  mmwaveHelper->EnableTraces ();


  // connect custom trace sinks for RRC connection establishment and handover notification
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionEstablished",
                   MakeCallback (&NotifyConnectionEstablishedEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionEstablished",
                   MakeCallback (&NotifyConnectionEstablishedUe));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
                   MakeCallback (&NotifyHandoverStartEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
                   MakeCallback (&NotifyHandoverStartUe));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
                   MakeCallback (&NotifyHandoverEndOkEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
                   MakeCallback (&NotifyHandoverEndOkUe));


  Simulator::Stop (Seconds (2));

  AnimationInterface anim("A_4MM.xml");


  Simulator::Run ();

  // GtkConfigStore config;
  // config.ConfigureAttributes ();

  




  Simulator::Destroy ();
  return 0;

}


