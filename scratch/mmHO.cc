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

void PhySnirTrace2 (std::string context ,uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId)
{ 
  std::cout << rsrp << "\n";
}

void printParams(std::string context,uint64_t imsi , SpectrumValue& sinr , SpectrumValue& sinr2){
  std::cout << "curr cell :: " << imsi << " | " << sinr << "\n";
}

void printParams2(std::string context,uint64_t currCell, uint16_t rnti,uint16_t cellId, double sinr){
  std::cout << "Curr Cell:: " <<  currCell << " RNTI :: " << rnti << " Cell :: " << cellId  << " " << "SINR :: " << sinr << "\n";
}

/**
 * Sample simulation script for an automatic X2-based handover based on the RSRQ measures.
 * It instantiates two eNodeB, attaches one UE to the 'source' eNB.
 * The UE moves between both eNBs, it reports measures to the serving eNB and
 * the 'source' (serving) eNB triggers the handover of the UE towards
 * the 'target' eNB when it considers it is a better eNB.
 */
int
main (int argc, char *argv[])
{
  // LogLevel logLevel = (LogLevel)(LOG_PREFIX_ALL | LOG_LEVEL_ALL);

  // LogComponentEnable ("LteHelper", logLevel);
  // LogComponentEnable ("EpcHelper", logLevel);
  // LogComponentEnable ("EpcEnbApplication", logLevel);
  // LogComponentEnable ("EpcX2", logLevel);
  // LogComponentEnable ("EpcSgwPgwApplication", logLevel);

  // LogComponentEnable ("LteEnbRrc", logLevel);
  // LogComponentEnable ("LteEnbNetDevice", logLevel);
  // LogComponentEnable ("LteUeRrc", logLevel);
  // LogComponentEnable ("LteUeNetDevice" , logLevel);
  // LogComponentEnable ("A2A4RsrqHandoverAlgorithm", logLevel);
  // LogComponentEnable ("A3RsrpHandoverAlgorithm", logLevel);
  //int scenario = 2;
  uint16_t numberOfUes = 1;
  uint16_t numberOfEnbs = 2;
  uint16_t numBearersPerUe = 1;
  double distance = 500.0; // m
  double yForUe = 1000.0;   // m
  double speed = 500;       // m/s
  double simTime = (double)(numberOfEnbs + 1) * distance / speed; // 1500 m / 20 m/s = 75 secs
  double enbTxPowerDbm = 5.0;

  // change some default attributes so that they are reasonable for
  // this scenario, but do this before processing command line
  // arguments, so that the user is allowed to override these settings
  Config::SetDefault ("ns3::UdpClient::Interval", TimeValue (MilliSeconds (10)));
  Config::SetDefault ("ns3::UdpClient::MaxPackets", UintegerValue (1000000));
  Config::SetDefault ("ns3::MmWaveHelper::UseIdealRrc", BooleanValue (false));

  // Config::SetDefault("ns3::MmWaveUePhy::ReportCurrentCellRsrpSinr" , &printParams );

  // Config::SetDefault ("ns3::MmWavePhyMacCommon::ResourceBlockNum", UintegerValue (1));
  // Config::SetDefault ("ns3::MmWavePhyMacCommon::ChunkPerRB", UintegerValue (72));
  // Config::SetDefault ("ns3::MmWave3gppPropagationLossModel::Shadowing",BooleanValue(false));
  // Command line arguments
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
  
  //Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  //Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  //lteHelper->SetEpcHelper (epcHelper);
  

  //lteHelper->SetHandoverAlgorithmType ("ns3::A2A4RsrqHandoverAlgorithm");
  //lteHelper->SetHandoverAlgorithmAttribute ("ServingCellThreshold",
  //                                          UintegerValue (30));
  //lteHelper->SetHandoverAlgorithmAttribute ("NeighbourCellOffset",
  //                                          UintegerValue (1));

    //lteHelper->SetHandoverAlgorithmType ("ns3::A3RsrpHandoverAlgorithm");
    //lteHelper->SetHandoverAlgorithmAttribute ("Hysteresis",
      //                                        DoubleValue (3.0));
    //lteHelper->SetHandoverAlgorithmAttribute ("TimeToTrigger",
      //                                        TimeValue (MilliSeconds (256)));

  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;  
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);


  // Routing of the Internet Host (towards the LTE network)
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  // interface 0 is localhost, 1 is the p2p device
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

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

    // enbPositionAlloc->Add( Vector(0,0,0) );
    // enbPositionAlloc->Add( Vector(100,0,0) );



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

  BuildingsHelper::Install (ueNodes);

  // Install LTE Devices in eNB and UEs
  Config::SetDefault ("ns3::MmWaveEnbPhy::TxPower", DoubleValue (enbTxPowerDbm));
  NetDeviceContainer enbDevs = mmwaveHelper->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueDevs = mmwaveHelper->InstallUeDevice (ueNodes);
  //NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  //NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));

  for(int i=0;i<numberOfEnbs;i++){
    Ptr<LteEnbRrc> sourceRrc = enbDevs.Get(i)->GetObject<MmWaveEnbNetDevice> ()->GetRrc ();
  }


  // Attach all UEs to the first eNodeB
  //for (uint16_t i = 0; i < numberOfUes; i++)
    //{
      //lteHelper->Attach (ueLteDevs.Get (i), enbLteDevs.Get (0));
      mmwaveHelper->AttachToClosestEnb(ueDevs, enbDevs);
    //}


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

  for (uint32_t u = 0; u < numberOfUes; ++u)
    {
      Ptr<Node> ue = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);

      for (uint32_t b = 0; b < numBearersPerUe; ++b)
        {
          ++dlPort;
          ++ulPort;

          ApplicationContainer clientApps;
          ApplicationContainer serverApps;

          NS_LOG_LOGIC ("installing UDP DL app for UE " << u);
          UdpClientHelper dlClientHelper (ueIpIfaces.GetAddress (u), dlPort);
          clientApps.Add (dlClientHelper.Install (remoteHost));
          PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          serverApps.Add (dlPacketSinkHelper.Install (ue));

          NS_LOG_LOGIC ("installing UDP UL app for UE " << u);
          UdpClientHelper ulClientHelper (remoteHostAddr, ulPort);
          clientApps.Add (ulClientHelper.Install (ue));
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));

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


  // Add X2 interface
  mmwaveHelper->AddX2Interface (enbNodes);

  // X2-based Handover
  //lteHelper->HandoverRequest (Seconds (0.100), ueLteDevs.Get (0), enbLteDevs.Get (0), enbLteDevs.Get (1));

  // Uncomment to enable PCAP tracing
  // p2ph.EnablePcapAll("lena-x2-handover-measures");



//   Ptr<NetDevice> targetEnbDev = enbDevs.Get(1);
// Ptr<NetDevice> ueDev = ueDevs.Get(0);
// Ptr<NetDevice> sourceEnbDev = enbDevs.Get(0);

// uint16_t targetCellId = targetEnbDev->GetObject<MmWaveEnbNetDevice> ()->GetCellId ();
// Ptr<LteEnbRrc> sourceRrc = sourceEnbDev->GetObject<MmWaveEnbNetDevice> ()->GetRrc ();
// uint16_t rnti = ueDev->GetObject<MmWaveUeNetDevice> ()->GetRrc ()->GetRnti ();
// std :: cout << rnti << " " << targetCellId << "\n";
// sourceRrc->SendHandoverRequest (rnti, targetCellId);



  Simulator::Schedule (Seconds(1), &PerformHandover, enbDevs.Get(1) , ueDevs.Get(0) , enbDevs.Get(0) );






  // mmwaveHelper->EnablePhyTraces ();
  mmwaveHelper->EnableTraces ();
  //mmwaveHelper->EnableDlPhyTrace ();
  //mmwaveHelper->EnableUlPhyTrace ()
  // mmwaveHelper->EnableMcTraces ();
  //mmwaveHelper->EnableRlcTraces ();
  //mmwaveHelper->EnablePdcpTraces ();
  //Ptr<MmWaveBearerStatsCalculator> rlcStats = mmwaveHelper->GetRlcStats ();
  //rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (1.0)));
  //Ptr<MmWaveBearerStatsCalculator> pdcpStats = mmwaveHelper->GetPdcpStats ();
  //pdcpStats->SetAttribute ("EpochDuration", TimeValue (Seconds (1.0)));

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
  // Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
  //                  MakeCallback (&NotifyHandoverEndOkUe));
  // Config::Connect ("/NodeList/*/DeviceList/*/MmWaveUePhy/ReportCurrentCellRsrpSinr",
  //                       MakeCallback (&printParams));

  // Config::Connect ("/NodeList/*/DeviceList/*/MmWaveUePhy/ReportAllCellRsrpSinr",
  //                       MakeCallback (&printParams2));
  // AsciiTraceHelper asciiTraceHelper;
  // Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("ueTest.txt");
  // Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MmWaveUeNetDevice/ComponentCarrierMapUe/*/MmWaveUePhy/ReportCurrentCellRsrpSinr",
  // MakeBoundCallback (&ReportUeMeasurementsCallbackDetail,stream));
  // Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MmWaveUeNetDevice/ComponentCarrierMapUe/*/MmWaveUePhy/ReportCurrentCellRsrpSinr", MakeCallback (&printParams));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MmWaveUeNetDevice/ComponentCarrierMapUe/*/MmWaveUePhy/ReportAllCellRsrpSinr", MakeCallback (&printParams2));




  Simulator::Stop (Seconds (5));

  // AnimationInterface anim("mmHo.xml");

  // std::cout<<"\n";
  // std::cout<<"\n";
  // std::cout<<"    SINR values of Shard 2  \n";
  // std::cout<<"\n";
   // mmwaveHelper->EnableSidelinkTraces ();
// Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MmWaveUeNetDevice/ComponentCarrierMapUe/*/MmWaveUePhy/ReportCurrentCellRsrpSinr", MakeCallback (&printParams));
  
// AsciiTraceHelper asciiTraceHelper;
  // Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("ueTest.txt");
  // Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MmWaveUeNetDevice/ComponentCarrierMapUe/*/MmWaveUePhy/ReportCurrentCellRsrpSinr",MakeBoundCallback (&ReportUeMeasurementsCallbackDetail,stream));
 

// Config::Connect ("/NodeList/*/DeviceList/*/MmWaveUePhy/ReportCurrentCellRsrpSinr",MakeCallback (&printParams));

  Simulator::Run ();

  // GtkConfigStore config;
  // config.ConfigureAttributes ();

  




  Simulator::Destroy ();
  return 0;

}


