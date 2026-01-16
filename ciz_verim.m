% --- THROUGHPUT (VERİM) GRAFİĞİ - PROFESYONEL STİL ---
% Mavi (Sabit) ve Turuncu (Mobil) yan yana, değerler üzerinde.

% 1. Dosyaları Oku
try
    sabit = csvread('sonuclar_sabit.csv');
    mobil = csvread('sonuclar_mobil.csv');
    disp('Dosyalar başarıyla okundu...');
catch
    error('CSV dosyaları bulunamadı!');
end

% 2. Verileri Hazırla (Sütun 6 = Throughput)
% Sıra: [AODV-20, OLSR-20, AODV-50, OLSR-50]
sabit_thr = [sabit(1,6), sabit(2,6), sabit(3,6), sabit(4,6)];
mobil_thr = [mobil(1,6), mobil(2,6), mobil(3,6), mobil(4,6)];

% Yan yana çubuklar için matris (Gruplandırma)
veri_matrisi = [sabit_thr; mobil_thr]'; 

% 3. Grafik Çizimi
h = figure('Position', [150, 150, 1000, 600], 'Visible', 'on');
b = bar(veri_matrisi);

% 4. Renk Ayarları (Diğer grafikle birebir aynı Mavi-Turuncu)
set(b(1), 'FaceColor', [0.12, 0.56, 1.00]); % Sabit (Mavi)
set(b(2), 'FaceColor', [1.00, 0.40, 0.10]); % Mobil (Turuncu)

% 5. Başlıklar ve Eksenler
title('Sabit ve Hareketli Senaryolarda Throughput (Verim) Analizi', 'FontSize', 16, 'FontWeight', 'bold');
ylabel('Throughput (Kbps)', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('Senaryolar', 'FontSize', 12, 'FontWeight', 'bold');

% X Ekseni Etiketleri
set(gca, 'XTick', 1:4, 'XTickLabel', {'AODV-20', 'OLSR-20', 'AODV-50', 'OLSR-50'}, 'FontSize', 11, 'FontWeight', 'bold');

% Lejant
legend({'Sabit (Durağan)', 'Hareketli (Mobil)'}, 'Location', 'south', 'FontSize', 11); % Lejantı aşağı aldım, sütunları kapatmasın
grid on;

% 6. Değerleri Sütunların Üzerine Yazma
% Sabit (Mavi) Sütunlar:
x_mavi = (1:4) - 0.14; 
for i = 1:4
    text(x_mavi(i), sabit_thr(i), sprintf('%.2f', sabit_thr(i)), ...
        'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom', 'FontSize', 10, 'FontWeight', 'bold');
end

% Mobil (Turuncu) Sütunlar:
x_turuncu = (1:4) + 0.14; 
for i = 1:4
    text(x_turuncu(i), mobil_thr(i), sprintf('%.2f', mobil_thr(i)), ...
        'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom', 'FontSize', 10, 'FontWeight', 'bold');
end

% 7. Kaydetme
print(h, 'KARSILASTIRMA_THROUGHPUT.png', '-dpng', '-r300');
disp('KARSILASTIRMA_THROUGHPUT.png başarıyla oluşturuldu.');