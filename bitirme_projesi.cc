/*
  Bitirme Projesi: AODV ve OLSR Performans Analizi 
  Ad Soyad:Ayşe Nur Akıncı
  Öğrenci No: 22220030015
  Amaç: Protokolleri kıyaslamak, sonuçları hesaplamak ve Octave için kaydetmek.
 */

// 1. KÜTÜPHANELER
// Simülasyonun çalışması için gerekli modülleri çağırıyoruz.
#include "ns3/core-module.h"          
#include "ns3/network-module.h"       // Düğüm, paket ve ağ yapısı
#include "ns3/internet-module.h"   
#include "ns3/wifi-module.h"       
#include "ns3/mobility-module.h"     
#include "ns3/aodv-module.h"          
#include "ns3/olsr-module.h"          
#include "ns3/applications-module.h" 
#include "ns3/flow-monitor-module.h"  // Veri toplama ve analiz modülü
#include <fstream>                    // Dosya yazma (Csv/Excel) kütüphanesi

using namespace ns3; 

NS_LOG_COMPONENT_DEFINE ("BitirmeProjesi");

int main (int argc, char *argv[])
{
  // 2. SİMÜLASYON PARAMETRELERİ
  std::string protocol = "AODV"; // Varsayılan protokolümüz
  uint32_t nNodes = 20;          // Varsayılan düğüm sayısı
  double simulationTime = 30.0;  // Simülasyon süresi (saniye)

  // 3. KOMUT SATIRI YÖNETİMİ
  // Terminalden parametre girerek kodu tekrar derlemeden ayar değiştirmemizi sağlar.
  CommandLine cmd;
  cmd.AddValue ("protocol", "Kullanılacak Protokol (AODV veya OLSR)", protocol);
  cmd.AddValue ("nNodes", "Düğüm Sayısı", nNodes);
  cmd.Parse (argc, argv);

  // 4. DÜĞÜMLERİN OLUŞTURULMASI
  NodeContainer nodes;
  nodes.Create (nNodes); // Belirlenen sayıda boş düğüm yaratılır.

  // 5. KABLOSUZ AĞ (WI-FI) AYARLARI 
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b); // Geniş kapsama alanı için 802.11b standardı

  YansWifiPhyHelper wifiPhy; // Fiziksel katman (Anten vb.)
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ()); // Ortak bir kanal (frekans) oluştur

  WifiMacHelper wifiMac; 
  // Ağı Ad-hoc (altyapısız) moda alıyoruz. Modem yok, cihazlar direkt konuşur.Projenin MANET (Mobile Ad-hoc Network) olduğunu tanımlar.
  wifiMac.SetType ("ns3::AdhocWifiMac");

  NetDeviceContainer devices;
  devices = wifi.Install (wifiPhy, wifiMac, nodes); // Ayarları donanımlara uygula

  // 6. MOBİLİTE (KONUMLANDIRMA) 
  MobilityHelper mobility;
  // Düğümleri rastgele atmak yerine, deneylerin tutarlı olması için ızgara (Grid) şeklinde düzenli diziyoruz.
 /* mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (20.0), // 20 metre aralıkla
                                 "DeltaY", DoubleValue (20.0),
                                 "GridWidth", UintegerValue (5), // 5'erli sıralar halinde
                                 "LayoutType", StringValue ("RowFirst"));
  
  // Şimdilik düğümleri SABİT tutuyoruz
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");*/
  
 // YENİ MOBİLİTE KODU 
  // Alanı 100x100 metre yapıyoruz (Herkes menzil içinde!)
  mobility.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                 "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"),
                                 "Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=100.0]"));

  // Hızı Düşür: 1 m/s ile 5 m/s arası (Yürüyüş hızı)
  // Eski halini 20 m/s olarak denedim.Çok hızlıydı, kopuyordu
  mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
                             "Speed", StringValue ("ns3::UniformRandomVariable[Min=1.0|Max=5.0]"),
                             "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=2.0]"),
                             "PositionAllocator", StringValue ("ns3::RandomRectanglePositionAllocator"));
  
  mobility.Install (nodes);

  // 7. PROTOKOL SEÇİMİ (KARAR MEKANİZMASI) 
  InternetStackHelper internet;
  int protocolId = 0; // Octave dosyasında AODV=1, OLSR=2 diye kodlamak için değişken

  if (protocol == "AODV")
    {
      AodvHelper aodv;
      internet.SetRoutingHelper (aodv); // Yönlendirme yardımcısı olarak AODV'yi seç demek
      protocolId = 1; // Octave için kimlik: 1
      NS_LOG_UNCOND ("--- AODV Simülasyonu Başladı (" << nNodes << " Düğüm) ---");
    }
  else if (protocol == "OLSR")
    {
      OlsrHelper olsr;
      internet.SetRoutingHelper (olsr); // Yönlendirme yardımcısı olarak OLSR'yi seç demek
      protocolId = 2; // Octave için kimlik: 2
      NS_LOG_UNCOND ("--- OLSR Simülasyonu Başladı (" << nNodes << " Düğüm) ---");
    }

  internet.Install (nodes); // Protokolü tüm düğümlere yükle

  // IP Adreslerini Ata
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0"); // Ağ adresi
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  // 8. TRAFİK SENARYOSU (UDP ECHO) 
  // SERVER (Sunucu)  Son düğümü sunucu yapıyoruz.Paketleri alacak.
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (nNodes - 1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (simulationTime));

  // CLIENT (İstemci): İlk düğümü istemci yapıyoruz.Paketleri gönderir.
  UdpEchoClientHelper echoClient (interfaces.GetAddress (nNodes - 1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (10)); // 10 paket gönder
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.5))); // 0.5 sn arayla
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024)); // 1 KB boyutunda

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (15.0));
  clientApps.Stop (Seconds (simulationTime));

  // 9. VERİ TOPLAMA (FLOW MONITOR) 
  // Ağdaki tüm paketleri takip eden monitör modülünü kuruyoruz.
  // Bu sayede tek tek log okumak zorunda kalmayacağız.
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // 10. SİMÜLASYONU YÖNETME 
  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run (); // Simülasyonu BAŞLAT

  // 11. HESAPLAMA (OTOMATİK ANALİZ) 
  // Simülasyon bitti, monitörden verileri çekiyoruz.
  monitor->CheckForLostPackets ();
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

  // Toplamları tutacak değişkenler
  double totalDelay = 0;
  double totalJitter = 0;
  uint32_t totalRx = 0;    // Alınan paket
  uint32_t totalTx = 0;    // Gönderilen paket
  uint32_t totalLost = 0;  // Kayıp paket
  double totalRxBytes = 0; // Toplam veri (Throughput için)

  // Döngü ile tüm akışları gezip sayıları topluyoruz
  for (auto i = stats.begin (); i != stats.end (); ++i)
    {
      totalRx += i->second.rxPackets;
      totalTx += i->second.txPackets;
      totalLost += i->second.lostPackets;
      totalRxBytes += i->second.rxBytes;

      if (i->second.rxPackets > 0)
        {
          totalDelay += i->second.delaySum.GetSeconds();
          totalJitter += i->second.jitterSum.GetSeconds();
        }
    }

  // Ortalamaları hesaplamak için formüller
  double avgDelay = (totalRx > 0) ? (totalDelay / totalRx) : 0.0;
  double avgJitter = (totalRx > 0) ? (totalJitter / totalRx) : 0.0;
  double lossRatio = (totalTx > 0) ? (((double)(totalTx - totalRx) / totalTx) * 100.0) : 0.0;
  // Throughput (Kbps) = (Bayt * 8) / (Süre * 1000)
  double throughput = (totalRxBytes * 8.0) / (simulationTime * 1000.0);

  // 12. SONUÇLARI EKRANA YAZDIRMA 
  NS_LOG_UNCOND ("\n--- SONUÇLAR ---");
  NS_LOG_UNCOND ("Protokol: " << protocol << " | Düğüm: " << nNodes);
  NS_LOG_UNCOND ("Gecikme (Delay): " << avgDelay << " s");
  NS_LOG_UNCOND ("Kayıp (Loss)   : %" << lossRatio);
  NS_LOG_UNCOND ("Jitter         : " << avgJitter << " s");
  NS_LOG_UNCOND ("Throughput     : " << throughput << " Kbps\n");

  // 13. DOSYAYA KAYDETME (CSV - OCTAVE İÇİN) 
  // Verileri "sonuclar.csv" dosyasına virgülle ayırarak ekliyoruz.
  std::ofstream outData;
  outData.open("sonuclar.csv", std::ios::app); // app = append modu: Üstüne ekle, silme.

  // Sıra: ProtokolID, DüğümSayısı, Gecikme, Kayıp, Jitter, Throughput
  outData << protocolId << ","      
          << nNodes << ","          
          << avgDelay << ","        
          << lossRatio << ","       
          << avgJitter << ","       
          << throughput << std::endl; 

  outData.close(); // Dosyayı kapat

  Simulator::Destroy (); // Hafızayı temizle
  return 0;
}
