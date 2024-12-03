clc
clear all
close all

% Nome do arquivo CSV
filename = 'arrays\Array_DadosVidroBranco_20cm_VL53L5.txt';

% Ler o arquivo CSV
data = readmatrix(filename, 'Delimiter', ';');

% Exibir os dados lidos
disp('Dados lidos do arquivo:');
disp(data);

% Extrair a quarta coluna
quarta_coluna = data(:, 4);

% Aplicar o filtro de média móvel com 10 posições
filtro_media_movel = movmean(quarta_coluna, 10);

% Calcular a média dos dados da quarta coluna
media_quarta_coluna = mean(quarta_coluna);

% Criar o gráfico de linha comparando os dados originais e filtrados
figure;
hold on;
plot(quarta_coluna, 'b-.', 'Color', [0.0, 0.0, 1], 'LineWidth', 0.5,'DisplayName', 'Dados Originais');
plot(filtro_media_movel, '-', 'Color', [1, 0.0, 0.0], 'LineWidth', 2, 'DisplayName', 'Média Móvel');
yline(media_quarta_coluna, '-', 'Color', [0.93, 0.69, 0.13], 'LineWidth', 2, 'DisplayName', 'Valor Médio');
ylim([150 350]);
title('Comparação dos Dados Originais e Filtrados');
xlabel('Tempo - seg');
ylabel('Valor - mm');
legend('show');
grid on;
hold off;

% Exibir o valor médio
disp('Valor médio da quarta coluna:');
disp(media_quarta_coluna);
% % Exibir o array formatado
% fprintf('Array lido do arquivo: \n');
% fprintf('%f ', quarta_coluna);
% fprintf('\n');
