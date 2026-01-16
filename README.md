
# MANET Performance Analysis: AODV vs. OLSR Protocols using NS-3

Bu proje, Mobil Ad-Hoc Ağlarda (MANET) yaygın olarak kullanılan **AODV** (Ad-hoc On-Demand Distance Vector) ve **OLSR** (Optimized Link State Routing) yönlendirme protokollerinin performans analizini içermektedir. Simülasyonlar **NS-3 (Network Simulator 3)** ortamında gerçekleştirilmiştir.

## 📝 Proje Özeti
Bu çalışma kapsamında, farklı düğüm sayıları (20 ve 50 düğüm) ile hem sabit hem de hareketli senaryolar altında ağ performansı değerlendirilmiştir. Analiz edilen temel metrikler şunlardır:
* **Uçtan Uca Gecikme (End-to-End Delay)**
* **Ağ Verimliliği (Throughput)**
* **Paket Kayıp Oranı (Packet Loss Ratio)**

## 🛠 Kullanılan Teknolojiler ve Araçlar
* **Simülatör:** NS-3 (v3.xx)
* **Programlama Dili:** C++
* **Veri Analizi ve Grafik:** GNU Octave / MATLAB
* **İşletim Sistemi:** Ubuntu (Linux)

## 📂 Dosya Yapısı
* `bitirme_projesi.cc`: NS-3 üzerinde koşturulan ana simülasyon senaryosu.
* `ciz_gecikme.m`: Gecikme verilerini görselleştiren Octave betiği.
* `ciz_verim.m`: Throughput (verim) verilerini görselleştiren Octave betiği.
* `ciz_kayip.m`: Paket kaybı analizini yapan Octave betiği.
* `sonuclar_sabit.csv`: Sabit senaryo simülasyon çıktıları.
* `sonuclar_mobil.csv`: Hareketli senaryo simülasyon çıktıları.

##  Nasıl Çalıştırılır?
1. `.cc` uzantılı dosyayı NS-3 kurulumunuzdaki `scratch` klasörüne kopyalayın.
2. Terminal üzerinden `./ns3 run scratch/bitirme_projesi` komutu ile simülasyonu başlatın.
3. Oluşan `.csv` dosyalarını Octave betikleri ile çalıştırarak grafikleri elde edebilirsiniz.

##  Hazırlayan
* **Ayşe Nur AKINCI** - Mersin Üniversitesi, Bilgisayar Mühendisliği Bölümü
