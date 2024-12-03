clc
clear all
close all

% Nome do arquivo CSV
filename = 'arrays\temperatura_AMG_8833\Array_DadosAluminioNatural_200mm_AMG8833.txt';

% Ler o arquivo CSV
data = readmatrix(filename, 'Delimiter', ';');

% Aplicar o filtro de média móvel com 10 posições
% para cada ponto na série de dados, o script calcula a média dos 10 valores anteriores a esse ponto, 
% incluindo o ponto atual. Isso resulta em um novo vetor filtro_media_movel onde cada valor é a média 
% dos 10 valores anteriores a ele na série original.
filtro_media_movel = movmean(data,[5 0]);

% Exibir os dados lidos
disp('filtro_media_movel:');
disp(filtro_media_movel);

% Extrair a coluna mais relevante da coleta
coluna = filtro_media_movel(:, 20);
disp('Dados coluna:');
disp(coluna);

% Calcular a média dos dados da quarta coluna
media_coluna = coluna(end);


%
ultima_linha = filtro_media_movel(end, :);
disp('A última linha é:');
disp(ultima_linha);

% % Criar o gráfico de linha comparando os dados originais e filtrados
% figure;
% hold on;
% plot(coluna, 'b-.', 'Color', [0.0, 0.0, 1], 'LineWidth', 0.5,'DisplayName', 'Dados Originais');
% plot(filtro_media_movel, '-', 'Color', [1, 0.0, 0.0], 'LineWidth', 2, 'DisplayName', 'Média Móvel');
% yline(media_coluna, '-', 'Color', [0.93, 0.69, 0.13], 'LineWidth', 2, 'DisplayName', 'Valor Médio');
%
% % Adicionar a linha de referência de 200 mm
% yline(200, '--', 'Color', [0.0, 0.7, 0.0], 'LineWidth', 1.5, 'DisplayName', 'Referência: 200 mm');
%
% % Adicionar o valor médio no gráfico
% x_position = length(coluna)/2; % Posição X para o texto
% y_position = media_coluna + 50 ; % Ajustar a posição Y acima da linha
% text(x_position, y_position, sprintf('Média: %.2f', media_coluna),'HorizontalAlignment', 'center', 'FontSize', 10, 'Color', [0.0, 0.0, 0.0]);
%
% ylim([150 500]);
% title('Comparação dos Dados Originais e Filtrados (Vidro Branco 200mm)');
% xlabel('Tempo - seg');
% ylabel('Valor - mm');
% legend('show');
% grid on;
% hold off;

% Exibir o valor médio
disp('Valor médio da coluna:');
disp(media_coluna);
% % Exibir o array formatado
% fprintf('Array lido do arquivo: \n');
% fprintf('%f ', coluna);
% fprintf('\n');
