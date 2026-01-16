% --- AODV vs OLSR Karşılaştırma Grafiği ---

% 1. Dosyaları Oku
sabit = csvread('sonuclar_sabit.csv');
mobil = csvread('sonuclar_mobil.csv');

% CSV Formatımız: [ProtID, Dugum, Gecikme, Kayip, Jitter, Throughput]
% Sütun 4 = Paket Kaybı (%)

% Verileri Çekiyoruz
% [AODV-20, OLSR-20, AODV-50, OLSR-50]
y_sabit = [sabit(1,4), sabit(2,4), sabit(3,4), sabit(4,4)];
y_mobil = [mobil(1,4), mobil(2,4), mobil(3,4), mobil(4,4)];

% Gruplandırılmış Veri Seti (Yan yana çubuklar için)
data = [y_sabit; y_mobil]';

% 2. Grafiği Çiz
figure; % Yeni pencere aç
h = bar(data);

% 3. Renk ve Görsellik Ayarları
set(h(1), 'FaceColor', [0.2 0.6 1.0]); % Mavi (Sabit Senaryo)
set(h(2), 'FaceColor', [1.0 0.4 0.2]); % Turuncu (Hareketli Senaryo)

% 4. Başlıklar ve Etiketler
title('Sabit ve Hareketli Senaryolarda Paket Kayip Orani');
ylabel('Paket Kayip Orani (%)');
xlabel('Senaryolar');
legend('Sabit (Durağan)', 'Hareketli (Mobil)', 'Location', 'NorthWest');
set(gca, 'XTickLabel', {'AODV-20', 'OLSR-20', 'AODV-50', 'OLSR-50'});
grid on; % Izgara çizgilerini aç

% 5. Değerleri Çubukların Üstüne Yaz
for i = 1:4
    % Sabit değerleri yaz
    text(i-0.15, y_sabit(i)+1, num2str(y_sabit(i), '%.1f'), 'FontSize', 10, 'HorizontalAlignment', 'center');
    % Mobil değerleri yaz
    text(i+0.15, y_mobil(i)+1, num2str(y_mobil(i), '%.1f'), 'FontSize', 10, 'HorizontalAlignment', 'center');
end

% 6. Grafiği Otomatik Kaydet (Resim olarak)
print -dpng 'karsilastirma_grafigi.png';
