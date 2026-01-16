% --- GECİKME (DELAY) GRAFİĞİ - PROFESYONEL STİL ---
% Bu kod, Sabit ve Mobil verileri yan yana (Grouped Bar) çizer.
% Mavi ve Turuncu renkleri kullanır.

% 1. Dosyaları Oku
try
    sabit = csvread('sonuclar_sabit.csv');
    mobil = csvread('sonuclar_mobil.csv');
    disp('Dosyalar başarıyla okundu...');
catch
    error('CSV dosyaları bulunamadı! Lütfen dosya isimlerini kontrol et.');
end

% 2. Verileri Hazırla (Sütun 3 = Delay)
% Gruplama: [AODV-20, OLSR-20, AODV-50, OLSR-50]
sabit_verileri = [sabit(1,3), sabit(2,3), sabit(3,3), sabit(4,3)];
mobil_verileri = [mobil(1,3), mobil(2,3), mobil(3,3), mobil(4,3)];

% Yan yana çubuklar için veriyi matris yapıyoruz
veri_matrisi = [sabit_verileri; mobil_verileri]';

% 3. Grafik Çizimi
h = figure('Position', [100, 100, 1000, 600], 'Visible', 'on');
b = bar(veri_matrisi);

% 4. Renk Ayarları (İstediğin Mavi ve Turuncu)
set(b(1), 'FaceColor', [0.12, 0.56, 1.00]); % Sabit (Parlak Mavi)
set(b(2), 'FaceColor', [1.00, 0.40, 0.10]); % Mobil (Turuncu)

% 5. Başlıklar ve Eksenler
title('Sabit ve Hareketli Senaryolarda Ortalama Gecikme (Delay)', 'FontSize', 16, 'FontWeight', 'bold');
ylabel('Gecikme Süresi (Saniye)', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('Senaryolar', 'FontSize', 12, 'FontWeight', 'bold');

% X Ekseni Etiketleri (AODV-20, OLSR-20...)
set(gca, 'XTick', 1:4, 'XTickLabel', {'AODV-20', 'OLSR-20', 'AODV-50', 'OLSR-50'}, 'FontSize', 11, 'FontWeight', 'bold');

% Lejant (Kutu açıklaması)
legend({'Sabit (Durağan)', 'Hareketli (Mobil)'}, 'Location', 'northwest', 'FontSize', 11);
grid on;

%
